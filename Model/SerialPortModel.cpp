/*
 * SerialPortModel.cpp
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
#include "SerialPortModel.h"

SerialPortModel::SerialPortModel(QObject *parent) : QObject(parent)
{
    // Setup serial port
    mSerialPort.setParity(QSerialPort::NoParity);
    mSerialPort.setDataBits(QSerialPort::Data8);
    mSerialPort.setFlowControl(QSerialPort::NoFlowControl);
    mSerialPort.setStopBits(QSerialPort::OneStop);
    connect(&mSerialPort, SIGNAL(readyRead()), this, SLOT(onSerialPortReadyRead()), Qt::QueuedConnection);
    connect(&mSerialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(onSerialPortError(QSerialPort::SerialPortError)));
}

SerialPortModel::~SerialPortModel()
{

}


void SerialPortModel::onSerialPortReadyRead()
{
    /*
    while (mSerialPort.canReadLine()) {
        QString data = mSerialPort.readLine().trimmed();

        // Filter prereset responses
        if (mReseting) {
            qDebug() << "reseting filter:" << data;
            if (!dataIsReset(data)) continue;
            else {
                mReseting = false;
                m_timerStateQuery.setInterval(mSettingsForm->queryStateTime());
            }
        }

        // Status response
        if (data[0] == '<') {
            int status = -1;

            mStatusReceived = true;

            // Update machine coordinates
            QRegExp mpx("MPos:([^,]*),([^,]*),([^,^>]*)");
            if (mpx.indexIn(data) != -1) {
                mUi->txtMPosX->setText(mpx.cap(1));
                mUi->txtMPosY->setText(mpx.cap(2));
                mUi->txtMPosZ->setText(mpx.cap(3));
            }

            // Status
            QRegExp stx("<([^,^>]*)");
            if (stx.indexIn(data) != -1) {
                status = mStatus.indexOf(stx.cap(1));

                // Undetermined status
                if (status == -1) status = 0;

                // Update status
                if (status != mLastGrblStatus) {
                    mUi->txtStatus->setText(mStatusCaptions[status]);
                    mUi->txtStatus->setStyleSheet(QString("background-color: %1; color: %2;")
                                                 .arg(mStatusBackColors[status]).arg(mStatusForeColors[status]));
                }

                // Update controls
                mUi->cmdRestoreOrigin->setEnabled(status == IDLE);
                mUi->cmdSafePosition->setEnabled(status == IDLE);
                mUi->cmdZeroXY->setEnabled(status == IDLE);
                mUi->cmdZeroZ->setEnabled(status == IDLE);
                mUi->chkTestMode->setEnabled(status != RUN && !mProcessingFile);
                mUi->chkTestMode->setChecked(status == CHECK);
                mUi->cmdFilePause->setChecked(status == HOLD || status == QUEUE);
#ifdef WINDOWS
                if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
                    if (m_taskBarProgress) m_taskBarProgress->setPaused(status == HOLD || status == QUEUE);
                }
#endif

                // Update "elapsed time" timer
                if (mProcessingFile) {
                    QTime time(0, 0, 0);
                    int elapsed = m_startTime.elapsed();
                    mUi->glwVisualizer->setSpendTime(time.addMSecs(elapsed));
                }

                // Test for job complete
                if (mProcessingFile && mTransferCompleted &&
                        ((status == IDLE && mLastGrblStatus == RUN) || status == CHECK)) {
                    qDebug() << "job completed:" << mFileCommandIndex << m_currentModel->rowCount() - 1;

                    // Shadow last segment
                    GcodeViewParse *parser = mCurrentDrawer->viewParser();
                    QList<LineSegment*> list = parser->getLineSegmentList();
                    if (mLastDrawnLineIndex < list.count()) {
                        list[mLastDrawnLineIndex]->setDrawn(true);
                        mCurrentDrawer->update(QList<int>() << mLastDrawnLineIndex);
                    }

                    // Update state
                    mProcessingFile = false;
                    mFileProcessedCommandIndex = 0;
                    mLastDrawnLineIndex = 0;
                    m_storedParserStatus.clear();

                    updateControlsState();

                    qApp->beep();

                    m_timerStateQuery.stop();
                    m_timerConnection.stop();

                    QMessageBox::information(this, qApp->applicationDisplayName(), tr("Job done.\nTime elapsed: %1")
                                             .arg(mUi->glwVisualizer->spendTime().toString("hh:mm:ss")));

                    m_timerStateQuery.setInterval(mSettingsForm->queryStateTime());
                    m_timerConnection.start();
                    m_timerStateQuery.start();
                }

                // Store status
                if (status != mLastGrblStatus) mLastGrblStatus = status;

                // Abort
                static double x = sNan;
                static double y = sNan;
                static double z = sNan;

                if (m_aborting) {
                    switch (status) {
                    case IDLE: // Idle
                        if (!mProcessingFile && mResetCompleted) {
                            m_aborting = false;
                            restoreOffsets();
                            restoreParserState();
                            return;
                        }
                        break;
                    case HOLD: // Hold
                    case QUEUE:
                        if (!mReseting && compareCoordinates(x, y, z)) {
                            x = sNan;
                            y = sNan;
                            z = sNan;
                            grblReset();
                        } else {
                            x = mUi->txtMPosX->text().toDouble();
                            y = mUi->txtMPosY->text().toDouble();
                            z = mUi->txtMPosZ->text().toDouble();
                        }
                        break;
                    }
                }
            }

            // Update work coordinates
            QRegExp wpx("WPos:([^,]*),([^,]*),([^,^>]*)");
            if (wpx.indexIn(data) != -1)
            {
                mUi->txtWPosX->setText(wpx.cap(1));
                mUi->txtWPosY->setText(wpx.cap(2));
                mUi->txtWPosZ->setText(wpx.cap(3));
                QVector3D toolPosition;

                // Update tool position
                if (!(status == CHECK && mFileProcessedCommandIndex < m_currentModel->rowCount() - 1)) {
                    toolPosition = QVector3D(toMetric(mUi->txtWPosX->text().toDouble()),
                                             toMetric(mUi->txtWPosY->text().toDouble()),
                                             toMetric(mUi->txtWPosZ->text().toDouble()));
                    m_toolDrawer.setToolPosition(mCodeDrawer->getIgnoreZ() ? QVector3D(toolPosition.x(), toolPosition.y(), 0) : toolPosition);
                }


                // toolpath shadowing
                if (mProcessingFile && status != CHECK) {
                    GcodeViewParse *parser = mCurrentDrawer->viewParser();

                    bool toolOntoolpath = false;

                    QList<int> drawnLines;
                    QList<LineSegment*> list = parser->getLineSegmentList();

                    for (int i = mLastDrawnLineIndex; i < list.count()
                         && list.at(i)->getLineNumber()
                         <= (m_currentModel->data(m_currentModel->index(mFileProcessedCommandIndex, 4)).toInt() + 1); i++) {
                        if (list.at(i)->contains(toolPosition)) {
                            toolOntoolpath = true;
                            mLastDrawnLineIndex = i;
                            break;
                        }
                        drawnLines << i;
                    }

                    if (toolOntoolpath) {
                        foreach (int i, drawnLines) {
                            list.at(i)->setDrawn(true);
                        }
                        if (!drawnLines.isEmpty()) mCurrentDrawer->update(drawnLines);
                    } else if (mLastDrawnLineIndex < list.count()) {
                        qDebug() << "tool missed:" << list.at(mLastDrawnLineIndex)->getLineNumber()
                                 << m_currentModel->data(m_currentModel->index(mFileProcessedCommandIndex, 4)).toInt()
                                 << mFileProcessedCommandIndex;
                    }
                }
            }

        } else if (data.length() > 0) {

            // Processed commands
            if (mCommands.length() > 0 && !dataIsFloating(data)
                    && !(mCommands[0].command != "[CTRL+X]" && dataIsReset(data))) {

                static QString response; // Full response string

                if ((mCommands[0].command != "[CTRL+X]" && dataIsEnd(data))
                        || (mCommands[0].command == "[CTRL+X]" && dataIsReset(data))) {

                    response.append(data);

                    // Take command from buffer
                    CommandAttributes ca = mCommands.takeFirst();
                    QTextBlock tb = mUi->txtConsole->document()->findBlockByNumber(ca.consoleIndex);
                    QTextCursor tc(tb);

                    // Restore absolute/relative coordinate system after jog
                    if (ca.command.toUpper() == "$G" && ca.tableIndex == -2) {
                        if (mUi->chkKeyboardControl->isChecked()) m_absoluteCoordinates = response.contains("G90");
                        else if (response.contains("G90")) sendCommand("G90", -1, mSettingsForm->showUICommands());
                    }

                    // Process parser status
                    if (ca.command.toUpper() == "$G" && ca.tableIndex == -3) {
                        // Update status in visualizer window
                        mUi->glwVisualizer->setParserStatus(response.left(response.indexOf("; ")));

                        // Store parser status
                        if (mProcessingFile) storeParserState();

                        // Process spindle state
                        if (!response.contains("M5")) {
                            m_spindleCW = response.contains("M3");
                            m_timerToolAnimation.start(25, this);
                            mUi->cmdSpindle->setChecked(true);
                        } else {
                            m_timerToolAnimation.stop();
                            mUi->cmdSpindle->setChecked(false);
                        }

                        // Spindle speed
                        QRegExp rx(".*S([\\d\\.]+)");
                        if (rx.indexIn(response) != -1) {
                            double speed = toMetric(rx.cap(1).toDouble()); //RPM in imperial?
                            if (fabs(mUi->txtSpindleSpeed->value() - speed) < 2.54) mUi->txtSpindleSpeed->setStyleSheet("color: palette(text);");
                        }

                        // Feed
                        rx.setPattern(".*F([\\d\\.]+)");
                        if (rx.indexIn(response) != -1) {
                            double feed = toMetric(rx.cap(1).toDouble());
                            double set = mUi->chkFeedOverride->isChecked() ? m_originalFeed / 100 * mUi->txtFeed->value()
                                                                          : m_originalFeed;
                            if (response.contains("G20")) set *= 25.4;
                            if (fabs(feed - set) < 2.54) mUi->txtFeed->setStyleSheet("color: palette(text);");
                        }

                        mUpdateParserStatus = true;
                    }

                    // Store origin
                    if (ca.command == "$#" && ca.tableIndex == -2) {
                        qDebug() << "Received offsets:" << response;
                        QRegExp rx(".*G92:([^,]*),([^,]*),([^\\]]*)");

                        if (rx.indexIn(response) != -1) {
                            if (m_settingZeroXY) {
                                m_settingZeroXY = false;
                                m_storedX = toMetric(rx.cap(1).toDouble());
                                m_storedY = toMetric(rx.cap(2).toDouble());
                            } else if (m_settingZeroZ) {
                                m_settingZeroZ = false;
                                m_storedZ = toMetric(rx.cap(3).toDouble());
                            }
                            mUi->cmdRestoreOrigin->setToolTip(QString(tr("Restore origin:\n%1, %2, %3")).arg(m_storedX).arg(m_storedY).arg(m_storedZ));
                        }
                    }

                    // Homing response
                    if ((ca.command.toUpper() == "$H" || ca.command.toUpper() == "$T") && mHoming) mHoming = false;

                    // Reset complete
                    if (ca.command == "[CTRL+X]") {
                        mResetCompleted = true;
                        mUpdateParserStatus = true;
                    }

                    // Clear command buffer on "M2" & "M30" command (old firmwares)
                    if ((ca.command.contains("M2") || ca.command.contains("M30")) && response.contains("ok") && !response.contains("[Pgm End]")) {
                        mCommands.clear();
                        mQueue.clear();
                    }

                    // Process probing on heightmap mode only from table commands
                    if (ca.command.contains("G38.2") && m_heightMapMode && ca.tableIndex > -1) {
                        // Get probe Z coordinate
                        // "[PRB:0.000,0.000,0.000:0];ok"
                        QRegExp rx(".*PRB:([^,]*),([^,]*),([^]^:]*)");
                        double z = qQNaN();
                        if (rx.indexIn(response) != -1) {
                            qDebug() << "probing coordinates:" << rx.cap(1) << rx.cap(2) << rx.cap(3);
                            z = toMetric(rx.cap(3).toDouble());
                        }

                        static double firstZ;
                        if (mProbeIndex == -1) {
                            firstZ = z;
                            z = 0;
                        } else {
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
                    if (ca.command.contains(QRegExp("$[cC]"))) {
                        m_timerStateQuery.setInterval(response.contains("Enable") ? 1000 : mSettingsForm->queryStateTime());
                    }

                    // Add response to console
                    if (tb.isValid() && tb.text() == ca.command) {

                        bool scrolledDown = mUi->txtConsole->verticalScrollBar()->value() == mUi->txtConsole->verticalScrollBar()->maximum();

                        // Update text block numbers
                        int blocksAdded = response.count("; ");

                        if (blocksAdded > 0) for (int i = 0; i < mCommands.count(); i++) {
                            if (mCommands[i].consoleIndex != -1) mCommands[i].consoleIndex += blocksAdded;
                        }

                        tc.beginEditBlock();
                        tc.movePosition(QTextCursor::EndOfBlock);

                        tc.insertText(" < " + QString(response).replace("; ", "\r\n"));
                        tc.endEditBlock();

                        if (scrolledDown) mUi->txtConsole->verticalScrollBar()->setValue(mUi->txtConsole->verticalScrollBar()->maximum());
                    }

                    // Check queue
                    if (mQueue.length() > 0) {
                        CommandQueue cq = mQueue.takeFirst();
                        while ((bufferLength() + cq.command.length() + 1) <= BUFFERLENGTH) {
                            sendCommand(cq.command, cq.tableIndex, cq.showInConsole);
                            if (mQueue.isEmpty()) break; else cq = m_queue.takeFirst();
                        }
                    }

                    // Add response to table, send next program commands
                    if (mProcessingFile) {

                        // Only if command from table
                        if (ca.tableIndex > -1) {
                            m_currentModel->setData(m_currentModel->index(ca.tableIndex, 2), GCodeItem::Processed);
                            m_currentModel->setData(m_currentModel->index(ca.tableIndex, 3), response);

                            mFileProcessedCommandIndex = ca.tableIndex;

                            if (mUi->chkAutoScroll->isChecked() && ca.tableIndex != -1) {
                                mUi->tblProgram->scrollTo(m_currentModel->index(ca.tableIndex + 1, 0));
                                mUi->tblProgram->setCurrentIndex(m_currentModel->index(ca.tableIndex, 1));
                            }
                        }

                        // Update taskbar progress
#ifdef WINDOWS
                        if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
                            if (m_taskBarProgress) m_taskBarProgress->setValue(mFileProcessedCommandIndex);
                        }
#endif
                        // Process error messages
                        static bool holding = false;
                        static QString errors;

                        if (ca.tableIndex > -1 && response.toUpper().contains("ERROR") && !mSettingsForm->ignoreErrors()) {
                            errors.append(QString::number(ca.tableIndex + 1) + ": " + ca.command
                                          + " < " + response + "\n");

                            m_senderErrorBox->setText(tr("Error message(s) received:\n") + errors);

                            if (!holding) {
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
                                || ca.command.contains(QRegExp("M0*2|M30"))) mTransferCompleted = true;
                        // Send next program commands
                        else if (!mFileEndSent && (mFileCommandIndex < m_currentModel->rowCount()) && !holding) sendNextFileCommands();
                    }

                    // Scroll to first line on "M30" command
                    if (ca.command.contains("M30")) mUi->tblProgram->setCurrentIndex(m_currentModel->index(0, 1));

                    // Toolpath shadowing on check mode
                    if (mStatusCaptions.indexOf(mUi->txtStatus->text()) == CHECK) {
                        GcodeViewParse *parser = mCurrentDrawer->viewParser();
                        QList<LineSegment*> list = parser->getLineSegmentList();

                        if (!mTransferCompleted && mFileProcessedCommandIndex < m_currentModel->rowCount() - 1) {
                            int i;
                            QList<int> drawnLines;

                            for (i = mLastDrawnLineIndex; i < list.count()
                                 && list.at(i)->getLineNumber()
                                 <= (m_currentModel->data(m_currentModel->index(mFileProcessedCommandIndex, 4)).toInt()); i++) {
                                drawnLines << i;
                            }

                            if (!drawnLines.isEmpty() && (i < list.count())) {
                                mLastDrawnLineIndex = i;
                                QVector3D vec = list.at(i)->getEnd();
                                m_toolDrawer.setToolPosition(vec);
                            }

                            foreach (int i, drawnLines) {
                                list.at(i)->setDrawn(true);
                            }
                            if (!drawnLines.isEmpty()) mCurrentDrawer->update(drawnLines);
                        } else {
                            foreach (LineSegment* s, list) {
                                if (!qIsNaN(s->getEnd().length())) {
                                    m_toolDrawer.setToolPosition(s->getEnd());
                                    break;
                                }
                            }
                        }
                    }

                    response.clear();
                } else {
                    response.append(data + "; ");
                }

            } else {
                // Unprocessed responses
                qDebug() << "floating response:" << data;

                // Handle hardware reset
                if (dataIsReset(data)) {
                    qDebug() << "hardware reset";

                    mProcessingFile = false;
                    mTransferCompleted = true;
                    mFileCommandIndex = 0;

                    mReseting = false;
                    mHoming = false;
                    mLastGrblStatus = -1;

                    mUpdateParserStatus = true;
                    mStatusReceived = true;

                    mCommands.clear();
                    mQueue.clear();

                    updateControlsState();
                }
                mUi->txtConsole->appendPlainText(data);
            }
        } else {
            // Blank response
//            mUi->txtConsole->appendPlainText(data);
        }
    }
    */
}

