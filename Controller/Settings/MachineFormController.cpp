/*
 * MachineForm.cpp
 *
 * Created: 08 2018 by Ashley
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
#include "MachineFormController.h"
#include "ui_MachineForm.h"

MachineFormController::MachineFormController(QWidget *parent) :
    AbstractFormController(parent)
{
    mUi.setupUi(this);
}

MachineFormController::~MachineFormController() {}

void MachineFormController::setupSignalSlots()
{
    // Machine Information
    connect(mUi.txtQueryStateTime, SIGNAL(valueChanged(QString)), this, SLOT(onQueryStateTimeValueChanged(QString)));
    connect(mUi.cboUnits, SIGNAL(currentIndexChanged(QString)), this, SLOT(onUnitsCurrentIndexChanged(QString)));
    connect(mUi.txtRapidSpeed, SIGNAL(valueChanged(QString)), this, SLOT(onRapidSpeedValueChanged(QString)));
    connect(mUi.txtAcceleration, SIGNAL(valueChanged(QString)), this, SLOT(onAccelerationValueChanged(QString)));
    connect(mUi.txtSpindleSpeedMin, SIGNAL(valueChanged(QString)), this, SLOT(onSpindleSpeedMinValueChanged(QString)));
    connect(mUi.txtSpindleSpeedMax, SIGNAL(valueChanged(QString)), this, SLOT(onSpindleSpeedMaxValueChanged(QString)));
    // Control
    connect(mUi.txtTouchCommand, SIGNAL(textChanged(QString)), this, SLOT(onTouchCommandValueChanged(QString)));
    connect(mUi.txtSafeCommand, SIGNAL(textChanged(QString)), this, SLOT(onSafeCommandValueChanged(QString)));
    connect(mUi.chkMoveOnRestore, SIGNAL(toggled(bool)), this, SLOT(onMoveOnRestoreToggled(bool)));
    connect(mUi.cboRestoreMode, SIGNAL(currentIndexChanged(QString)), this, SLOT(onRestoreModeCurrentIndexChanged(QString)));
    // User Commands
    connect(mUi.txtUserCommand0, SIGNAL(textChanged(QString)), this, SLOT(onUserCommand1ValueChanged(QString)));
    connect(mUi.txtUserCommand1, SIGNAL(textChanged(QString)), this, SLOT(onUserCommand2ValueChanged(QString)));
    connect(mUi.txtUserCommand2, SIGNAL(textChanged(QString)), this, SLOT(onUserCommand3ValueChanged(QString)));
    connect(mUi.txtUserCommand3, SIGNAL(textChanged(QString)), this, SLOT(onUserCommand4ValueChanged(QString)));
    // Height Mep
    connect(mUi.txtHeightMapProbingFeed, SIGNAL(textChanged(QString)), this, SLOT(onHeightMapProbingFeedValueChanged(QString)));
}

void MachineFormController::setFormActive(bool)
{

}

void MachineFormController::initialise()
{
    setQueryStateTime(40);
    setRapidSpeed(2000);
    setAcceleration(100);
    setSpindleSpeedMin(0);
    setSpindleSpeedMax(10000);
    setTouchCommand("G21G91G38.2Z-30F100; G0Z1; G38.2Z-2F10");
    setSafePositionCommand("G21G90; G53G0Z0");
    setMoveOnRestore(false);
    setRestoreMode(0);
    setHeightmapProbingFeed("10");
    setUnits(0);
}

QString MachineFormController::safePositionCommand()
{
    return mUi.txtSafeCommand->text();
}

void MachineFormController::setSafePositionCommand(QString command)
{
    mUi.txtSafeCommand->setText(command);
}

bool MachineFormController::moveOnRestore()
{
    return mUi.chkMoveOnRestore->isChecked();
}

void MachineFormController::setMoveOnRestore(bool value)
{
    mUi.chkMoveOnRestore->setChecked(value);
}

int MachineFormController::restoreMode()
{
    return mUi.cboRestoreMode->currentIndex();
}

void MachineFormController::setRestoreMode(int value)
{
    mUi.cboRestoreMode->setCurrentIndex(value);
}

int MachineFormController::spindleSpeedMin()
{
    return mUi.txtSpindleSpeedMin->value();
}

void MachineFormController::setSpindleSpeedMin(int speed)
{
    mUi.txtSpindleSpeedMin->setValue(speed);
}

int MachineFormController::spindleSpeedMax()
{
    return mUi.txtSpindleSpeedMax->value();
}

void MachineFormController::setSpindleSpeedMax(int speed)
{
    mUi.txtSpindleSpeedMax->setValue(speed);
}

int MachineFormController::rapidSpeed()
{
    return mUi.txtRapidSpeed->value();
}

void MachineFormController::setRapidSpeed(int rapidSpeed)
{
    mUi.txtRapidSpeed->setValue(rapidSpeed);
}

QString MachineFormController::heightmapProbingFeed()
{
    return mUi.txtHeightMapProbingFeed->text();
}

void MachineFormController::setHeightmapProbingFeed(QString heightmapProbingFeed)
{
    mUi.txtHeightMapProbingFeed->setText(heightmapProbingFeed);
}

int MachineFormController::acceleration()
{
    return mUi.txtAcceleration->value();
}

void MachineFormController::setAcceleration(int acceleration)
{
    mUi.txtAcceleration->setValue(acceleration);
}

int MachineFormController::queryStateTime()
{
    return mUi.txtQueryStateTime->value();
}

void MachineFormController::setQueryStateTime(int queryStateTime)
{
    mUi.txtQueryStateTime->setValue(queryStateTime);
}

int MachineFormController::units()
{
    return mUi.cboUnits->currentIndex();
}

void MachineFormController::setUnits(int units)
{
    mUi.cboUnits->setCurrentIndex(units);
}

QString MachineFormController::touchCommand()
{
    return mUi.txtTouchCommand->text();
}

void MachineFormController::setTouchCommand(QString touchCommand)
{
    mUi.txtTouchCommand->setText(touchCommand);
}

QString MachineFormController::userCommands(int index)
{
    return this->findChild<QLineEdit*>(QString("txtUserCommand%1").arg(index))->text();
}

void MachineFormController::setUserCommands(int index, QString commands)
{
    this->findChild<QLineEdit*>(QString("txtUserCommand%1").arg(index))->setText(commands);
}

void MachineFormController::onQueryStateTimeValueChanged(QString value)
{
    emit settingChangedSignal(Settings::VISUALISER, Settings::VISUALISER_QUERY_STATE_TIME, value);
}

void MachineFormController::onUnitsCurrentIndexChanged(QString value)
{
    emit settingChangedSignal(Settings::UI, Settings::UI_UNITS, value);
}

void MachineFormController::onRapidSpeedValueChanged(QString value)
{
    emit settingChangedSignal(Settings::VISUALISER, Settings::VISUALISER_RAPID_SPEED, value);
}

void MachineFormController::onAccelerationValueChanged(QString value)
{
    emit settingChangedSignal(Settings::VISUALISER, Settings::VISUALISER_ACCELERATION, value);
}

void MachineFormController::onSpindleSpeedMinValueChanged(QString value)
{
    emit settingChangedSignal(Settings::UI, Settings::UI_SPINDLE_SPEED_MIN, value);
}

void MachineFormController::onSpindleSpeedMaxValueChanged(QString value)
{
    emit settingChangedSignal(Settings::UI, Settings::UI_SPINDLE_SPEED_MAX, value);
}

void MachineFormController::onTouchCommandValueChanged(QString value)
{
    emit settingChangedSignal(Settings::UI, Settings::UI_TOUCH_COMMAND, value);
}

void MachineFormController::onSafeCommandValueChanged(QString value)
{
    emit settingChangedSignal(Settings::UI, Settings::UI_SAFE_POSITION, value);
}

void MachineFormController::onMoveOnRestoreToggled(bool value)
{
    emit settingChangedSignal(Settings::UI, Settings::UI_MOVE_ON_RESTORE, value);
}

void MachineFormController::onRestoreModeCurrentIndexChanged(QString value)
{
    emit settingChangedSignal(Settings::UI, Settings::UI_RESTORE_MODE, value);
}

void MachineFormController::onUserCommand1ValueChanged(QString value)
{
    emit settingChangedSignal(Settings::USER_COMMAND, Settings::USER_COMMAND_1, value);
}

void MachineFormController::onUserCommand2ValueChanged(QString value)
{
    emit settingChangedSignal(Settings::USER_COMMAND, Settings::USER_COMMAND_2, value);
}

void MachineFormController::onUserCommand3ValueChanged(QString value)
{
    emit settingChangedSignal(Settings::USER_COMMAND, Settings::USER_COMMAND_3, value);
}

void MachineFormController::onUserCommand4ValueChanged(QString value)
{
    emit settingChangedSignal(Settings::USER_COMMAND, Settings::USER_COMMAND_4, value);
}

void MachineFormController::onHeightMapProbingFeedValueChanged(QString value)
{
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_PROBING_FEED, value);
}
