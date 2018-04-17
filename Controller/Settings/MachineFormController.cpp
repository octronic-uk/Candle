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

#include "Model/Settings/Sql/SqlSettingsModel.h"
#include "MachineFormController.h"
#include "ui_MachineForm.h"

MachineFormController::MachineFormController(QWidget *parent) :
    AbstractFormController(parent),
    mSettingsModelHandle(nullptr)
{
    mUi.setupUi(this);
    setupSignalSlots();
}

MachineFormController::~MachineFormController() {}

void MachineFormController::setupSignalSlots()
{
    // Machine Information
    connect(mUi.txtQueryStateTime, SIGNAL(valueChanged(QString)), this, SLOT(onQueryStateTimeValueChanged(QString)));
    connect(mUi.cboUnits, SIGNAL(activated(QString)), this, SLOT(onUnitsCurrentIndexChanged(QString)));
    connect(mUi.txtRapidSpeed, SIGNAL(valueChanged(QString)), this, SLOT(onRapidSpeedValueChanged(QString)));
    connect(mUi.txtAcceleration, SIGNAL(valueChanged(QString)), this, SLOT(onAccelerationValueChanged(QString)));
    connect(mUi.txtSpindleSpeedMin, SIGNAL(valueChanged(QString)), this, SLOT(onSpindleSpeedMinValueChanged(QString)));
    connect(mUi.txtSpindleSpeedMax, SIGNAL(valueChanged(QString)), this, SLOT(onSpindleSpeedMaxValueChanged(QString)));
    // Control
    connect(mUi.txtTouchCommand, SIGNAL(textEdited(QString)), this, SLOT(onTouchCommandValueChanged(QString)));
    connect(mUi.txtSafeCommand, SIGNAL(textEdited(QString)), this, SLOT(onSafeCommandValueChanged(QString)));
    connect(mUi.chkMoveOnRestore, SIGNAL(toggled(bool)), this, SLOT(onMoveOnRestoreToggled(bool)));
    connect(mUi.cboRestoreMode, SIGNAL(activated(QString)), this, SLOT(onRestoreModeCurrentIndexChanged(QString)));
    // User Commands
    connect(mUi.txtUserCommand0, SIGNAL(textEdited(QString)), this, SLOT(onUserCommand1ValueChanged(QString)));
    connect(mUi.txtUserCommand1, SIGNAL(textEdited(QString)), this, SLOT(onUserCommand2ValueChanged(QString)));
    connect(mUi.txtUserCommand2, SIGNAL(textEdited(QString)), this, SLOT(onUserCommand3ValueChanged(QString)));
    connect(mUi.txtUserCommand3, SIGNAL(textEdited(QString)), this, SLOT(onUserCommand4ValueChanged(QString)));
    // Height Mep
    connect(mUi.txtHeightMapProbingFeed, SIGNAL(textEdited(QString)), this, SLOT(onHeightMapProbingFeedValueChanged(QString)));
    // Work Area
    connect(mUi.workAreaWidthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onWorkAreaWidthValueChanged(int)));
    connect(mUi.workAreaHeightSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onWorkAreaHeightValueChanged(int)));
}

void MachineFormController::setFormActive(bool)
{

}

void MachineFormController::initialise()
{
    if (isModelValid())
    {
        auto settings = getSettings();
        setQueryStateTime(settings->getQueryPeriod());
        setRapidSpeed(settings->getRapidSpeed());
        setAcceleration(settings->getAcceleration());
        setSpindleSpeedMin(settings->getSpindleMin());
        setSpindleSpeedMax(settings->getSpindleMax());
        setTouchCommand(settings->getProbeCmds());
        setSafePositionCommand(settings->getSafePositionCmds());
        setMoveOnRestore(settings->getRestoreOrigin());
        setRestoreMode(settings->getRestoreType());
        setHeightmapProbingFeed(QString::number(settings->getHeightMapProbeFeed()));
        setUnits(settings->getUnits());
        mUi.workAreaWidthSpinBox->setValue(settings->getWorkAreaWidth());
        mUi.workAreaHeightSpinBox->setValue(settings->getWorkAreaHeight());
    }
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

void MachineFormController::setSettingsModel(SqlSettingsModel* handle)
{
    mSettingsModelHandle = handle;
    initialise();
}

void MachineFormController::onProfileChanged(Profile* profile)
{
    qDebug() << "MachineFormController: onProfileChanged";
    initialise();
}

void MachineFormController::onQueryStateTimeValueChanged(QString value)
{
    qDebug() << "MachineFormController: state time" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setQueryPeriod(value.toInt());
        commit();
    }
}

