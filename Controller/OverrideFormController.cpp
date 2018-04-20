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
    mUi.rapidOverrideSlider->setEnabled(checked);
    if (!checked)
    {
        mUi.rapidOverrideSlider->setValue(100);
    }
    emit updateRapidOverrideSignal(mUi.rapidOverrideSlider->value());

}

void OverrideFormController::onFeedOverrideToggled(bool checked)
{
    mUi.feedOverrideSlider->setEnabled(checked);
    if (!checked)
    {
        mUi.feedOverrideSlider->setValue(100);
    }
    emit updateFeedOverrideSignal(mUi.feedOverrideSlider->value());
}

void OverrideFormController::setupSignalSlots()
{
    qDebug() << "OverrideFormController: Setup Signals/Slots";

    // Spindle
    connect
    (
        mUi.spindleOverrideSlider, SIGNAL(valueChanged(int)),
        this, SLOT(onSpindleSliderValueChanged(int))
    );
    connect
    (
        mUi.spindleOverrideButton, SIGNAL(toggled(bool)),
        this, SLOT(onSpindleOverrideToggled(bool))
    );

    // Feed
    connect(
        mUi.feedOverrideSlider, SIGNAL(valueChanged(int)),
        this, SLOT(onFeedSliderValueChanged(int))
    );
    connect(
        mUi.feedOverrideButton, SIGNAL(toggled(bool)),
        this, SLOT(onFeedOverrideToggled(bool))
    );

    connect(
        mUi.rapidOverrideSlider, SIGNAL(valueChanged(int)),
        this, SLOT(onRapidSliderValueChanged(int))
    );
    connect(
        mUi.rapidOverrideButton, SIGNAL(toggled(bool)),
        this, SLOT(onRapidOverrideToggled(bool))
    );
}

void OverrideFormController::onUpdateFeedOverride(float val)
{
    mUi.feedOverrideButton->setChecked(val != 100);
    mUi.feedOverrideSlider->setValue(val);
}

void OverrideFormController::onUpdateRapidOverride(float val)
{
    mUi.rapidOverrideButton->setChecked(val != 100);
    mUi.rapidOverrideSlider->setValue(val);
}

void OverrideFormController::onUpdateSpindleOverride(float val)
{
    mUi.spindleOverrideButton->setChecked(val != 100);
    mUi.spindleOverrideSlider->setValue(val);
}

void OverrideFormController::onFeedSliderValueChanged(int value)
{
    mUi.feedOverrideButton->setChecked(value != 100);
    emit updateFeedOverrideSignal(value);
}

void OverrideFormController::onRapidSliderValueChanged(int value)
{
    mUi.rapidOverrideButton->setChecked(value != 100);
    emit updateRapidOverrideSignal(value);
}
void OverrideFormController::onSpindleSliderValueChanged(int value)
{
    mUi.spindleOverrideButton->setChecked(value != 100);
    emit updateSpindleOverrideSignal(value);
}
