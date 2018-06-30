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
#include "Model/Settings/Sql/SqlSettingsModel.h"
#include <QMessageBox>

ControlFormController::ControlFormController(QWidget *parent)
    : AbstractFormController(parent),
      mSettingsModelHandle(nullptr),
      mSafePositionSet(false),
      mZOriginSet(false),
      mXYOriginSet(false)
{
    qDebug() << "Contructing ControlFormController";
    mUi.setupUi(this);
    setupSignalSlots();
}

ControlFormController::~ControlFormController()
{
    qDebug() << "Destructing ControlFormController";

}

void ControlFormController::onHomeButtonClicked()
{
   emit gcodeCommandManualSendSignal(GcodeCommand::HomingCommand());
}

MachineSettings* ControlFormController::getMachineSettingsHandle()
{
    if (isModelValid())
    {
        auto profile = mSettingsModelHandle->getCurrentProfileHandle();
        if (profile)
        {
            return profile->getMachineSettingsHandle();
        }
    }
    qDebug() << "ConnectionFormController: Settings model is invalid";
    return nullptr;
}

void ControlFormController::onProbeButtonClicked()
{
    auto machine = getMachineSettingsHandle();
    QStringList list = machine->getProbeCmds().split(";");

    foreach (QString cmd, list)
    {
        // TODO - Leak Spin
        GcodeCommand *next = new GcodeCommand(cmd);
        emit gcodeCommandManualSendSignal(next);
    }
}

void ControlFormController::setClearAlarmResetActive(bool active)
{
    mUi.resetButton->setEnabled(active);
    mUi.clearAlarmButton->setEnabled(active);
}
void ControlFormController::onZeroXYButtonClicked()
{
    if (mXYOriginSet)
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("Reset XY Origin"));
        msg.setText("XY Origin has all ready been set.\n\nAre you sure you want to overwrite it?");
        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        auto result = msg.exec();

        if (result != QMessageBox::Ok)
        {
            return;
        }
    }

    emit gcodeCommandManualSendSignal(GcodeCommand::ZeroXYCommand());
    mXYOriginSet = true;
}

void ControlFormController::onZeroZButtonClicked()
{
    if (mZOriginSet)
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("Reset Z Origin"));
        msg.setText("Z Origin has all ready been set.\n\nAre you sure you want to overwrite it?");
        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        auto result = msg.exec();

        if (result != QMessageBox::Ok)
        {
            return;
        }
    }
    emit gcodeCommandManualSendSignal(GcodeCommand::ZeroZCommand());
    mZOriginSet = true;
}

void ControlFormController::onUserCommand1ButtonClicked()
{
    auto settings = getMachineSettingsHandle();
    QString command = settings->getUserCmd1();
    // TODO - Leak Spin
    emit gcodeCommandManualSendSignal(new GcodeCommand(command));
}

void ControlFormController::onUserCommand2ButtonClicked()
{
    auto settings = getMachineSettingsHandle();
    QString command = settings->getUserCmd2();
    // TODO - Leak Spin
    emit gcodeCommandManualSendSignal(new GcodeCommand(command));
}

void ControlFormController::onUserCommand3ButtonClicked()
{
    auto settings = getMachineSettingsHandle();
    QString command = settings->getUserCmd3();
    // TODO - Leak Spin
    emit gcodeCommandManualSendSignal(new GcodeCommand(command));
}

void ControlFormController::onUserCommand4ButtonClicked()
{
    auto settings = getMachineSettingsHandle();
    QString command = settings->getUserCmd4();
    // TODO - Leak Spin
    emit gcodeCommandManualSendSignal(new GcodeCommand(command));
}

void ControlFormController::onUnlockUiButtonClicked()
{
    emit uiLockOverrideSignal();
}

