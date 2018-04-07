/*
 * FeedFormController.cpp
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
#include "FeedFormController.h"

FeedFormController::FeedFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "FeedFormController: Constructing";
    mUi.setupUi(this);
    setupSignalSlots();
}

FeedFormController::~FeedFormController()
{
    qDebug() << "FeedFormController: Destructing";
}

void FeedFormController::setFormActive(bool active)
{
    mUi.overrideButton->setEnabled(active);
    mUi.overrideButton->setChecked(false);
    mUi.feedSlider->setEnabled(active);
    mUi.feedSlider->setValue(100);
}

void FeedFormController::initialise()
{
    mUi.overrideButton->setChecked(false);
    mUi.feedSlider->setValue(100);
}

void FeedFormController::onFeedRateUpdate(float val)
{

}

void FeedFormController::onFeedOverrideToggled(bool checked)
{
    if (!checked)
    {
        mUi.feedSlider->setValue(100);
        emit feedOverrideChangedSignal(100);
    }
    else
    {
        emit feedOverrideChangedSignal(mUi.feedSlider->value());
    }
}



void FeedFormController::setupSignalSlots()
{
    qDebug() << "FeedFormController: Setup Signals/Slots";
    connect(
        mUi.feedSlider, SIGNAL(valueChanged(int)),
        this, SLOT(onFeedSliderValueChanged(int))
    );
    connect(
        mUi.overrideButton, SIGNAL(toggled(bool)),
        this, SLOT(onFeedOverrideToggled(bool))
    );
}

void FeedFormController::onFeedSliderValueChanged(int value)
{
    mUi.overrideButton->setChecked(value != 100);
    emit feedOverrideChangedSignal(value);
}


