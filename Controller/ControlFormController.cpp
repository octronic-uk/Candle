/*
 * ControlFormController.cpp
 *
 * Created: 22 2018 by Ashley
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
#include "ControlFormController.h"

ControlFormController::ControlFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "Contructing ControlFormController";
    mUi.setupUi(this);
    setupSignalSlots();
}

ControlFormController::~ControlFormController()
{
    qDebug() << "Destructing ControlFormController";

}

void ControlFormController::onCmdHomeClicked()
{
    mIsHoming = true;
    //mIsUpdatingSpindleSpeed = true;
    //sendCommand("$H", -1, mSettingsForm->showUICommands());
}

void ControlFormController::onCmdTouchClicked()
{
    /*
    mIsHoming = true;

    QStringList list = mSettingsForm->touchCommand().split(";");

    foreach (QString cmd, list)
    {
        sendCommand(cmd.trimmed(), -1, mSettingsForm->showUICommands());
    }
    */
}

void ControlFormController::onCmdZeroXYClicked()
{
    /*
    mSettingZeroXY = true;
    sendCommand("G92X0Y0", -1, mSettingsForm->showUICommands());
    sendCommand("$#", -2, mSettingsForm->showUICommands());
    */
}

void ControlFormController::onCmdZeroZClicked()
{
    /*
    mSettingZeroZ = true;
    sendCommand("G92Z0", -1, mSettingsForm->showUICommands());
    sendCommand("$#", -2, mSettingsForm->showUICommands());
    */
}

void ControlFormController::setupSignalSlots()
{

    qDebug() << "ControlFormController: Setup Signals/Slots";
}

void ControlFormController::onCmdRestoreOriginClicked()
{
    /*
    // Restore offset
    sendCommand(QString("G21"), -1, mSettingsForm->showUICommands());
    sendCommand(QString("G53G90G0X%1Y%2Z%3").arg(toMetric(mUi->txtMPosX->text().toDouble()))
                                            .arg(toMetric(mUi->txtMPosY->text().toDouble()))
                                            .arg(toMetric(mUi->txtMPosZ->text().toDouble())), -1, mSettingsForm->showUICommands());
    sendCommand(QString("G92X%1Y%2Z%3").arg(toMetric(mUi->txtMPosX->text().toDouble()) - mStoredX)
                                        .arg(toMetric(mUi->txtMPosY->text().toDouble()) - mStoredY)
                                        .arg(toMetric(mUi->txtMPosZ->text().toDouble()) - mStoredZ), -1, mSettingsForm->showUICommands());

    // Move tool
    if (mSettingsForm->moveOnRestore())
    {
        switch (mSettingsForm->restoreMode())
        {
        case 0:
            sendCommand("G0X0Y0", -1, mSettingsForm->showUICommands());
            break;
        case 1:
            sendCommand("G0X0Y0Z0", -1, mSettingsForm->showUICommands());
            break;
        }
    }
    */
}

void ControlFormController::onCmdResetClicked()
{
    //grblReset();
}

void ControlFormController::onCmdUnlockClicked()
{
    /*
    mUpdateSpindleSpeed = true;
    sendCommand("$X", -1, mSettingsForm->showUICommands());
    */
}

void ControlFormController::onCmdSafePositionClicked()
{
    /*
    QStringList list = mSettingsForm->safePositionCommand().split(";");

    foreach (QString cmd, list)
    {
        sendCommand(cmd.trimmed(), -1, mSettingsForm->showUICommands());
    }
    */
}

void ControlFormController::onChkTestModeClicked(bool checked)
{
    /*
    if (checked)
    {
        storeOffsets();
        storeParserState();
        sendCommand("$C", -1, mSettingsForm->showUICommands());
    }
    else
    {
        mAborting = true;
        grblReset();
    };
    */
}

