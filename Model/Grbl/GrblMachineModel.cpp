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
#include "Model/Grbl/GrblMachineState.h"
#include "Utils/Util.h"
#include "Utils/IndexOutOfBoundsException.h"
#include <QMetaType>

GrblMachineModel::GrblMachineModel(QObject *parent)
    : QObject(parent),
      mState(GrblMachineState::Unknown),
      mLastState(GrblMachineState::Unknown),
      mMachinePosition(QVector3D(0.0,0.0,0.0)),
      mWorkPosition(QVector3D(0.0,0.0,0.0)),
      mWorkCoordinateOffset(QVector3D(0.0,0.0,0.0)),
      mSettingsModelHandle(nullptr),
      mFileEndSent(false),
      mFeedOverrideRate(100.0), // 100% of feed
      mProcessingFile(false),
      mTransferCompleted(false),
      mAborting(false),
      mProgramSendInterval(1000/50),
      mStatusInterval(1000/20),
      mCountProcessedCommands(0),
      mCommandQueueInitialSize(0),
      mCurrentFeedRate(0),
      mCurrentSpindleSpeed(0),
      mError(false),
      mErrorCode(-1),
      mBytesWaiting(0),
      mStatusRequested(false),
      mWaitingForStatus(false)
{
    setupSerialPort();
    // Setup timer
    connect(
        &mProgramSendTimer, SIGNAL(timeout()),
        this, SLOT(onProgramSendTimerTimeout())
    );
    connect(
        &mStatusTimer, SIGNAL(timeout()),
        this, SLOT(onStatusTimerTimeout())
    );

    startProgramSendTimer();
    startStatusTimer();
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
    connect(&mSerialPort, SIGNAL(readyRead()),this, SLOT(onSerialPortReadyRead()),Qt::DirectConnection);
    connect(&mSerialPort, SIGNAL(bytesWritten(qint64)), this, SLOT(onSerialBytesWritten(qint64)),Qt::DirectConnection);
    connect(&mSerialPort, SIGNAL(error(QSerialPort::SerialPortError)),this, SLOT(onSerialPortError(QSerialPort::SerialPortError)));
}

void GrblMachineModel::onConnect()
{
    if (!mSerialPort.isOpen())
    {
        /*qDebug() << "GrblMachineModel: Attempting to open port"
             << getPortName()
             << getBaudRate();
             */
        openPort();
    }
}

void GrblMachineModel::updateMachinePosition(const GrblResponse& response)
{
    // Update machine coordinates
    QString data = response.getData();
    static QRegExp machinePositionExpression("MPos:([^,]*),([^,]*),([^,|]*)");
    int caps = machinePositionExpression.indexIn(data);
    if (caps != -1)
    {
        for (int ix=0; ix<caps+1; ix++)
        {
            //qDebug() << "GrblMachineModel: Status Cap"
             //        << ix
              //       <<  machinePositionExpression.cap(ix);
        }
        mMachinePosition = QVector3D
        (
            machinePositionExpression.cap(1).toFloat(),
            machinePositionExpression.cap(2).toFloat(),
            machinePositionExpression.cap(3).toFloat()
        );

        //qDebug() << "GrblMachineModel: got MPos" << mMachinePosition;

//        mCurrentFeedRate = machinePositionExpression.cap(4).toFloat();
//        mCurrentSpindleSpeed = machinePositionExpression.cap(5).toFloat();

        emit updateMachinePositionSignal(mMachinePosition);
        emit updateFeedRateSignal(mCurrentFeedRate);
        emit updateSpindleSpeedSignal(mCurrentSpindleSpeed);
    }
}

void GrblMachineModel::updateWorkCoordinateOffset(const GrblResponse& resp)
{
    // Update work coordinates
    static QRegExp workCoordExpression("WCO:([^,]*),([^,]*),([^,^>]*)");
    if (workCoordExpression.indexIn(resp.getData()) != -1)
    {
        mWorkCoordinateOffset.setX(workCoordExpression.cap(1).toFloat());
        mWorkCoordinateOffset.setY(workCoordExpression.cap(2).toFloat());
        mWorkCoordinateOffset.setZ(workCoordExpression.cap(3).toFloat());
        //qDebug() << "GrblMachineModel: Got WCO" << mWorkCoordinateOffset;
    }
}

void GrblMachineModel::updateWorkPosition()
{
    mWorkPosition.setX(mMachinePosition.x() - mWorkCoordinateOffset.x());
    mWorkPosition.setY(mMachinePosition.y() - mWorkCoordinateOffset.y());
    mWorkPosition.setZ(mMachinePosition.z() - mWorkCoordinateOffset.z());
    //qDebug() << "GrblMachineModel: Got WPos" << mWorkPosition;
    emit updateWorkPositionSignal(mWorkPosition);
}

