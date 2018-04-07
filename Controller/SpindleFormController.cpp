/*
 * SpindleFormController.cpp
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
#include "SpindleFormController.h"

SpindleFormController::SpindleFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "SpindleFormController: Constructing";
    mUi.setupUi(this);
    setupSignalSlots();
}

SpindleFormController::~SpindleFormController()
{
    qDebug() << "SpindleFormController: Destructing";

}

void SpindleFormController::setFormActive(bool active)
{
    mUi.cmdSpindle->setEnabled(active);
    mUi.rpmSlider->setEnabled(active);
    if (!active)
    {
        mUi.rpmSlider->setValue(100);
    }

}

void SpindleFormController::initialise()
{
    mUi.cmdSpindle->setEnabled(true);
    mUi.cmdSpindle->setChecked(false);
    mUi.rpmSlider->setValue(100);
    mUi.rpmSlider->setEnabled(false);
}

void SpindleFormController::onCmdSpindleClicked(bool checked)
{
    mUi.rpmSlider->setEnabled(checked);
    if (!checked)
    {
        mUi.rpmSlider->setValue(100);
    }
    emit updateSpindleSpeedSignal(mUi.rpmSlider->value());
}

void SpindleFormController::onSliderValueChanged(int value)
{
    emit updateSpindleSpeedSignal(value);
}

void SpindleFormController::onUpdateSpindleSpeed(float speed)
{
    bool enabled = speed != 100;
    mUi.cmdSpindle->setChecked(enabled);
    mUi.rpmSlider->setValue(speed);
}

void SpindleFormController::setupSignalSlots()
{
    qDebug() << "SpindleFormController: Setup Signals/Slots";
    connect
    (
        mUi.rpmSlider, SIGNAL(valueChanged(int)),
        this, SLOT(onSliderValueChanged(int))
    );
    connect
    (
        mUi.cmdSpindle, SIGNAL(toggled(bool)),
        this, SLOT(onCmdSpindleClicked(bool))
    );
}