void ControlFormController::setupSignalSlots()
{

    qDebug() << "ControlFormController: Setup Signals/Slots";
    connect(mUi.homeButton,SIGNAL(clicked()),this,SLOT(onHomeButtonClicked()));
    connect(mUi.probeZButton,SIGNAL(clicked()),this,SLOT(onProbeButtonClicked()));
    connect(mUi.zeroXYButton,SIGNAL(clicked()),this,SLOT(onZeroXYButtonClicked()));
    connect(mUi.zeroZButton,SIGNAL(clicked()),this,SLOT(onZeroZButtonClicked()));

    connect(mUi.goToSafePositionButton,SIGNAL(clicked()),this,SLOT(onGoToSafePositionButtonClicked()));
    connect(mUi.setSafePositionButton,SIGNAL(clicked()),this,SLOT(onSetSafePositionButtonClicked()));
    connect(mUi.clearAlarmButton, SIGNAL(clicked()),this,SLOT(onClearAlarmButtonClicked()));
    connect(mUi.resetButton, SIGNAL(clicked()),this,SLOT(onResetButtonClicked()));

    connect(mUi.user1Button,SIGNAL(clicked()),this,SLOT(onUserCommand1ButtonClicked()));
    connect(mUi.user2Button,SIGNAL(clicked()),this,SLOT(onUserCommand2ButtonClicked()));
    connect(mUi.user3Button,SIGNAL(clicked()),this,SLOT(onUserCommand3ButtonClicked()));
    connect(mUi.user4Button,SIGNAL(clicked()),this,SLOT(onUserCommand4ButtonClicked()));

    connect(mUi.unlockUiButton,SIGNAL(clicked()),this,SLOT(onUnlockUiButtonClicked()));
}

void ControlFormController::setFormActive(bool active)
{
    mUi.homeButton->setEnabled(active);
    mUi.probeZButton->setEnabled(active);
    mUi.zeroXYButton->setEnabled(active);
    mUi.zeroZButton->setEnabled(active);
    mUi.goToSafePositionButton->setEnabled(active && mSafePositionSet);
    mUi.setSafePositionButton->setEnabled(active);
    mUi.resetButton->setEnabled(active);
    mUi.clearAlarmButton->setEnabled(active);

    if (isModelValid())
    {
        auto settings = getMachineSettingsHandle();
        mUi.user1Button->setEnabled(!settings->getUserCmd1().isEmpty());
        mUi.user2Button->setEnabled(!settings->getUserCmd2().isEmpty());
        mUi.user3Button->setEnabled(!settings->getUserCmd3().isEmpty());
        mUi.user4Button->setEnabled(!settings->getUserCmd4().isEmpty());
    }
    else
    {
        mUi.user1Button->setEnabled(false);
        mUi.user2Button->setEnabled(false);
        mUi.user3Button->setEnabled(false);
        mUi.user4Button->setEnabled(false);
    }
}

void ControlFormController::initialise()
{
    mSafePositionSet = false;
    mXYOriginSet = false;
    mZOriginSet = false;
    setFormActive(false);
}

void ControlFormController::highlightClearAlarmReset(bool highlight)
{
   if (highlight)
   {
       QPalette p = mUi.clearAlarmButton->palette();
       p.setColor(mUi.clearAlarmButton->backgroundRole(),QColor(200,40,40));
       mUi.clearAlarmButton->setPalette(p);

       p = mUi.resetButton->palette();
       p.setColor(mUi.resetButton->backgroundRole(),QColor(200,0,0));
       mUi.resetButton->setPalette(p);
   }
}

void ControlFormController::onGoToSafePositionButtonClicked()
{
    if (mSafePositionSet)
    {
        emit gcodeCommandManualSendSignal(GcodeCommand::GoToSafePositionCommand());
    }
}

void ControlFormController::onResetButtonClicked()
{
    emit gcodeCommandManualSendSignal(GcodeCommand::ResetCommand());
    mSafePositionSet = false;
}

void ControlFormController::onClearAlarmButtonClicked()
{
    emit gcodeCommandManualSendSignal(GcodeCommand::UnlockCommand());
}

void ControlFormController::onSetSafePositionButtonClicked()
{
    if (mSafePositionSet)
    {
        QMessageBox dialog(this);

        dialog.setWindowTitle(tr("Reset Safe Position"));
        dialog.setText(tr("A G28 Safe position has already been set.\n\nAre you sure you want to overwrite it?"));
        dialog.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        auto result = dialog.exec();

        if (result != QMessageBox::Ok)
        {
            evaluateGotoSafePosition();
            return;
        }
    }

    emit gcodeCommandManualSendSignal(GcodeCommand::SetSafePositionCommand());
    emit safePositionSetSignal();
    mSafePositionSet = true;
    evaluateGotoSafePosition();
}

bool ControlFormController::isModelValid()
{
    return mSettingsModelHandle != nullptr;
}

void ControlFormController::evaluateGotoSafePosition()
{
   mUi.goToSafePositionButton->setEnabled(mSafePositionSet);
}

void ControlFormController::onSettingsModelReady(SqlSettingsModel* model)
{
    mSettingsModelHandle = model;
    initialise();
}