void GrblMachineModel::parseGrblVersion(const GrblResponse& response)
{
    static QRegExp grblVersionRegex("Grbl ([\\w.]+)");

    if (grblVersionRegex.indexIn(response.getData().trimmed()) != -1)
    {
        mGrblVersion = grblVersionRegex.cap(1);
        //qDebug() << "GrblMachineModel: Grbl Version" << mGrblVersion;
    }
}

void GrblMachineModel::processResponse(const GrblResponse& response)
{
//    qDebug() << "GrblMachineModel: Process Response" << response.getData();

    mLastState = mState;
    GcodeCommand* next = nullptr;

    switch (response.getType())
    {
        case GrblResponseType::Startup:
            //qDebug() << "GrblMachineModel: Got a Startup Message";
            parseGrblVersion(response);
            emit appendResponseToConsoleSignal(response);
            mStatusRequested = false;
            mWaitingForStatus = false;
            break;
        case GrblResponseType::Locked:
            mState = GrblMachineState::Locked;
            emit appendResponseToConsoleSignal(response);
            break;
        case GrblResponseType::Unlocked:
            mState = GrblMachineState::Unlocked;
            emit appendResponseToConsoleSignal(response);
            break;
        case GrblResponseType::Status:
            updateStatus(response);
            updateMachinePosition(response);
            updateWorkCoordinateOffset(response);
            updateWorkPosition();
            //qDebug() << "GrblMachineModel: Got status!";
            mStatusRequested = false;
            mWaitingForStatus = false;
            break;
        case GrblResponseType::Ok:
            if (!mCommandBuffer.isEmpty())
            {
                next = mCommandBuffer.takeFirst();
                /*qDebug() << "GrblMachineModel: Popping"
                         << next->getID() << "/" << next->getLine()
                         << "from the buffer";
                         */
                next->setResponse(response);
                next->setState(GcodeCommandState::Processed);
                emit updateProgramTableStatusSignal(next);
                mCountProcessedCommands++;
                emit setCompletionProgressSignal(getProcessedPercent());
                //emit appendResponseToConsoleSignal(response);
            }
            break;
        case GrblResponseType::Error:
            parseError(response);
            mError = true;
            //qDebug() << "GrblMachineModel: Error" << response.getData();
            stopProgramSendTimer();
            //stopStatusTimer();
            emit errorSignal(mErrorString);
            emit appendResponseToConsoleSignal(response);
            break;
        case GrblResponseType::Configuration:
            parseConfigurationResponse(response);
            break;
    };

    if (mState != mLastState)
    {
        emit machineStateUpdatedSignal(mState);
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
            //qDebug() << "GrblMachineModel: Error code not found in regex pattern";
        }
    }
}

int GrblMachineModel::getProcessedPercent()
{
    if (mCommandQueue.count() > 0)
        return
        static_cast<int>(
            (
                static_cast<double>(mCountProcessedCommands)
                / static_cast<double>(mCommandQueueInitialSize)
            )
            * 100
        );
    else
        return 0;
}

void GrblMachineModel::onSerialPortReadyRead()
{
    //qDebug() << "GrblMachineModel: onSerialPortReadyRead";
    while (mSerialPort.canReadLine())
    {
        QString data = mSerialPort.readLine().trimmed();
        //qDebug() << "GrblMachineModel: Got line from serial port" << data;
        GrblResponse response(data);
        processResponse(response);
    }
}

void GrblMachineModel::onSerialPortError(QSerialPort::SerialPortError error)
{
    static QSerialPort::SerialPortError previousError;

    if (error == QSerialPort::TimeoutError)
    {
        //qDebug() << "GrblMachineModel: serial wait timeout";
        return;
    }

    if (error != QSerialPort::NoError && error != previousError)
    {
        //qDebug() << "GrblMachineModel: onSerialPortError" << error;
        previousError = error;
        if (mSerialPort.isOpen())
        {
            mSerialPort.close();
        }
        emit serialPortErrorSignal(tr("Serial port error ") + QString::number(error) + ": " + mSerialPort.errorString());
    }
}

void GrblMachineModel::onSerialPortNameChanged(QString port)
{
   setPortName(port);
   closePort();
}

void GrblMachineModel::onSerialPortBaudRateChanged(int baud)
{
   setBaudRate(baud);
   closePort();
}