void SerialPortModel::onSerialPortError(QSerialPort::SerialPortError error)
{
    /*
    static QSerialPort::SerialPortError previousError;

    if (error != QSerialPort::NoError && error != previousError) {
        previousError = error;
        mUi->txtConsole->appendPlainText(tr("Serial port error ") + QString::number(error) + ": " + mSerialPort.errorString());
        if (mSerialPort.isOpen()) {
            mSerialPort.close();
            updateControlsState();
        }
    }
    */
}


void SerialPortModel::openPort()
{
    /*
    if (mSerialPort.open(QIODevice::ReadWrite)) {
        mUi->txtStatus->setText(tr("Port opened"));
        mUi->txtStatus->setStyleSheet(QString("background-color: palette(button); color: palette(text);"));
//        updateControlsState();
        grblReset();
    }
    */
}

void SerialPortModel::closePort()
{

}

void SerialPortModel::sendCommand(QString command, int tableIndex, bool showInConsole)
{
    /*
    if (!mSerialPort.isOpen() || !mResetCompleted) return;

    command = command.toUpper();

    // Commands queue
    if ((bufferLength() + command.length() + 1) > BUFFERLENGTH) {
//        qDebug() << "queue:" << command;

        CommandQueue cq;

        cq.command = command;
        cq.tableIndex = tableIndex;
        cq.showInConsole = showInConsole;

        mQueue.append(cq);
        return;
    }

    CommandAttributes ca;

//    if (!(command == "$G" && tableIndex < -1) && !(command == "$#" && tableIndex < -1)
//            && (!mTransferringFile || (mTransferringFile && m_showAllCommands) || tableIndex < 0)) {
    if (showInConsole) {
        mUi->txtConsole->appendPlainText(command);
        ca.consoleIndex = mUi->txtConsole->blockCount() - 1;
    } else {
        ca.consoleIndex = -1;
    }

    ca.command = command;
    ca.length = command.length() + 1;
    ca.tableIndex = tableIndex;

    mCommands.append(ca);

    // Processing spindle speed only from g-code program
    QRegExp s("[Ss]0*(\\d+)");
    if (s.indexIn(command) != -1 && ca.tableIndex > -2) {
        int speed = s.cap(1).toInt();
        if (mUi->txtSpindleSpeed->value() != speed) {
            mUi->txtSpindleSpeed->setValue(speed);
            mUi->sliSpindleSpeed->setValue(speed / 100);
        }
    }

    // Set M2 & M30 commands sent flag
    if (command.contains(QRegExp("M0*2|M30"))) {
        mFileEndSent = true;
    }

    mSerialPort.write((command + "\r").toLatin1());
    */
}