void ControlFormController::updateControlsState()
{
    /*
    bool portOpened = mSerialPort.isOpen();

    mUi->grpState->setEnabled(portOpened);
    mUi->grpControl->setEnabled(portOpened);
    mUi->widgetUserCommands->setEnabled(portOpened && !mProcessingFile);
    mUi->widgetSpindle->setEnabled(portOpened);
    mUi->widgetJog->setEnabled(portOpened && !mProcessingFile);
//    mUi->grpConsole->setEnabled(portOpened);
    mUi->cboCommand->setEnabled(portOpened && (!mUi->chkKeyboardControl->isChecked()));
    mUi->cmdCommandSend->setEnabled(portOpened);
//    mUi->widgetFeed->setEnabled(!mTransferringFile);

    mUi->chkTestMode->setEnabled(portOpened && !mProcessingFile);
    mUi->cmdHome->setEnabled(!mProcessingFile);
    mUi->cmdTouch->setEnabled(!mProcessingFile);
    mUi->cmdZeroXY->setEnabled(!mProcessingFile);
    mUi->cmdZeroZ->setEnabled(!mProcessingFile);
    mUi->cmdRestoreOrigin->setEnabled(!mProcessingFile);
    mUi->cmdSafePosition->setEnabled(!mProcessingFile);
    mUi->cmdUnlock->setEnabled(!mProcessingFile);
    mUi->cmdSpindle->setEnabled(!mProcessingFile);

    mUi->actFileNew->setEnabled(!mProcessingFile);
    mUi->actFileOpen->setEnabled(!mProcessingFile);
    mUi->cmdFileOpen->setEnabled(!mProcessingFile);
    mUi->cmdFileReset->setEnabled(!mProcessingFile && mProgramModel.rowCount() > 1);
    mUi->cmdFileSend->setEnabled(portOpened && !mProcessingFile && mProgramModel.rowCount() > 1);
    mUi->cmdFilePause->setEnabled(mProcessingFile && !mUi->chkTestMode->isChecked());
    mUi->cmdFileAbort->setEnabled(mProcessingFile);
    mUi->actFileOpen->setEnabled(!mProcessingFile);
    mUi->mnuRecent->setEnabled(!mProcessingFile && ((mRecentFiles.count() > 0 && !mHeightMapMode)
                                                      || (mRecentHeightmaps.count() > 0 && mHeightMapMode)));
    mUi->actFileSave->setEnabled(mProgramModel.rowCount() > 1);
    mUi->actFileSaveAs->setEnabled(mProgramModel.rowCount() > 1);

    mUi->tblProgram->setEditTriggers(mProcessingFile ? QAbstractItemView::NoEditTriggers :
                                                         QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked
                                                         | QAbstractItemView::EditKeyPressed | QAbstractItemView::AnyKeyPressed);

    if (!portOpened)
    {
        mUi->txtStatus->setText(tr("Not connected"));
        mUi->txtStatus->setStyleSheet(QString("background-color: palette(button); color: palette(text);"));
    }

    this->setWindowTitle(mProgramFileName.isEmpty() ? qApp->applicationDisplayName()
                                                     : mProgramFileName.mid(mProgramFileName.lastIndexOf("/") + 1) + " - " + qApp->applicationDisplayName());

    if (!mProcessingFile)
    {
        mUi->chkKeyboardControl->setChecked(mStoredKeyboardControl);
    }

#ifdef WINDOWS
    if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
        if (!mProcessingFile && mTaskBarProgress) mTaskBarProgress->hide();
    }
#endif

    style()->unpolish(mUi->cmdFileOpen);
    style()->unpolish(mUi->cmdFileReset);
    style()->unpolish(mUi->cmdFileSend);
    style()->unpolish(mUi->cmdFilePause);
    style()->unpolish(mUi->cmdFileAbort);
    mUi->cmdFileOpen->ensurePolished();
    mUi->cmdFileReset->ensurePolished();
    mUi->cmdFileSend->ensurePolished();
    mUi->cmdFilePause->ensurePolished();
    mUi->cmdFileAbort->ensurePolished();

    // Heightmap
    mHeightMapBorderDrawer.setVisible(mUi->chkHeightMapBorderShow->isChecked() && mHeightMapMode);
    mHeightMapGridDrawer.setVisible(mUi->chkHeightMapGridShow->isChecked() && mHeightMapMode);
    mHeightMapInterpolationDrawer.setVisible(mUi->chkHeightMapInterpolationShow->isChecked() && mHeightMapMode);

    mUi->grpProgram->setTitle(mHeightMapMode ? tr("Heightmap") : tr("G-code program"));
    mUi->grpProgram->setProperty("overrided", mHeightMapMode);
    style()->unpolish(mUi->grpProgram);
    mUi->grpProgram->ensurePolished();

    mUi->grpHeightMapSettings->setVisible(mHeightMapMode);
    mUi->grpHeightMapSettings->setEnabled(!mProcessingFile && !mUi->chkKeyboardControl->isChecked());

    mUi->chkTestMode->setVisible(!mHeightMapMode);
    mUi->chkAutoScroll->setVisible(mUi->splitter->sizes()[1] && !mHeightMapMode);

    mUi->tblHeightMap->setVisible(mHeightMapMode);
    mUi->tblProgram->setVisible(!mHeightMapMode);

    mUi->widgetHeightMap->setEnabled(!mProcessingFile && mProgramModel.rowCount() > 1);
    mUi->cmdHeightMapMode->setEnabled(!mUi->txtHeightMap->text().isEmpty());

    mUi->cmdFileSend->setText(mHeightMapMode ? tr("Probe") : tr("Send"));

    mUi->chkHeightMapUse->setEnabled(!mHeightMapMode && !mUi->txtHeightMap->text().isEmpty());

    mUi->actFileSaveTransformedAs->setVisible(mUi->chkHeightMapUse->isChecked());

    mUi->cmdFileSend->menu()->actions().first()->setEnabled(!mUi->cmdHeightMapMode->isChecked());

    mSelectionDrawer.setVisible(!mUi->cmdHeightMapMode->isChecked());
    */
}

bool ControlFormController::isHoming()
{
   return false;
}

bool ControlFormController::isResetCompleted()
{
   return false;
}
