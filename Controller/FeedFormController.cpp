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

bool FeedFormController::isUpdatingFeed()
{

}

void FeedFormController::setUpdatingFeed(bool updating)
{

}

double FeedFormController::getOriginalFeedRate()
{

}

bool FeedFormController::isFeedOverrideChecked()
{

}

double FeedFormController::getTxtFeedValue()
{

}


// Feed

void FeedFormController::onChkBoxFeedOverrideToggled(bool checked)
{
    /*
    mUi.grpFeed->setProperty("overrided", checked);
    style()->unpolish(mUi.grpFeed);
    mUi.grpFeed->ensurePolished();
    updateProgramEstimatedTime(mCurrentDrawer->viewParser()->getLineSegmentList());

    if (mIsProcessingFile)
    {
        mUi.txtFeed->setStyleSheet("color: red;");
        mIsUpdatingFeed = true;
    }
    */
}

void FeedFormController::onTextFeedEditingFinished()
{
    mUi.sliFeed->setValue(mUi.txtFeed->value());
}

QString FeedFormController::feedOverride(QString command)
{
    /*
    // Feed override if not in heightmap probing mode
    if (!mUi.cmdHeightMapMode->isChecked())
    {
        command = GcodePreprocessorUtils::overrideSpeed(
            command,
            mUi.chkFeedOverride->isChecked() ? mUi.txtFeed->value() : 100,
            &mOriginalFeedRate
        );
    }
    return command;
    */
}

void FeedFormController::setupSignalSlots()
{

    qDebug() << "FeedFormController: Setup Signals/Slots";
}

void FeedFormController::onSliderFeedValueChanged(int value)
{
    /*
    mUi.txtFeed->setValue(value);
    updateProgramEstimatedTime(mCurrentDrawer->viewParser()->getLineSegmentList());
    if (mIsProcessingFile && mUi.chkFeedOverride->isChecked())
    {
        mUi.txtFeed->setStyleSheet("color: red;");
        mIsUpdatingFeed = true;
    }
    */
}

