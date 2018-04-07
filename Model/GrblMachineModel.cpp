/*
 * GrblMachineModel.cpp
 *
 * Created: 24 2018 by Ashley
 *
 * Copyright 2018 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#include "Model/Settings/Settings.h"
#include "GrblMachineModel.h"
#include "Model/GrblMachineState.h"
#include "Utils/Util.h"
#include "Utils/IndexOutOfBoundsException.h"
#include <QMetaType>

GrblMachineModel::GrblMachineModel(QObject *parent)
    : QObject(parent),
      mState(GrblMachineState::Unknown),
      mLastState(GrblMachineState::Unknown),
      mMachinePosition(QVector3D(0.0,0.0,0.0)),
      mWorkPosition(QVector3D(0.0,0.0,0.0)),
      mFileEndSent(false),
      mFeedOverrideRate(100.0), // 100% of feed
      mProcessingFile(false),
      mTransferCompleted(false),
      mAborting(false),
      mProgramSendInterval(100),
      mCountProcessedCommands(0),
      mCommandQueueInitialSize(0),
      mUpdateRate(2),
      mCurrentFeedRate(0),
      mCurrentSpindleSpeed(0),
      mError(false),
      mErrorCode(-1)
{
    setupSerialPort();
    // Setup timer
    connect(
                &mProgramSendTimer, SIGNAL(timeout()),
                this, SLOT(onProgramSendTimerTimeout())
                );

    startProgramSendTimer();
}

GrblMachineModel::~GrblMachineModel()
{
    closePort();
}

void GrblMachineModel::setupSerialPort()
{
    // Setup serial port
    mSerialPort.setParity(QSerialPort::NoParity);
    mSerialPort.setDataBits(QSerialPort::Data8);
    mSerialPort.setFlowControl(QSerialPort::NoFlowControl);
    mSerialPort.setStopBits(QSerialPort::OneStop);

    mConnectionTimer.start(1000);

    connect(&mConnectionTimer,SIGNAL(timeout()),
            this, SLOT(onConnectionTimer()));
    connect(
                &mSerialPort, SIGNAL(readyRead()),
                this, SLOT(onSerialPortReadyRead()),
                Qt::QueuedConnection
                );
    connect(
                &mSerialPort, SIGNAL(error(QSerialPort::SerialPortError)),
                this, SLOT(onSerialPortError(QSerialPort::SerialPortError))
                );
}

void GrblMachineModel::onConnectionTimer()
{
    if (!mSerialPort.isOpen())
    {
        openPort();
    }
}

void GrblMachineModel::updateMachineCoordinates(const GrblResponse& response)
{
    // Update machine coordinates
    QString data = response.getData();
    static QRegExp machinePositionExpression("MPos:([^,]*),([^,]*),([^,]*)\\|FS:([^,]*),([^,>]*)");
    int caps = machinePositionExpression.indexIn(data);
    if (caps != -1)
    {
        for (int ix=0; ix<caps+1; ix++)
        {
            qDebug() << "GrblMachineModel: Status Cap"
                     << ix
                     <<  machinePositionExpression.cap(ix);
        }
        mMachinePosition = QVector3D
        (
            machinePositionExpression.cap(1).toFloat(),
            machinePositionExpression.cap(2).toFloat(),
            machinePositionExpression.cap(3).toFloat()
        );
        qDebug() << "GrblMachineModel: got MPos" << mMachinePosition;

        mCurrentFeedRate = machinePositionExpression.cap(4).toFloat();
        mCurrentSpindleSpeed = machinePositionExpression.cap(5).toFloat();

        emit updateMachinePositionSignal(mMachinePosition);
        emit updateFeedRateSignal(mCurrentFeedRate);
        emit updateSpindleSpeedSignal(mCurrentSpindleSpeed);
    }
}

void GrblMachineModel::updateWorkCoordinates(const GrblResponse& resp)
{
    // Update work coordinates
    static QRegExp workPositionExpression("WPos:([^,]*),([^,]*),([^,^>]*)");

    if (workPositionExpression.indexIn(resp.getData()) != -1)
    {
        mWorkPosition.setX(workPositionExpression.cap(1).toFloat());
        mWorkPosition.setY(workPositionExpression.cap(2).toFloat());
        mWorkPosition.setZ(workPositionExpression.cap(3).toFloat());
    }
    emit updateWorkPositionSignal(mWorkPosition);
}

void GrblMachineModel::parseGrblVersion(const GrblResponse& response)
{
    static QRegExp grblVersionRegex("Grbl ([\\w.]+)");

    if (grblVersionRegex.indexIn(response.getData().trimmed()) != -1)
    {
        mGrblVersion = grblVersionRegex.cap(1);
        qDebug() << "GrblMachineModel: Grbl Version" << mGrblVersion;
    }
}

void GrblMachineModel::processResponse(const GrblResponse& response)
{
    qDebug() << "GrblMachineModel: Process Response";

    mLastState = mState;
    GcodeCommand next;

    switch (response.getType())
    {
        case GrblResponseType::Startup:
            qDebug() << "GrblMachineModel: Got a Startup Message";
            parseGrblVersion(response);
            emit statusTextUpdateSignal("Found Machine",QColor("White"),QColor("Green"));
            break;
        case GrblResponseType::Locked:
            mState = GrblMachineState::Locked;
            emit statusTextUpdateSignal("Locked",QColor("Black"),QColor("Orange"));
            break;
        case GrblResponseType::Unlocked:
            mState = GrblMachineState::Unlocked;
            emit statusTextUpdateSignal("Unlocked & Ready",QColor("White"),QColor("Green"));
            break;
        case GrblResponseType::Status:
            updateMachineCoordinates(response);
            updateWorkCoordinates(response);
            break;
        case GrblResponseType::Ok:
            if (!mCommandBuffer.isEmpty())
            {
                next = mCommandBuffer.takeFirst();
                qDebug() << "GrblMachineModel: Popping"
                         << next.getID() << "/" << next.getLine()
                         << "from the buffer";
                next.setResponse(response);
                next.setState(GcodeCommandState::Processed);
                emit updateProgramTableStatusSignal(next);
                mCountProcessedCommands++;
            }
            else
            {
                qDebug() << "GrblMachineModel: Command Buffer is Empty";
                qDebug() << "GrblMachineModel: Job Completed";
                stopProgramSendTimer();
                emit setCompletionProgressSignal((int)getProcessedPercent());
                emit jobCompletedSignal();
            }
            break;
        case GrblResponseType::Error:
            parseError(response);
            mError = true;
            qDebug() << "GrblMachineModel: Error" << response.getData();
            stopProgramSendTimer();
            emit errorSignal(mErrorString);
            break;
    };

    if (mState != mLastState)
    {
        emit machineStateUpdatedSignal(mState);
    }

    if (response.getType() != GrblResponseType::Status)
    {
        emit appendResponseToConsoleSignal(response);
    }
}

void GrblMachineModel::parseError(const GrblResponse& error)
{
    if (!mError)
    {
        QString trimmedError = error.getData().trimmed();
        static QRegExp errorNumRegex("error:(\\d+)$");
        static QRegExp errorStrRegex("error:(.+)$");

        if (errorNumRegex.indexIn(trimmedError) != -1)
        {
            mErrorCode = errorNumRegex.cap(1).toInt();
            mErrorString = ERROR_STRINGS.at(mErrorCode);
        }
        else if (errorStrRegex.indexIn(trimmedError) != -1)
        {
            mErrorString = errorStrRegex.cap(1);
        }
        else
        {
            qDebug() << "GrblMachineModel: Error code not found in regex pattern";
        }
    }
}

double GrblMachineModel::getProcessedPercent()
{
    if (mCommandQueue.count() > 0)
        return (double)mCountProcessedCommands / (double)mCommandQueueInitialSize * 100;
    else
        return 100;
}

void GrblMachineModel::onSerialPortReadyRead()
{
    qDebug() << "GrblMachineModel: onSerialPortReadyRead";
    while (mSerialPort.canReadLine())
    {
        QString data = mSerialPort.readLine().trimmed();
        qDebug() << "GrblMachineModel: Got line from serial port" << data;
        GrblResponse response(data);
        processResponse(response);
    }
}

void GrblMachineModel::onSerialPortError(QSerialPort::SerialPortError error)
{
    static QSerialPort::SerialPortError previousError;

    if (error != QSerialPort::NoError && error != previousError)
    {
        qDebug() << "GrblMachineModel: onSerialPortError" << error;
        previousError = error;
        if (mSerialPort.isOpen())
        {
            mSerialPort.close();
        }
        emit serialPortErrorSignal(tr("Serial port error ") + QString::number(error) + ": " + mSerialPort.errorString());
    }
}

void GrblMachineModel::onProgramSendTimerTimeout()
{
    sendNextCommandFromQueue();
}

bool GrblMachineModel::openPort()
{
    if (mSerialPort.open(QIODevice::ReadWrite))
    {
        emit statusBarUpdateSignal(QString("Connected on Serial Port %1 @ %2 baud").arg(getPortName()).arg(getBaudRate()));
        emit statusTextUpdateSignal(QString("Port Open"),QColor("White"),QColor("Green"));
        //grblReset();
        return true;
    }
    else
    {
        emit statusBarUpdateSignal(QString("Unable to open port %1").arg(getPortName()));
    }
    return false;
}

bool GrblMachineModel::closePort()
{
    mSerialPort.close();
    return true;
}

void GrblMachineModel::initialise()
{

}

bool GrblMachineModel::sendNextCommandFromQueue()
{
    // Get the next queued command but don't remove it yet
    if (mCommandQueue.isEmpty())
    {
        return false;
    }

    GcodeCommand command = mCommandQueue.first();

    qDebug() << "GrblMachineModel: Attempting to send queued command"
             << (
                    command.getRawCommand() > 0 ?
                    QString::number(command.getRawCommand(),16) :
                    command.getCommand()
                );

    if (!mSerialPort.isOpen())
    {
        qDebug() << "GrblMachineModel: sendCommand FAILED -> SerialPort is not open";
        return false;
    }

    // Buffer full, append to queue
    if (!isSpaceInBuffer(command))
    {
        qDebug() << "GrblMachineModel: Buffer full, waiting... " << command.getCommand();
        return false;
    }
    // Buffer space available
    else
    {
        // Take the command off the queue for processing
        command = mCommandQueue.takeFirst();// feedOverride(mCommandQueue.takeFirst(),mFeedOverrideRate);

        if (command.getRawCommand() > 0)
        {
            emit appendCommandToConsoleSignal(command);
        }
        else if (command.getCommand() != "?")
        {
            emit appendCommandToConsoleSignal(command);
        }

        mCommandBuffer.append(command);

        if (!mError)
        {
            if (command.getRawCommand() > 0)
            {
                char c = command.getRawCommand();
                qDebug() << "GrblMachineModel: Writing raw command" << QString::number(c ,16);
                mSerialPort.write(&c ,1);
                mSerialPort.write("\r");
            }
            else
            {
                mSerialPort.write((command.getCommand() + "\r").toLatin1());
                command.setState(GcodeCommandState::Sent);
            }
        }
        else if (command.getRawCommand() > 0) // for real-time commands
        {
            char c = command.getRawCommand();
            qDebug() << "GrblMachineModel: Writing raw command" << QString::number(c ,16);
            mSerialPort.write(&c, 1);
            mSerialPort.write("\r");
        }

        return true;
    }
}

void GrblMachineModel::grblReset()
{
    qDebug() << "GrblMachineModel: Grbl Reset";
    mProcessingFile = false;
    mStatusReceived = false;
    // Drop all remaining commands in buffer
    clearCommandBuffer();
    clearCommandQueue();
    // Prepare reset response catch
    GcodeCommand cX = GcodeCommand::ControlXCommand();
    emit appendCommandToConsoleSignal(cX);
    mCommandBuffer.append(cX);
    // Write the C^X
    mSerialPort.write(QByteArray(1, static_cast<char>(24)));
    mSerialPort.flush();
}

void GrblMachineModel::onSendProgram(const GcodeFileModel& gcodeFile)
{
    qDebug() << "GrblMachineModel: onSendProgram()";

    clearCommandBuffer();
    clearCommandQueue();
    mCountProcessedCommands = 0;

    int index = 0;
    for (GcodeCommand next : gcodeFile.getData())
    {
        if (index != 0 && (index % mUpdateRate) == 0)
        {
            queueCommand(GcodeCommand::StatusUpdateCommand());
        }
        queueCommand(next);
        index++;
    }

    mCommandQueueInitialSize = mCommandQueue.count();
}

void GrblMachineModel::onSendProgramFromLine(const GcodeFileModel& gcodeFile, long id)
{
    qDebug() << "GrblMachineModel: onSendProgramFromLine @ id" << id;

    clearCommandBuffer();
    clearCommandQueue();
    mCountProcessedCommands = 0;

    int index = gcodeFile.getCommandByID(id).getLine();
    int size = gcodeFile.getData().count();

    for (; index < size; index++)
    {
        if (index != 0 && (index % mUpdateRate) == 0)
        {
            queueCommand(GcodeCommand::StatusUpdateCommand());
        }
        queueCommand(gcodeFile.getData()[index]);
    }

    mCommandQueueInitialSize = mCommandQueue.count();
}

void GrblMachineModel::startProgramSendTimer()
{
    if (!mProgramSendTimer.isActive())
    {
        qDebug() << "GrblMachineModel: Starting Program Timer";
        mProgramSendTimer.start(mProgramSendInterval);
    }
}

void GrblMachineModel::stopProgramSendTimer()
{
    qDebug() << "GrblMachineController: Stopping program send timer";
    mProgramSendTimer.stop();
    clearCommandBuffer();
    clearCommandQueue();
}

void GrblMachineModel::clearCommandQueue()
{
    mCommandQueue.clear();
    mCountProcessedCommands = 0;
    mCommandQueueInitialSize = 0;
}

void GrblMachineModel::clearCommandBuffer()
{
    mCommandBuffer.clear();
}

bool GrblMachineModel::isPortOpen()
{
    return mSerialPort.isOpen();
}

bool GrblMachineModel::isStatusReceived()
{
    return mStatusReceived;
}

void GrblMachineModel::setStatusReceived(bool rx)
{
    mStatusReceived=rx;
}

void GrblMachineModel::write(QByteArray data)
{
    mSerialPort.write(data.data(), data.length());
}

int GrblMachineModel::commandsQueueLength()
{
    return mCommandQueue.count();
}

QString GrblMachineModel::getPortName()
{
    return mSerialPort.portName();
}

void GrblMachineModel::setPortName(QString name)
{
    mSerialPort.setPortName(name);
}

void GrblMachineModel::setBaudRate(int baud)
{
    mSerialPort.setBaudRate(baud);
}

int GrblMachineModel::getBaudRate()
{
    return mSerialPort.baudRate();
}

void GrblMachineModel::onSettingChanged(QString group, QString param, QVariant value)
{
    qDebug() << "GrblMachineModel: onSettingChanged()"
             << group
             << param
             << value;

    if (group == Settings::SERIAL)
    {
        if (param == Settings::SERIAL_PORT_NAME)
        {
            setPortName(value.toString());
        }
        else if (param == Settings::SERIAL_BAUD_RATE)
        {
            setBaudRate(value.toInt());
        }
        closePort();
    }
}

void GrblMachineModel::onGcodeCommandManualSend(const GcodeCommand& command)
{
    queueCommand(command);
    startProgramSendTimer();
}

void GrblMachineModel::onUpdateSpindleSpeed(float speed)
{

}

void GrblMachineModel::onUpdateFeedRate(float rate)
{

}

void GrblMachineModel::queueCommand(const GcodeCommand& command)
{
    mCommandQueue.append(command);
}

int GrblMachineModel::bufferLengthInUse()
{
    int length = 0;

    for (GcodeCommand gc : mCommandBuffer)
    {
        length += gc.getCommandLength()+1;
    }

    qDebug() << "GrblMachineModel: Buffer in use:" << length;
    return length;
}

bool GrblMachineModel::isSpaceInBuffer(const GcodeCommand& cmd)
{
    int bufferLeft = BUFFER_LENGTH_LIMIT - bufferLengthInUse();
    int bufferUsed = BUFFER_LENGTH_LIMIT - bufferLeft;
    emit setBufferProgressSignal(((float)bufferUsed/BUFFER_LENGTH_LIMIT)*100);
    return (bufferLeft) > (cmd.getCommandLength()+1);
}

GcodeCommand GrblMachineModel::feedOverride(GcodeCommand& command, double overridePercent)
{
    return GcodeParser::overrideSpeed(command,overridePercent);
}

QVector3D GrblMachineModel::getMachinePosition()
{
    return mMachinePosition;
}

float GrblMachineModel::getMachinePositionX()
{
    return mMachinePosition.x();
}

float GrblMachineModel::getMachinePositionY()
{
    return mMachinePosition.y();
}

float GrblMachineModel::getMachinePositionZ()
{
    return mMachinePosition.z();
}

QVector3D GrblMachineModel::getWorkPosition()
{
    return mWorkPosition;
}

float GrblMachineModel::getWorkPositionX()
{
    return mWorkPosition.x();
}

float GrblMachineModel::getWorkPositionY()
{
    return mWorkPosition.y();
}

float GrblMachineModel::getWorkPositionZ()
{
    return mWorkPosition.z();
}

QString GrblMachineModel::stateToString(GrblMachineState state)
{
    switch (state)
    {
        case GrblMachineState::Unknown:
            return QString("Unknown");
        case GrblMachineState::Idle:
            return QString("Idle");
        case GrblMachineState::Alarm:
            return QString("Alarm");
        case GrblMachineState::Run:
            return QString("Running");
        case GrblMachineState::Home:
            return QString("Home");
        case GrblMachineState::Hold:
            return QString("Hold");
        case GrblMachineState::Queue:
            return QString("Queue");
        case GrblMachineState::Check:
            return QString("Check");
        case GrblMachineState::Door:
            return QString("Door");
        case GrblMachineState::Locked:
            return QString("Locked");
        case GrblMachineState::Unlocked:
            return QString("Unlocked");
    }
}

const int GrblMachineModel::BUFFER_LENGTH_LIMIT = 127;

const map<int,QString> GrblMachineModel::ERROR_STRINGS =
{
    {
        1,
        QString("G-code words consist of a letter and a value. Letter was not found.")
    },
    {
        2,
        QString("Numeric value format is not valid or missing an expected value.")
    },
    {
        3,
        QString("Grbl '$' system command was not recognized or supported.")
    },
    {
        4,
        QString("Negative value received for an expected positive value.")
    },
    {
        5,
        QString("Homing cycle is not enabled via settings.")
    },
    {
        6,
        QString("Minimum step pulse time must be greater than 3usec")
    },
    {
        7,
        QString("EEPROM read failed. Reset and restored to default values.")
    },
    {
        8,
        QString("Grbl '$' command cannot be used unless Grbl is IDLE. Ensures smooth operation during a job.")
    },
    {
        9,
        QString("G-code locked out during alarm or jog state")
    },
    {
        10,
        QString("Soft limits cannot be enabled without homing also enabled.")
    },
    {
        11,
        QString("Max characters per line exceeded. Line was not processed and executed.")
    },
    {
        12,
        QString("(Compile Option) Grbl '$' setting value exceeds the maximum step rate supported.")
    },
    {
        13,
        QString("Safety door detected as opened and door state initiated.")
    },
    {
        14,
        QString("(Grbl-Mega Only) Build info or startup line exceeded EEPROM line length limit.")
    },
    {
        15,
        QString("Jog target exceeds machine travel. Command ignored.")
    },
    {
        16,
        QString("Jog command with no '=' or contains prohibited g-code.")
    },
    {
        17,
        QString("Laser mode requires PWM output.")
    },
    {
        20,
        QString("Unsupported or invalid g-code command found in block.")
    },
    {
        21,
        QString("More than one g-code command from same modal group found in block.")
    },
    {
        22,
        QString("Feed rate has not yet been set or is undefined.")
    },
    {
        23,
        QString("G-code command in block requires an integer value.")
    },
    {
        24,
        QString("Two G-code commands that both require the use of the XYZ axis words were detected in the block.")
    },
    {
        25,
        QString("A G-code word was repeated in the block.")
    },
    {
        26,
        QString("A G-code command implicitly or explicitly requires XYZ axis words in the block, but none were detected.")
    },
    {
        27,
        QString("N line number value is not within the valid range of 1 - 9,999,999.")
    },
    {
        28,
        QString("A G-code command was sent, but is missing some required P or L value words in the line.")
    },
    {
        29,
        QString("Grbl supports six work coordinate systems G54-G59. G59.1, G59.2, and G59.3 are not supported.")
    },
    {
        30,
        QString("The G53 G-code command requires either a G0 seek or G1 feed motion mode to be active. A different motion was active.")
    },
    {
        31,
        QString("There are unused axis words in the block and G80 motion mode cancel is active.")
    },
    {
        32,
        QString("A G2 or G3 arc was commanded but there are no XYZ axis words in the selected plane to trace the arc.")
    },
    {
        33,
        QString("The motion command has an invalid target. G2, G3, and G38.2 generates this error, if the arc is impossible to generate or if the probe target is the current position.")
    },
    {
        34,
        QString("A G2 or G3 arc, traced with the radius definition, had a mathematical error when computing the arc geometry. Try either breaking up the arc into semi-circles or quadrants, or redefine them with the arc offset definition.")
    },
    {
        35,
        QString("A G2 or G3 arc, traced with the offset definition, is missing the IJK offset word in the selected plane to trace the arc.")
    },
    {
        36,
        QString("There are unused, leftover G-code words that aren't used by any command in the block.")
    },
    {
        37,
        QString("The G43.1 dynamic tool length offset command cannot apply an offset to an axis other than its configured axis. The Grbl default axis is the Z-axis.")
    }
};