void MachineFormController::onUnitsCurrentIndexChanged(QString value)
{
    qDebug() << "MachineFormController: units" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setUnits(value == "mm" ? false : true);
        commit();
    }

}

void MachineFormController::onRapidSpeedValueChanged(QString value)
{
    qDebug() << "MachineFormController: rapid speed" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setRapidSpeed(value.toFloat());
        commit();
    }

}

void MachineFormController::onAccelerationValueChanged(QString value)
{
    qDebug() << "MachineFormController: acceleration" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setAcceleration(value.toFloat());
        commit();
    }

}

void MachineFormController::onSpindleSpeedMinValueChanged(QString value)
{
    qDebug() << "MachineFormController: spindle min" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setSpindleMin(value.toInt());
        commit();
    }

}

void MachineFormController::onSpindleSpeedMaxValueChanged(QString value)
{
    qDebug() << "MachineFormController: spindle max" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setSpindleMax(value.toInt());
        commit();
    }

}

void MachineFormController::onTouchCommandValueChanged(QString value)
{
    qDebug() << "MachineFormController: probe command" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setProbeCmds(value);
        commit();
    }

}

void MachineFormController::onSafeCommandValueChanged(QString value)
{
    qDebug() << "MachineFormController: safe command" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setSafePositionCmds(value);
        commit();
    }

}

void MachineFormController::onMoveOnRestoreToggled(bool value)
{
    qDebug() << "MachineFormController: move on restore" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setRestoreOrigin(value);
        commit();
    }
}

void MachineFormController::onRestoreModeCurrentIndexChanged(QString value)
{
    qDebug() << "MachineFormController: restore mode" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setRestoreType(value == "Plane" ? false : true);
        commit();
    }
}

void MachineFormController::onUserCommand1ValueChanged(QString value)
{
    qDebug() << "MachineFormController: uc1" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setUserCmd1(value);
        commit();
    }
}

void MachineFormController::onUserCommand2ValueChanged(QString value)
{
    qDebug() << "MachineFormController: uc2" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setUserCmd2(value);
        commit();
    }
}

void MachineFormController::onUserCommand3ValueChanged(QString value)
{
    qDebug() << "MachineFormController: uc3" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setUserCmd3(value);
        commit();
    }
}

void MachineFormController::onUserCommand4ValueChanged(QString value)
{
    qDebug() << "MachineFormController: uc4" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setUserCmd4(value);
        commit();
    }
}

void MachineFormController::onHeightMapProbingFeedValueChanged(QString value)
{
    qDebug() << "MachineFormController: height map" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setHeightMapProbeFeed(value.toInt());
        commit();
    }
}

void MachineFormController::onWorkAreaWidthValueChanged(int width)
{
    qDebug() << "MachineFormController: work area width" << width;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setWorkAreaWidth(width);
        commit();
    }
}

void MachineFormController::onWorkAreaHeightValueChanged(int height)
{
    qDebug() << "MachineFormController: work area height" << height;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setWorkAreaHeight(height);
        commit();
    }
}

MachineSettings* MachineFormController::getSettings()
{
    return mSettingsModelHandle
            ->getCurrentProfileHandle()
            ->getMachineSettingsHandle();
}

bool MachineFormController::isModelValid()
{
    if (!mSettingsModelHandle)
    {
        qDebug() << "MachineFormController: mSettingsModel = nullptr";
        return false;
    }

    Profile* profile = mSettingsModelHandle->getCurrentProfileHandle();

    if(!profile)
    {
        qDebug() << "MachineFormController: profile = nullptr";
        return false;
    }

    MachineSettings* settings = profile->getMachineSettingsHandle();

    if (!settings)
    {
        qDebug() << "MachineFormController: settings = nullptr";
        return false;
    }
    return true;
}

void MachineFormController::commit()
{
    auto settings = getSettings();
    mSettingsModelHandle->onMachineSettingsUpdated(settings);
}
