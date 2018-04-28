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
#include "Model/Gcode/GcodeCommand.h"

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
    mUi.spindleDefaultButton->setEnabled(active);
    mUi.spindleMinusOneButton->setEnabled(active);
    mUi.spindlePlusOneButton->setEnabled(active);
    mUi.spindleMinusTenButton->setEnabled(active);
    mUi.spindlePlusTenButton->setEnabled(active);

    mUi.feedDefaultButton->setEnabled(active);
    mUi.feedMinusOneButton->setEnabled(active);
    mUi.feedPlusOneButton->setEnabled(active);
    mUi.feedMinusTenButton->setEnabled(active);
    mUi.feedPlusTenButton->setEnabled(active);

    mUi.rapidDefaultButton->setEnabled(active);
    mUi.rapid25PercentButton->setEnabled(active);
    mUi.rapid50PercentButton->setEnabled(active);
}

void OverrideFormController::setupSignalSlots()
{
    qDebug() << "OverrideFormController: Setup Signals/Slots";

    // Spindle
    connect(mUi.spindleDefaultButton,SIGNAL(clicked(bool)),this,SLOT(onSpindleDefaultClicked(bool)));
    connect(mUi.spindlePlusOneButton,SIGNAL(clicked(bool)),this,SLOT(onSpindlePlusOneClicked(bool)));
    connect(mUi.spindleMinusOneButton,SIGNAL(clicked(bool)),this,SLOT(onSpindleMinusOneClicked(bool)));
    connect(mUi.spindlePlusTenButton,SIGNAL(clicked(bool)),this,SLOT(onSpindlePlusTenClicked(bool)));
    connect(mUi.spindleMinusTenButton,SIGNAL(clicked(bool)),this,SLOT(onSpindleMinusTenClicked(bool)));
    // Feed
    connect(mUi.feedDefaultButton,SIGNAL(clicked(bool)),this,SLOT(onFeedDefaultClicked(bool)));
    connect(mUi.feedPlusOneButton,SIGNAL(clicked(bool)),this,SLOT(onFeedPlusOneClicked(bool)));
    connect(mUi.feedMinusOneButton,SIGNAL(clicked(bool)),this,SLOT(onFeedMinusOneClicked(bool)));
    connect(mUi.feedPlusTenButton,SIGNAL(clicked(bool)),this,SLOT(onFeedPlusTenClicked(bool)));
    connect(mUi.feedMinusTenButton,SIGNAL(clicked(bool)),this,SLOT(onFeedMinusTenClicked(bool)));

    // Rapid
    connect(mUi.rapidDefaultButton,SIGNAL(clicked(bool)),this,SLOT(onRapidDefaultClicked(bool)));
    connect(mUi.rapid25PercentButton,SIGNAL(clicked(bool)),this,SLOT(onRapid25PercentClicked(bool)));
    connect(mUi.rapid50PercentButton,SIGNAL(clicked(bool)),this,SLOT(onRapid50PercentClicked(bool)));
}

void OverrideFormController::initialise()
{
    setFormActive(false);
}

void OverrideFormController::onFeedDefaultClicked(bool checked)
{
    emit gcodeCommandManualSendSignal(GcodeCommand::FeedOvDefault());
}

void OverrideFormController::onFeedPlusOneClicked(bool checked)
{

    emit gcodeCommandManualSendSignal(GcodeCommand::FeedOvPlusOne());
}

void OverrideFormController::onFeedMinusOneClicked(bool checked)
{
    emit gcodeCommandManualSendSignal(GcodeCommand::FeedOvMinusOne());
}

void OverrideFormController::onFeedPlusTenClicked(bool checked)
{
    emit gcodeCommandManualSendSignal(GcodeCommand::FeedOvPlusTen());
}

void OverrideFormController::onFeedMinusTenClicked(bool checked)
{
    emit gcodeCommandManualSendSignal(GcodeCommand::FeedOvMinusTen());
}

void OverrideFormController::onSpindleDefaultClicked(bool checked)
{
    emit gcodeCommandManualSendSignal(GcodeCommand::SpindleOvDefault());

}

void OverrideFormController::onSpindlePlusOneClicked(bool checked)
{
    emit gcodeCommandManualSendSignal(GcodeCommand::SpindleOvPlusOne());

}

void OverrideFormController::onSpindleMinusOneClicked(bool checked)
{
    emit gcodeCommandManualSendSignal(GcodeCommand::SpindleOvMinusOne());

}

void OverrideFormController::onSpindlePlusTenClicked(bool checked)
{
    emit gcodeCommandManualSendSignal(GcodeCommand::SpindleOvPlusTen());

}

void OverrideFormController::onSpindleMinusTenClicked(bool checked)
{
    emit gcodeCommandManualSendSignal(GcodeCommand::SpindleOvMinusTen());

}

void OverrideFormController::onRapidDefaultClicked(bool checked)
{
    emit gcodeCommandManualSendSignal(GcodeCommand::RapidOvDefault());

}

void OverrideFormController::onRapid25PercentClicked(bool checked)
{
    emit gcodeCommandManualSendSignal(GcodeCommand::RapidOv25Percent());

}

void OverrideFormController::onRapid50PercentClicked(bool checked)
{
    emit gcodeCommandManualSendSignal(GcodeCommand::RapidOv50Percent());
}
