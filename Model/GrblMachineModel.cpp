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

const int GrblMachineModel::BUFFER_LENGTH_LIMIT = 127;

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
      mCommandQueueInitialSize(0)
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
    static QRegExp machinePositionExpression("MPos:([^,]*),([^,]*),([^,^>]*)");
    if (machinePositionExpression.indexIn(data) != -1)
    {
        mMachinePosition = QVector3D
                (
                    machinePositionExpression.cap(1).toFloat(),
                    machinePositionExpression.cap(2).toFloat(),
                    machinePositionExpression.cap(3).toFloat()
                    );
        emit updateMachinePositionSignal(mMachinePosition);
    }
}

void GrblMachineModel::updateStatus(const GrblResponse& resp)
{
    // Status
    static QRegExp statusExpression("<([^,^>]*)");
    if (statusExpression.indexIn(resp.getData()) != -1)
    {
        mStatusReceived = true;
        QString stxThing = statusExpression.cap(1);
        qDebug() << "GrblMachineModel: STX Thing is " << stxThing;
        mState = GrblMachineState::Unknown ; //MachineStatus::enumIndexOf(stxThing);

        // Update status
        if (mState != mLastState)
        {
            emit statusTextUpdateSignal(stateToString(mState),QColor("White"),QColor("Black"));
        }

        // Update controls
        emit machineStateUpdatedSignal(mState);

        /*
        // Update "elapsed time" timer
        if (mProcessingFile)
        {
            QTime time(0, 0, 0);
            int elapsed = m_startTime.elapsed();
            mUi->glwVisualizer->setSpendTime(time.addMSecs(elapsed));
        }
        */

        // Test for job complete
        if  (mProcessingFile && mTransferCompleted &&
             ((mState == GrblMachineState::Idle && mLastState == GrblMachineState::Run) ||
              mState == GrblMachineState::Check))
        {
            qDebug() << "GrblMachineModel: Job Completed";
            emit jobCompletedSignal();

            // Shadow last segment
            /*
            GcodeViewParse *parser = mCurrentDrawer->viewParser();
            QList<LineSegment*> list = parser->getLineSegmentList();
            if (mLastDrawnLineIndex < list.count())
            {
                list[mLastDrawnLineIndex]->setDrawn(true);
                mCurrentDrawer->update(QList<int>() << mLastDrawnLineIndex);
            }
            */

            // Update state
            mProcessingFile = false;
            //mLastDrawnLineIndex = 0;
            //m_storedParserStatus.clear();

            //updateControlsState();

            //qApp->beep();

            //m_timerStateQuery.stop();
            //m_timerConnection.stop();

            /*
            QMessageBox::information(this, qApp->applicationDisplayName(), tr("Job done.\nTime elapsed: %1")
                                     .arg(mUi->glwVisualizer->spendTime().toString("hh:mm:ss")));

            m_timerStateQuery.setInterval(mSettingsForm->queryStateTime());
            m_timerConnection.start();
            m_timerStateQuery.start();
            */

        }

        // Store status
        if (mState != mLastState)
        {
            mLastState = mState;
        }

        /* TODO - Somethings Funky with coords here
        if (mAborting)
        {
            switch (status)
            {
            case Idle: // Idle
                if (!mProcessingFile && mResetCompleted)
                {
                    mAborting = false;
                    //restoreOffsets();
                    //restoreParserState();
                    return;
                }
                break;
            case Hold: // Hold
            case Queue:
                if (!mResetting && Util::compareCoordinates(mMachinePosition, x, y, z))
                {
                    mMachinePosition.setX(sNan);
                    mMachinePosition.setY(sNan);
                    mMachinePosition.setZ(sNan);
                    grblReset();
                }
                else
                {

                    x = mUi->txtMPosX->text().toDouble();
                    y = mUi->txtMPosY->text().toDouble();
                    z = mUi->txtMPosZ->text().toDouble();
                }
                break;
            }
        }
        */
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

void GrblMachineModel::processResponse(const GrblResponse& response)
{
    qDebug() << "GrblMachineModel: Process Response";

    emit appendResponseToConsoleSignal(response);
    mLastState = mState;
    GcodeCommand *next;

    switch (response.getType())
    {
        case GrblResponseType::Startup:
            emit statusTextUpdateSignal("Found Machine",QColor("Black"),QColor("DarkGreen"));
            break;
        case GrblResponseType::Locked:
            mState = GrblMachineState::Locked;
            emit statusTextUpdateSignal("Locked",QColor("Black"),QColor("Orange"));
            break;
        case GrblResponseType::Unlocked:
            mState = GrblMachineState::Unlocked;
            emit statusTextUpdateSignal("Unlocked",QColor("White"),QColor("Green"));
            break;
        case GrblResponseType::Status:
            updateMachineCoordinates(response);
            updateWorkCoordinates(response);
            break;
        case GrblResponseType::Ok:
            if (!mCommandBuffer.isEmpty())
            {
                next = mCommandBuffer.takeFirst();
                if (next)
                {
                    qDebug() << "GrblMachineModel: Popping"
                             << next->getID() << "/" << next->getLine()
                             << "from the buffer";
                    next->setResponse(response);
                    next->setState(GcodeCommandState::Processed);
                    emit updateProgramTableStatusSignal(next);
                }
                else
                {
                    qDebug() << "GrblMachineModel: Job Completed";
                    emit jobCompletedSignal();
                }
            }
            else {
                qDebug() <<
                            "GrblMachineModel: Command Buffer is Empty";
            }
            mCountProcessedCommands++;
            emit setCompletionProgressSignal((int)getProcessedPercent());
            break;
    };

    if (mState != mLastState)
    {
        emit machineStateUpdatedSignal(mState);
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

    GcodeCommand *command = mCommandQueue.first();

    qDebug() << "GrblMachineModel: Attempting to send queued command" << command->getCommand();

    if (!mSerialPort.isOpen())
    {
        qDebug() << "GrblMachineModel: sendCommand FAILED -> SerialPort is not open";
        return false;
    }

    // Buffer full, append to queue
    if (!isSpaceInBuffer(command))
    {
        qDebug() << "GrblMachineModel: Buffer full, waiting... " << command->getCommand();
        return false;
    }
    // Buffer space available
    else
    {
        // Take the command off the queue for processing
        command = mCommandQueue.takeFirst();// feedOverride(mCommandQueue.takeFirst(),mFeedOverrideRate);

        emit appendCommandToConsoleSignal(command);
        mCommandBuffer.append(command);

        // Look for spindle speed in gcode
        if (command->isSpindleSpeedCommand())
        {
            int speed = command->getSpindleSpeed();
            emit spindleSpeedChangedSignal(speed);
        }

        // Set M2 & M30 commands sent flag
        if (command->isFileEndCommand())
        {
            qDebug() << "GrblMachineModel: Detected end of program "
                     << command->getCommand();
            mFileEndSent = true;
        }

        mSerialPort.write((command->getCommand() + "\r").toLatin1());
        command->setState(GcodeCommandState::Sent);
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
    GcodeCommand* cX = GcodeCommand::ControlXCommand();
    emit appendCommandToConsoleSignal(cX);
    mCommandBuffer.append(cX);
    // Write the C^X
    mSerialPort.write(QByteArray(1, static_cast<char>(24)));
    mSerialPort.flush();
}

void GrblMachineModel::onSendProgram(QSharedPointer<GcodeFileModel> gcodeFile)
{
    qDebug() << "GrblMachineModel: onSendProgram()";

    clearCommandBuffer();
    clearCommandQueue();
    mCountProcessedCommands = 0;

    for (GcodeCommand* next : gcodeFile->getData())
    {
        queueCommand(next);
    }

    mCommandQueueInitialSize = mCommandQueue.count();
}

void GrblMachineModel::onSendProgramFromLine(QSharedPointer<GcodeFileModel> gcodeFile, long id)
{
    qDebug() << "GrblMachineModel: onSendProgramFromLine @ id" << id;

    clearCommandBuffer();
    clearCommandQueue();
    mCountProcessedCommands = 0;

    int index = gcodeFile->getCommandByID(id)->getLine();
    int size = gcodeFile->getData().count();

    for (; index < size; index++)
    {
        queueCommand(gcodeFile->getData()[index]);
    }

    mCommandQueueInitialSize = mCommandQueue.count();
}

void GrblMachineModel::startProgramSendTimer()
{
    mProgramSendTimer.start(mProgramSendInterval);
}

void GrblMachineModel::stopProgramSendTimer()
{
    mProgramSendTimer.stop();
}

void GrblMachineModel::clearCommandQueue()
{
    mCommandQueue.clear();
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

void GrblMachineModel::onGcodeCommandManualSend(GcodeCommand* command)
{
    queueCommand(command);
}

void GrblMachineModel::queueCommand(GcodeCommand* command)
{
    mCommandQueue.append(command);
}

int GrblMachineModel::bufferLengthInUse()
{
    int length = 0;

    for (GcodeCommand *gc : mCommandBuffer)
    {
        length += gc->getCommandLength()+1;
    }

    qDebug() << "GrblMachineModel: Buffer in use:" << length;
    return length;
}

bool GrblMachineModel::isSpaceInBuffer(GcodeCommand* cmd)
{
    int bufferLeft = BUFFER_LENGTH_LIMIT - bufferLengthInUse();
    int bufferUsed = BUFFER_LENGTH_LIMIT - bufferLeft;
    emit setBufferProgressSignal(((float)bufferUsed/BUFFER_LENGTH_LIMIT)*100);
    return (bufferLeft) > (cmd->getCommandLength()+1);
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