void SerialPortModel::grblReset()
{
    /*
    qDebug() << "grbl reset";

    mSerialPort.write(QByteArray(1, (char)24));
//    mSerialPort.flush();

    mProcessingFile = false;
    mTransferCompleted = true;
    mFileCommandIndex = 0;

    mReseting = true;
    mHoming = false;
    mResetCompleted = false;
    mUpdateSpindleSpeed = true;
    mLastGrblStatus = -1;
    mStatusReceived = true;

    // Drop all remaining commands in buffer
    mCommands.clear();
    mQueue.clear();

    // Prepare reset response catch
    CommandAttributes ca;
    ca.command = "[CTRL+X]";
    if (mSettingsForm->showUICommands()) mUi->txtConsole->appendPlainText(ca.command);
    ca.consoleIndex = mSettingsForm->showUICommands() ? mUi->txtConsole->blockCount() - 1 : -1;
    ca.tableIndex = -1;
    ca.length = ca.command.length() + 1;
    mCommands.append(ca);

    updateControlsState();
    */
}


void SerialPortModel::sendNextFileCommands()
{
    /*
    if (mCommandsQueue.length() > 0) return;

    QString command = feedOverride(mCurrentGCodeTableModel->data(mCurrentGCodeTableModel->index(mFileCommandIndex, 1)).toString());

    while ((bufferLength() + command.length() + 1) <= BUFFERLENGTH
           && mFileCommandIndex < mCurrentGCodeTableModel->rowCount() - 1
           && !(!mCommandsList.isEmpty() && mCommandsList.last().command.contains(QRegExp("M0*2|M30")))) {
        mCurrentGCodeTableModel->setData(mCurrentGCodeTableModel->index(mFileCommandIndex, 2), GCodeItem::Sent);
        sendCommand(command, mFileCommandIndex, mSettingsForm->showProgramCommands());
        mFileCommandIndex++;
        command = feedOverride(mCurrentGCodeTableModel->data(mCurrentGCodeTableModel->index(mFileCommandIndex, 1)).toString());
    }
    */
}

