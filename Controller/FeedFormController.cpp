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
    mUi.feedProgress->setMinimum(mUi.feedSlider->minimum());
    mUi.feedProgress->setMaximum(mUi.feedSlider->maximum());
    onFeedSliderValueChanged(mUi.feedSlider->value());
    // TODO - Think about this one
    //mUi.overrideButton->setStyleSheet("QPushButton:checked { font-weight: bold; window-color: rgb(255, 0,0 ) }");
    setupSignalSlots();
}

FeedFormController::~FeedFormController()
{
    qDebug() << "FeedFormController: Destructing";
}

bool FeedFormController::isUpdatingFeed()
{
    return mIsUpdatingFeed;
}

void FeedFormController::setUpdatingFeed(bool updating)
{
    mIsUpdatingFeed = updating;
}

double FeedFormController::getOriginalFeedRate()
{
    return 100;
}

bool FeedFormController::isFeedOverrideChecked()
{
    return mUi.overrideButton->isChecked();
}

double FeedFormController::getFeedOverrideValue()
{
    return mUi.feedSlider->value();
}

void FeedFormController::setFormActive(bool active)
{

}

void FeedFormController::onFeedOverrideToggled(bool checked)
{
    if (!checked)
    {
        mUi.feedSlider->setValue(100);
        mUi.feedSlider->setEnabled(false);

        mUi.feedProgress->setValue(100);
        mUi.feedProgress->setEnabled(false);
    }
    else
    {
        mUi.feedSlider->setEnabled(true);
        mUi.feedProgress->setEnabled(true);
    }
    /*
    updateProgramEstimatedTime(mCurrentDrawer->viewParser()->getLineSegmentList());

    if (mIsProcessingFile)
    {
        mUi.txtFeed->setStyleSheet("color: red;");
        mIsUpdatingFeed = true;
    }
    */
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
    mUi.feedProgress->setValue(value);
    if (value != 100)
    {
        mUi.overrideButton->setChecked(true);
    }

    /*updateProgramEstimatedTime(mCurrentDrawer->viewParser()->getLineSegmentList());
    if (mIsProcessingFile && mUi.chkFeedOverride->isChecked())
    {
        mUi.txtFeed->setStyleSheet("color: red;");
        mIsUpdatingFeed = true;
    }
    */
}