void GrblMachineModel::onProgramSendTimerTimeout()
{
    if (mStatusRequested)
    {
        if (mWaitingForStatus)
        {
            //qDebug() << "GrblMachineModel: Still waiting for status";
            return;
        }

        else if (mSerialPort.isOpen() && mBytesWaiting == 0)
        {
            //qDebug() << "GrblMachineModel: Requesting status";
            mBytesWaiting += mSerialPort.write
            (
                QString(GcodeCommand::StatusUpdateCommand()->getCommand()).toLatin1()
            );
            mStatusRequested = true;
            mWaitingForStatus = true;
        }
    }
    else if (!mWaitingForStatus)
    {
        sendNextCommandFromQueue();
    }
}

void GrblMachineModel::onStatusTimerTimeout()
{
    if (!mWaitingForStatus)
    {
        mStatusRequested = true;
    }
}

void GrblMachineModel::setSettingsModelHandle(SqlSettingsModel* settingsModelHandle)
{
    mSettingsModelHandle = settingsModelHandle;
}

bool GrblMachineModel::openPort()
{
    if (mSerialPort.open(QIODevice::ReadWrite))
    {
        emit statusBarUpdateSignal(QString("Connected on Serial Port %1 @ %2 baud").arg(getPortName()).arg(getBaudRate()));
        emit machineConnectedSigal(true);
        return true;
    }
    else
    {
        emit machineConnectedSigal(false);
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
//        qDebug() << "GrblMachineModel: Command queue is empty";
        return false;
    }

    GcodeCommand* command = mCommandQueue.first();

    /*qDebug() << "GrblMachineModel: Attempting to send queued command"
             << (
                    command->getRawCommand() > 0 ?
                    QString::number(command->getRawCommand(),16) :
                    command->getCommand()
                );
                */

    if (!mSerialPort.isOpen())
    {
        //qDebug() << "GrblMachineModel: sendCommand FAILED -> SerialPort is not open";
        mProgramSendTimer.stop();
        return false;
    }

    // Buffer full, append to queue
    if (!isSpaceInBuffer(command))
    {
        //qDebug() << "GrblMachineModel: Buffer full, waiting... " << command->getCommand();
        return false;
    }

    // Serial port has been flushed
    if (mBytesWaiting == 0)
    {
        if (!mError)
        {
            // Take the command off the queue for processing
            command = mCommandQueue.takeFirst();// feedOverride(mCommandQueue.takeFirst(),mFeedOverrideRate);

            // Don't append raw commands
            if (command->getRawCommand() == 0)
            {
                emit appendCommandToConsoleSignal(command);
            }


            mCommandBuffer.append(command);

            if (command->getRawCommand() > 0)
            {
                char c = command->getRawCommand();
                //qDebug() << "GrblMachineModel: Writing raw command" << QString::number(c ,16);
                mBytesWaiting += mSerialPort.write(&c ,1);
                //mBytesWaiting += mSerialPort.write("\r");
            }
            else
            {
                mBytesWaiting += mSerialPort.write(QString(command->getCommand() + "\r").toLatin1());
                command->setState(GcodeCommandState::Sent);
            }

            return true;
        }
        else
        {
            qDebug() << "GrblMachineController: Cannot send command, in error state" << command->getCommand();
        }
    }
    else
    {
        qDebug() << "GrblMachineController: Cannot send command, bytes waiting:"
                 << mBytesWaiting
                 << command->getCommand();
    }
    mSerialPort.flush();
    return false;
}

void GrblMachineModel::onSerialBytesWritten(qint64 bytes)
{
    mBytesWaiting -= bytes;
    /*qDebug() << "GrblMachineModel: Serial bytes Written:" << bytes
             << "/ Remaining:" << mBytesWaiting;
             */
}

void GrblMachineModel::grblReset()
{
    //qDebug() << "GrblMachineModel: Grbl Reset";
    mProcessingFile = false;
    // Drop all remaining commands in buffer
    clearCommandBuffer();
    clearCommandQueue();
    // Write the C^X
    mBytesWaiting += mSerialPort.write(QByteArray(1, static_cast<char>(24)));
}

void GrblMachineModel::onSendProgram(const GcodeFileModel& gcodeFile)
{
    //qDebug() << "GrblMachineModel: onSendProgram()";

    clearCommandBuffer();
    clearCommandQueue();
    mCountProcessedCommands = 0;

    int index = 0;
    for (GcodeCommand* next : gcodeFile.getData())
    {
        queueCommand(next);
        index++;
    }

    mCommandQueueInitialSize = mCommandQueue.count();
}

