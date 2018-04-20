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
#include "OverrideFormController.h"

OverrideFormController::OverrideFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "SpindleFormController: Constructing";
    mUi.setupUi(this);
    setupSignalSlots();
}

OverrideFormController::~OverrideFormController()
{
    qDebug() << "SpindleFormController: Destructing";

}

void OverrideFormController::setFormActive(bool active)
{
    mUi.spindleOverrideButton->setEnabled(active);
    mUi.spindleOverrideButton->setChecked(false);
    mUi.spindleOverrideSlider->setEnabled(active);
    mUi.spindleOverrideSlider->setValue(100);

    mUi.feedOverrideButton->setEnabled(active);
    mUi.feedOverrideButton->setChecked(false);
    mUi.feedOverrideSlider->setEnabled(active);
    mUi.feedOverrideSlider->setValue(100);

    mUi.rapidOverrideButton->setEnabled(active);
    mUi.rapidOverrideButton->setChecked(false);
    mUi.rapidOverrideSlider->setEnabled(active);
    mUi.rapidOverrideSlider->setValue(100);


}

void OverrideFormController::initialise()
{
    mUi.spindleOverrideButton->setEnabled(false);
    mUi.spindleOverrideButton->setChecked(false);
    mUi.spindleOverrideSlider->setValue(100);
    mUi.spindleOverrideSlider->setEnabled(false);

    mUi.feedOverrideButton->setEnabled(false);
    mUi.feedOverrideButton->setChecked(false);
    mUi.feedOverrideSlider->setValue(100);
    mUi.feedOverrideSlider->setEnabled(false);

    mUi.rapidOverrideButton->setEnabled(false);
    mUi.rapidOverrideButton->setChecked(false);
    mUi.rapidOverrideSlider->setValue(100);
    mUi.rapidOverrideSlider->setEnabled(false);
}

void OverrideFormController::onSpindleOverrideToggled(bool checked)
{
    mUi.spindleOverrideSlider->setEnabled(checked);
    if (!checked)
    {
        mUi.spindleOverrideSlider->setValue(100);
    }
    emit updateSpindleOverrideSignal(mUi.spindleOverrideSlider->value());
}

void OverrideFormController::onRapidOverrideToggled(bool checked)
{

}

void OverrideFormController::onSpindleSliderValueChanged(int value)
{
    emit updateSpindleOverrideSignal(value);
}

void OverrideFormController::onUpdateSpindleOverride(float speed)
{
    bool enabled = speed != 100;
    mUi.spindleOverrideButton->setChecked(enabled);
    mUi.spindleOverrideSlider->setValue(speed);
}

void OverrideFormController::setupSignalSlots()
{
    qDebug() << "OverrideFormController: Setup Signals/Slots";

    connect
    (
        mUi.spindleOverrideSlider, SIGNAL(valueChanged(int)),
        this, SLOT(onSliderValueChanged(int))
    );
    connect
    (
        mUi.spindleOverrideButton, SIGNAL(toggled(bool)),
        this, SLOT(onCmdSpindleClicked(bool))
    );

    connect(
        mUi.feedOverrideSlider, SIGNAL(valueChanged(int)),
        this, SLOT(onFeedSliderValueChanged(int))
    );
    connect(
        mUi.feedOverrideButton, SIGNAL(toggled(bool)),
        this, SLOT(onFeedOverrideToggled(bool))
    );
}

void OverrideFormController::onUpdateFeedOverride(float val)
{
    mUi.feedOverrideButton->setChecked(val == 100);
    mUi.feedOverrideSlider->setValue(val);
}

void OverrideFormController::onUpdateRapidOverride(float val)
{

}

void OverrideFormController::onFeedOverrideToggled(bool checked)
{
    if (!checked)
    {
        mUi.feedOverrideSlider->setValue(100);
        emit updateFeedOverrideSignal(100);
    }
    else
    {
        emit updateFeedOverrideSignal(mUi.feedOverrideSlider->value());
    }
}


void OverrideFormController::onFeedSliderValueChanged(int value)
{
    mUi.feedOverrideButton->setChecked(value != 100);
    emit updateFeedOverrideSignal(value);
}

void OverrideFormController::onRapidSliderValueChanged(int value)
{

}
