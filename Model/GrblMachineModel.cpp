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
#include "Utils/MachineState.h"
#include "Utils/Util.h"
#include "Utils/IndexOutOfBoundsException.h"
#include <QMetaType>

const int GrblMachineModel::BUFFER_LENGTH = 128;

GrblMachineModel::GrblMachineModel(QObject *parent)
    : QObject(parent),
      mLastGrblStatus(Unknown),
      mMachinePosition(QVector3D(0.0,0.0,0.0)),
      mWorkPosition(QVector3D(0.0,0.0,0.0)),
      mHoming(false),
      mPortNameSet(false),
      mBaudRateSet(false),
      mResetting(false),
      mFeedOverrideRate(100.0), // 100% of feed
      mCommandIndex(0),
      mProcessingFile(false),
      mTransferCompleted(false),
      mAborting(false)
{
    setupSerialPort();
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

void GrblMachineModel::updateMachineCoordinates(GrblResponse response)
{
    // Update machine coordinates
    QString data = response.getData();
    static QRegExp machinePositionExpression("MPos:([^,]*),([^,]*),([^,^>]*)");
    if (machinePositionExpression.indexIn(data) != -1)
    {
        mMachinePosition = QVector3D(
                    machinePositionExpression.cap(1).toFloat(),
                    machinePositionExpression.cap(2).toFloat(),
                    machinePositionExpression.cap(3).toFloat()
                    );
        emit updateMachinePositionSignal(mMachinePosition);
    }
}

void GrblMachineModel::updateStatus(GrblResponse resp)
{
    MachineStateEnum status = Unknown;

    // Status
    static QRegExp statusExpression("<([^,^>]*)");
    if (statusExpression.indexIn(resp.getData()) != -1)
    {

        mStatusReceived = true;
        QString stxThing = statusExpression.cap(1);
        qDebug() << "GrblMachineModel: STX Thing is " << stxThing;
        status = Unknown ; //MachineStatus::enumIndexOf(stxThing);

        // Update status
        if (status != mLastGrblStatus)
        {
            emit statusTextUpdateSignal(MachineState::enumToQString(status),QColor("White"),QColor("Black"));
        }

        // Update controls
        emit setInterfaceLockSignal(status == MachineStateEnum::Idle);

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
        if  (
             mProcessingFile &&
             mTransferCompleted &&
             ((status == Idle && mLastGrblStatus == Run) || status == Check)
             ){
            qDebug() << "GrblMachineModel: Job Completed @" << mCommandIndex ;// << m_currentModel->rowCount() - 1;

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
            mCommandIndex = 0;
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

            emit jobCompletedSignal();
        }

        // Store status
        if (status != mLastGrblStatus)
        {
            mLastGrblStatus = status;
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

void GrblMachineModel::updateWorkCoordinates(GrblResponse resp)
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

void GrblMachineModel::updateToolCoordinates(GrblResponse response)
{
    QVector3D toolPosition;

    // Update tool position
    if (!(mLastGrblStatus == Check && mCommandIndex < mCommandsHistoryList.count()))
    {
        toolPosition = QVector3D(mWorkPosition);
        emit toolPositionChangedSignal(toolPosition);
        //m_toolDrawer.setToolPosition(mCodeDrawer->getIgnoreZ() ? QVector3D(toolPosition.x(), toolPosition.y(), 0) : toolPosition);
    }

    // toolpath shadowing
    if (mProcessingFile && mLastGrblStatus != Check)
    {
        /*
        GcodeViewParse *parser = mCurrentDrawer->viewParser();

        bool toolOntoolpath = false;

        QList<int> drawnLines;
        QList<LineSegment*> list = parser->getLineSegmentList();

        for (
             int i = mLastDrawnLineIndex;
             i < list.count() && list.at(i)->getLineNumber() <= (m_currentModel->data(m_currentModel->index(mFileProcessedCommandIndex, 4)).toInt() + 1);
             i++
             )
        {
            if (list.at(i)->contains(toolPosition))
            {
                toolOntoolpath = true;
                mLastDrawnLineIndex = i;
                break;
            }
            drawnLines << i;
        }

        if (toolOntoolpath)
        {
            foreach (int i, drawnLines)
            {
                list.at(i)->setDrawn(true);
            }
            if (!drawnLines.isEmpty()) mCurrentDrawer->update(drawnLines);
        }
        else if (mLastDrawnLineIndex < list.count())
        {
            qDebug() << "tool missed:" << list.at(mLastDrawnLineIndex)->getLineNumber()
                     << m_currentModel->data(m_currentModel->index(mFileProcessedCommandIndex, 4)).toInt()
                     << mFileProcessedCommandIndex;
        }
        */
    }
}

void GrblMachineModel::processResponse(GrblResponse response)
{
    qDebug() << "GrblMachineModel: Process Response";

    emit appendResponseToConsoleSignal(response.getData());

    switch (response.getType())
    {
        case GrblResponseType::Startup:
            mResetting = false;
            emit statusTextUpdateSignal("Found Machine",QColor("Black"),QColor("DarkGreen"));
            break;
        case GrblResponseType::Locked:
            mLocked = true;
            emit statusTextUpdateSignal("Locked",QColor("Black"),QColor("Orange"));
            break;
        case GrblResponseType::Unlocked:
            mLocked = false;
            emit statusTextUpdateSignal("Unlocked",QColor("Black"),QColor("Green"));
            break;
    };
    /*
    updateMachineCoordinates(response);
    updateStatus(response);
    updateWorkCoordinates(response);
    updateToolCoordinates();
    * /
}

void GrblMachineModel::onSerialResponseProcessed(GcodeCommand response)
{
    /*
    qDebug() << "GrblMachineModel: ProcessResponse 1";

    if  (
            (!mCommandsHistoryList[0].isCtrlXResponse() && response.isEndResponse()) ||
            (mCommandsHistoryList[0].isCtrlXResponse() && response.isResetResponse())
        )
        {

            // Take command from buffer
            GcodeCommand ca = mCommandsHistoryList.takeFirst();

            // Restore absolute/relative coordinate system after jog
            if (ca.isParserStateResponse())
            {
                mAbsoluteCoordinates = response.isAbsoluteCoordinatesResponse();
                sendCommand(GcodeCommand::AbsoluteCoordinatesCommand());
            }

            // Process parser status
            if (ca.isParserStatusResponse())
            {
                // Update status in visualizer window
                //mUi->glwVisualizer->setParserStatus(response.left(response.indexOf("; ")));

                // Store parser status
                if (mProcessingFile)
                {
                    //storeParserState();
                }

                // Process spindle state
                if (!response.isSpindleStopCommand("M5"))
                {
                    mSpindleCW = response.isSpindleClockwiseCommand();
                    mTimerToolAnimation.start(25, this);
                    //mUi->cmdSpindle->setChecked(true);
                }
                else
                {
                    m_timerToolAnimation.stop();
                    mUi->cmdSpindle->setChecked(false);
                }

                // Spindle speed
                QRegExp rx(".*S([\\d\\.]+)");
                if (rx.indexIn(response) != -1)
                {
                    double speed = toMetric(rx.cap(1).toDouble()); //RPM in imperial?
                    if (fabs(mUi->txtSpindleSpeed->value() - speed) < 2.54)
                        mUi->txtSpindleSpeed->setStyleSheet("color: palette(text);");
                }

                // Feed
                rx.setPattern(".*F([\\d\\.]+)");
                if (rx.indexIn(response) != -1)
                {
                    double feed = toMetric(rx.cap(1).toDouble());
                    double set = mUi->chkFeedOverride->isChecked() ? m_originalFeed / 100 * mUi->txtFeed->value()
                                                                  : m_originalFeed;
                    if (response.contains("G20"))
                        set *= 25.4;
                    if (fabs(feed - set) < 2.54)
                        mUi->txtFeed->setStyleSheet("color: palette(text);");
                }

                mUpdateParserStatus = true;
            }

            // Store origin
            if (ca.getCommand() == "$#" && ca.tableIndex == -2)
            {
                qDebug() << "Received offsets:" << response;
                QRegExp rx(".*G92:([^,]*),([^,]*),([^\\]]*)");

                if (rx.indexIn(response) != -1)
                {
                    if (m_settingZeroXY)
                    {
                        m_settingZeroXY = false;
                        m_storedX = toMetric(rx.cap(1).toDouble());
                        m_storedY = toMetric(rx.cap(2).toDouble());
                    }
                    else if (m_settingZeroZ)
                    {
                        m_settingZeroZ = false;
                        m_storedZ = toMetric(rx.cap(3).toDouble());
                    }
                    mUi->cmdRestoreOrigin->setToolTip(QString(tr("Restore origin:\n%1, %2, %3")).arg(m_storedX).arg(m_storedY).arg(m_storedZ));
                }
            }

            // Homing response
            if ((ca.getCommand().toUpper() == "$H" || ca.getCommand().toUpper() == "$T") && mHoming)
                mHoming = false;

            // Reset complete
            if (ca.getCommand() == "[CTRL+X]")
            {
                mResetCompleted = true;
                mUpdateParserStatus = true;
            }

            // Clear command buffer on "M2" & "M30" command (old firmwares)
            if ((ca.getCommand().contains("M2") || ca.getCommand().contains("M30")) && response.contains("ok") && !response.contains("[Pgm End]"))
            {
                mCommands.clear();
                mQueue.clear();
            }

            // Process probing on heightmap mode only from table commands
            if (ca.getCommand().contains("G38.2") && m_heightMapMode && ca.tableIndex > -1)
            {
                // Get probe Z coordinate
                // "[PRB:0.000,0.000,0.000:0];ok"
                QRegExp rx(".*PRB:([^,]*),([^,]*),([^]^:]*)");
                double z = qQNaN();
                if (rx.indexIn(response) != -1)
                {
                    qDebug() << "probing coordinates:" << rx.cap(1) << rx.cap(2) << rx.cap(3);
                    z = toMetric(rx.cap(3).toDouble());
                }

                static double firstZ;
                if (mProbeIndex == -1)
                {
                    firstZ = z;
                    z = 0;
                }
                else
                {
                    // Calculate delta Z
                    z -= firstZ;

                    // Calculate table indexes
                    int row = trunc(mProbeIndex / m_heightMapModel.columnCount());
                    int column = mProbeIndex - row * m_heightMapModel.columnCount();
                    if (row % 2) column = m_heightMapModel.columnCount() - 1 - column;

                    // Store Z in table
                    m_heightMapModel.setData(m_heightMapModel.index(row, column), z, Qt::UserRole);
                    mUi->tblHeightMap->update(m_heightMapModel.index(m_heightMapModel.rowCount() - 1 - row, column));
                    updateHeightMapInterpolationDrawer();
                }

                mProbeIndex++;
            }

            // Change state query time on check mode on
            if (ca.getCommand().contains(QRegExp("$[cC]")))
            {
                m_timerStateQuery.setInterval(response.contains("Enable") ? 1000 : mSettingsForm->queryStateTime());
            }

            // Add response to console
            if (tb.isValid() && tb.text() == ca.getCommand())
            {

                bool scrolledDown = mUi->txtConsole->verticalScrollBar()->value() == mUi->txtConsole->verticalScrollBar()->maximum();

                // Update text block numbers
                int blocksAdded = response.count("; ");

                if (blocksAdded > 0) for (int i = 0; i < mCommands.count(); i++)
                {
                    if (mCommands[i].consoleIndex != -1) mCommands[i].consoleIndex += blocksAdded;
                }

                tc.beginEditBlock();
                tc.movePosition(QTextCursor::EndOfBlock);

                tc.insertText(" < " + QString(response).replace("; ", "\r\n"));
                tc.endEditBlock();

                if (scrolledDown) mUi->txtConsole->verticalScrollBar()->setValue(mUi->txtConsole->verticalScrollBar()->maximum());
            }

            // Check queue
            if (mQueue.length() > 0)
            {
                CommandQueue cq = mQueue.takeFirst();
                while ((bufferLength() + cq.getCommand().length() + 1) <= BUFFERLENGTH)
                {
                    sendCommand(cq.getCommand(), cq.tableIndex, cq.showInConsole);
                    if (mQueue.isEmpty())
                        break;
                    else
                        cq = m_queue.takeFirst();
                }
            }

            // Add response to table, send next program commands
            if (mProcessingFile)
            {
                // Only if command from table
                if (ca.tableIndex > -1)
                {
                    m_currentModel->setData(m_currentModel->index(ca.tableIndex, 2), GcodeCommand::Processed);
                    m_currentModel->setData(m_currentModel->index(ca.tableIndex, 3), response);

                    mFileProcessedCommandIndex = ca.tableIndex;

                    if (mUi->chkAutoScroll->isChecked() && ca.tableIndex != -1)
                    {
                        mUi->tblProgram->scrollTo(m_currentModel->index(ca.tableIndex + 1, 0));
                        mUi->tblProgram->setCurrentIndex(m_currentModel->index(ca.tableIndex, 1));
                    }
                }

                // Process error messages
                static bool holding = false;
                static QString errors;

                if (ca.tableIndex > -1 && response.toUpper().contains("ERROR") && !mSettingsForm->ignoreErrors())
                {
                    errors.append(QString::number(ca.tableIndex + 1) + ": " + ca.getCommand()

                    m_senderErrorBox->setText(tr("Error message(s) received:\n") + errors);

                    if (!holding)
                    {
                        holding = true;         // Hold transmit while messagebox is visible
                        response.clear();

                        mSerialPort.write("!");
                        m_senderErrorBox->checkBox()->setChecked(false);
                        qApp->beep();
                        int result = m_senderErrorBox->exec();

                        holding = false;
                        errors.clear();
                        if (m_senderErrorBox->checkBox()->isChecked()) mSettingsForm->setIgnoreErrors(true);
                        if (result == QMessageBox::Ignore) mSerialPort.write("~"); else on_cmdFileAbort_clicked();
                    }
                }

                // Check transfer complete (last row always blank, last command row = rowcount - 2)
                if (mFileProcessedCommandIndex == m_currentModel->rowCount() - 2
                        || ca.getCommand().contains(QRegExp("M0*2|M30")))
                    mTransferCompleted = true;
                // Send next program commands
                else if (!mFileEndSent && (mFileCommandIndex < m_currentModel->rowCount()) && !holding)
                    sendNextFileCommands();
            }

            // Scroll to first line on "M30" command
            if (ca.getCommand().contains("M30"))
                mUi->tblProgram->setCurrentIndex(m_currentModel->index(0, 1));

            // Toolpath shadowing on check mode
            if (mStatusCaptions.indexOf(mUi->txtStatus->text()) == CHECK)
            {
                GcodeViewParse *parser = mCurrentDrawer->viewParser();
                QList<LineSegment*> list = parser->getLineSegmentList();

                if (!mTransferCompleted && mFileProcessedCommandIndex < m_currentModel->rowCount() - 1)
                {
                    int i;
                    QList<int> drawnLines;

                    for (i = mLastDrawnLineIndex; i < list.count()
                         && list.at(i)->getLineNumber()
                         <= (m_currentModel->data(m_currentModel->index(mFileProcessedCommandIndex, 4)).toInt()); i++)
                    {
                        drawnLines << i;
                    }

                    if (!drawnLines.isEmpty() && (i < list.count()))
                    {
                        mLastDrawnLineIndex = i;
                        QVector3D vec = list.at(i)->getEnd();
                        m_toolDrawer.setToolPosition(vec);
                    }

                    foreach (int i, drawnLines)
                    {
                        list.at(i)->setDrawn(true);
                    }
                    if (!drawnLines.isEmpty())
                        mCurrentDrawer->update(drawnLines);
                }
                else
                {
                    foreach (LineSegment* s, list)
                    {
                        if (!qIsNaN(s->getEnd().length()))
                        {
                            m_toolDrawer.setToolPosition(s->getEnd());
                            break;
                        }
                    }
                }
            }

            response.clear();
        }
        else
        {
            response.append(data + "; ");
        }
    }
    */
}

void GrblMachineModel::onSerialResponseUnprocessed(QString data)
{
    /*
    // Unprocessed responses
    qDebug() << "GrblMachineModel: onSerialResponseUnprocessed" << data;
    // Handle hardware reset
    if (dataIsReset(data))
    {
        qDebug() << "GrblMachineModel: Hardware Reset";

        mProcessingFile = false;
        mTransferCompleted = true;
        mCommandIndex = 0;

        mResetting = false;
        mHoming = false;
        mLastGrblStatus = Unknown;

        //mUpdateParserStatus = true;
        mStatusReceived = true;

        mCommandsHistoryList.clear();
        mCommandsQueue.clear();

        //updateControlsState();
    }
    emit appendToConsoleSignal(data);
    */
}

/*
void GrblMachineModel::processResponse(GrblResponse resp)
{
    /*
    qDebug() << "GrblMachineModel: processSerialResponse" << data;
    SerialResponseType responseType = SerialResponseType::Unknown;
    // Status response
    if (data[0] == '<')
    {
        responseType = SerialResponseType::Status;
    }
    else if (data.length() > 0)
    {
        // Processed commands
        if  (
                mCommandsHistoryList.length() > 0 &&                       // Commands in list
                !dataIsFloating(data) &&                            // Data is not floating
                !(
                    mCommandsHistoryList[0].getCommand() != "[CTRL+X]" &&  // Next command is not c^X
                    dataIsReset(data)                               // Data is reset
                 )
            )
        {
            responseType = SerialResponseType::Processed;
        }
        else
        {
            responseType = SerialResponseType::Unprocessed;
        }
    }
}
    */

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

bool GrblMachineModel::openPort()
{
    if (mSerialPort.open(QIODevice::ReadWrite))
    {
        emit statusBarUpdateSignal(QString("Connected on Serial Port %1 @ %2 baud").arg(getPortName()).arg(getBaudRate()));
        emit statusTextUpdateSignal(QString("Port Open"),QColor("Green"));
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

bool GrblMachineModel::sendCommand(GcodeCommand command)
{
    qDebug() << "GrblMachineModel: sendCommand" << command.getCommand() ;

    if (!mSerialPort.isOpen())
    {
        qDebug() << "GrblMachineModel: sendCommand FAILED -> SerialPort is not open";
        return false;
    }

    if (mResetting)
    {
        qDebug() << "GrblMachineModel: sendCommand FAILED -> Reset still in progress";
        return false;
    }

    // Buffer full, append to queue
    if ((bufferLength() + command.getLength() + 1) > BUFFER_LENGTH)
    {
        qDebug() << "GrblMachineModel: Buffer full, Queue up:" << command.getCommand();
        mCommandsQueue.append(command);
    }
    // Buffer space available
    else
    {
        emit appendCommandToConsoleSignal(command);

        command.setLength(command.getLength() + 1);
        mCommandsHistoryList.append(command);

        // Processing spindle speed only from g-code program

        // Look for spindle speed in gcode
        if (command.isSpindleSpeedCommand())
        {
            int speed = command.getSpindleSpeed();
            emit spindleSpeedChangedSignal(speed);
        }

        // Set M2 & M30 commands sent flag
        if (command.isFileEndCommand())
        {
            qDebug() << "GrblMachineModel: Detected end of program "
                     << command.getCommand();
            mFileEndSent = true;
        }

        mSerialPort.write((command.getCommand() + "\r").toLatin1());
        return true;
    }
    return false;
}

void GrblMachineModel::grblReset()
{
    qDebug() << "GrblMachineModel: Grbl Reset";
    mProcessingFile = false;
    mCommandIndex = 0;
    mResetting = true;
    mHoming = true;
    mStatusReceived = false;
    // Drop all remaining commands in buffer
    mCommandsHistoryList.clear();
    mCommandsQueue.clear();
    // Prepare reset response catch
    GcodeCommand cX = GcodeCommand::ControlXCommand();
    cX.setLength(cX.getCommand().length() + 1);
    emit appendCommandToConsoleSignal(cX.getCommand());
    mCommandsHistoryList.append(cX);
    // Write the C^X
    mSerialPort.write(QByteArray(1, static_cast<char>(24)));
    mSerialPort.flush();
}

QString GrblMachineModel::getNextCommand(GcodeFileModel &gcodeFile)
{
    try
    {
        QString command =
                feedOverride(gcodeFile.getCommand(mCommandIndex),mFeedOverrideRate);
        return command;
    }
    catch (IndexOutOfBoundsException ex)
    {
        qDebug() << "GrblMachineModel: Unable to get command at index"
                 << mCommandIndex
                 << "Out of bounds!!";
    }
    return QString();
}

void GrblMachineModel::onSendNextFileCommands(GcodeFileModel &gcodeFile)
{
    qDebug() << "GrblMachineModel: SendNextFileCommands()";

    if (mCommandsQueue.length() > 0)
    {
        return;
    }

    //QString command = feedOverride(mCurrentGCodeTableModel->data(mCurrentGCodeTableModel->index(mFileCommandIndex, 1)).toString());
    // Get the command at index mFileCommandIndex, (column 1 from table)
    // and scale it to the overridden (or not) feed rate

    QString command = getNextCommand(gcodeFile);

    if (command.isEmpty())
    {
        return;
    }

    while (
           (bufferLength() + command.length() + 1) <= BUFFER_LENGTH &&
           mCommandIndex < gcodeFile.countCommands() - 1 &&
           !(!mCommandsHistoryList.isEmpty() && mCommandsHistoryList.last().getCommand().contains(QRegExp("M0*2|M30")))
           )
    {
        sendCommand(command);
        mCommandIndex++;
        emit updateProgramTableStatusSignal(command);
        command = getNextCommand(gcodeFile);
    }
}

void GrblMachineModel::clearCommandsQueue()
{
    mCommandsQueue.clear();
}

void GrblMachineModel::clearCommandsList()
{
    mCommandsHistoryList.clear();
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
    return mCommandsQueue.count();
}

QString GrblMachineModel::getPortName()
{
    return mSerialPort.portName();
}

void GrblMachineModel::setPortName(QString name)
{
    mSerialPort.setPortName(name);
    mPortNameSet = true;
}

void GrblMachineModel::setBaudRate(int baud)
{
    mSerialPort.setBaudRate(baud);
    mBaudRateSet = true;
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

void GrblMachineModel::onGcodeCommandSend(GcodeCommand command)
{
    sendCommand(command);
}

int GrblMachineModel::bufferLength()
{
    int length = 0;

    foreach (GcodeCommand ca, mCommandsHistoryList)
    {
        length += ca.getLength();
    }

    return length;
}

QString GrblMachineModel::feedOverride(GcodeCommand command, double overridePercent)
{
    QString command_ovr = GcodeParser::overrideSpeed(command.getCommand(),overridePercent);
    return command_ovr;
}

void GrblMachineModel::resetCommandIndex()
{
    setCommandIndex(0);
}

void GrblMachineModel::setCommandIndex(int index)
{
    mCommandIndex = index;
}

int GrblMachineModel::getCommandIndex()
{
    return mCommandIndex;
}