void GrblMachineModel::onSendProgramFromLine(const GcodeFileModel& gcodeFile, long id)
{
    //qDebug() << "GrblMachineModel: onSendProgramFromLine @ id" << id;

    clearCommandBuffer();
    clearCommandQueue();
    mCountProcessedCommands = 0;

    int index = gcodeFile.getCommandByID(id)->getLine();
    int size = gcodeFile.getData().count();

    for (; index < size; index++)
    {
        queueCommand(gcodeFile.getData()[index]);
    }

    mCommandQueueInitialSize = mCommandQueue.count();
}

void GrblMachineModel::startStatusTimer()
{
    if (!mStatusTimer.isActive())
    {
        //qDebug() << "GrblMachineModel: Starting Status Timer";
        mStatusTimer.start(mStatusInterval);
    }
}

void GrblMachineModel::startProgramSendTimer()
{
    if (!mProgramSendTimer.isActive())
    {
        //qDebug() << "GrblMachineModel: Starting Program Timer";
        mProgramSendTimer.start(mProgramSendInterval);
    }
}

void GrblMachineModel::stopProgramSendTimer()
{
    //qDebug() << "GrblMachineController: Stopping program send timer";
    mProgramSendTimer.stop();
    clearCommandBuffer();
    clearCommandQueue();
}

void GrblMachineModel::stopStatusTimer()
{
    //qDebug() << "GrblMachineController: Stopping status timer";
    mStatusTimer.stop();
}

void GrblMachineModel::updateStatus(GrblResponse response)
{
    /*qDebug() << "GrblMachineModel: Updating status from response"
             << response.getData();
             */

    static QRegExp statusRegex("<([^|<]\\w+)\\|");

    if (statusRegex.indexIn(response.getData()) >=0)
    {
        QString stateStr = statusRegex.cap(1);
        //qDebug() << "GrblMachineModel: parsing state from" << stateStr;
        mLastState = mState;
        mState = stateFromString(stateStr);
        if (mState != mLastState)
        {
            emit machineStateUpdatedSignal(mState);
        }
    }
}

void GrblMachineModel::parseConfigurationResponse(GrblResponse response)
{
   static QRegExp configRegex("\\$(\\d+)=(\\S*)");
   int param = -1;
   QString value = "";

   if (configRegex.indexIn(response.getData()) > -1)
   {
      param = configRegex.cap(1).toInt();
      value = configRegex.cap(2);
      qDebug() << "GrblMachineModel: Got configuration parameter"
               << param << "-->" << value;
      mFirmwareConfiguration[param] = value;
      emit firmwareConfigurationReadSignal(param,value);
   }
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


void GrblMachineModel::onGcodeCommandManualSend(GcodeCommand* command)
{
    if (mSerialPort.isOpen())
    {
        //mSerialPort.flush();
        if (command->getRawCommand() > 0)
        {
            qDebug() << "GrblMachineController: Manual Raw Gcode Send 0x"
                     << QString::number(command->getRawCommand(),16).toUpper();
            char c = command->getRawCommand();
            mBytesWaiting += mSerialPort.write(&c,1);
        }
        else
        {
            qDebug() << "GrblMachineController: Manual ASCII Gcode Send" << command->getCommand();
            mBytesWaiting += mSerialPort.write(QString(command->getCommand()+"\r").toLatin1());
        }
        mSerialPort.flush();
    }
}

void GrblMachineModel::onUpdateSpindleSpeed(float speed)
{

}

void GrblMachineModel::onUpdateFeedRate(float rate)
{

}

void GrblMachineModel::queueCommand(GcodeCommand* command)
{
    mCommandQueue.append(command);
}

int GrblMachineModel::bufferLengthInUse()
{
    int length = 0;

    for (GcodeCommand* gc : mCommandBuffer)
    {
        length += gc->getCommandLength()+1;
    }

    //qDebug() << "GrblMachineModel: Buffer in use:" << length;
    return length;
}

bool GrblMachineModel::isSpaceInBuffer(GcodeCommand* cmd)
{
    int bufferLeft = BUFFER_LENGTH_LIMIT - bufferLengthInUse();
    int bufferUsed = BUFFER_LENGTH_LIMIT - bufferLeft;
    emit setBufferProgressSignal(((float)bufferUsed/BUFFER_LENGTH_LIMIT)*100);
    return (bufferLeft) > (cmd->getCommandLength()+1);
}

GcodeCommand GrblMachineModel::feedOverride(GcodeCommand* command, double overridePercent)
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