void SerialPortModel::onTimerConnection()
{
    /*
    if (!mSerialPort.isOpen())
    {
        openPort();
    }
    */
}

void SerialPortModel::restoreParserState()
{
    /*
    if (!mStoredParserStatus.isEmpty())
    {
        mSerialPortModel.sendCommand(
            mStoredParserStatus,
            -1,
            mSettingsFormController.showUICommands()
        );
    }
    */
}

void SerialPortModel::storeOffsets()
{
     //   sendCommand("$#", -2, mSettings->showUICommands());
}

void SerialPortModel::restoreOffsets()
{
    /*
    // Still have pre-reset working position
    sendCommand(
        QString("G21G53G90X%1Y%2Z%3")
            .arg(toMetric(mUi.txtMPosX->text().toDouble()))
            .arg(toMetric(mUi.txtMPosY->text().toDouble()))
            .arg(toMetric(mUi.txtMPosZ->text().toDouble())), -1, mSettingsForm->showUICommands());

    sendCommand(
        QString("G21G92X%1Y%2Z%3")
            .arg(toMetric(mUi.txtWPosX->text().toDouble()))
            .arg(toMetric(mUi.txtWPosY->text().toDouble()))
            .arg(toMetric(mUi.txtWPosZ->text().toDouble())), -1, mSettingsForm->showUICommands());
            */
}

bool SerialPortModel::isPortOpen()
{
    return mSerialPort.isOpen();
}

bool SerialPortModel::isStatusReceived()
{
   return mStatusReceived;
}

void SerialPortModel::setStatusReceived(bool rx)
{
    mStatusReceived=rx;
}

void SerialPortModel::write(QByteArray)
{

}

