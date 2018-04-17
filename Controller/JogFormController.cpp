/*
 * JogFormController.cpp
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
#include "JogFormController.h"
#include <QPushButton>
#include <QtDebug>

#include "Model/Gcode/GcodeCommand.h"

JogFormController::JogFormController(QWidget *parent)
    : AbstractFormController(parent),
      mJogDelta(1.0),
      mKeyboardControl(false),
      mJoystickControl(false)
{
    qDebug() << "JogFormController: Constructing";
    mUi.setupUi(this);
    setupSignalSlots();
}

JogFormController::~JogFormController()
{
    qDebug() << "JogFormController: Destructing";
}

void JogFormController::onKeyboardControlToggled(bool checked)
{
   mKeyboardControl = checked;
}

void JogFormController::onJoystickControlToggled(bool checked)
{
    mJoystickControl=checked;
}



void JogFormController::setFormActive(bool active)
{
    QList<QWidget*> children = mUi.jogWidget->findChildren<QWidget*>();
    for (QWidget* child : children)
    {
        child->setEnabled(active);
    }
}

void JogFormController::initialise()
{

}

void JogFormController::onStepPresetButtonClicked()
{
    if (mUi.jogPreset_0_01->isChecked())
    {
        mJogDelta = 0.01;
    }
    else if (mUi.jogPreset_0_1->isChecked())
    {
        mJogDelta = 0.1;
    }
    else if (mUi.jogPreset_1->isChecked())
    {
        mJogDelta = 1.0;
    }
    else if (mUi.jogPreset_10->isChecked())
    {
        mJogDelta = 10.0;
    }
    else if (mUi.jogPreset_100->isChecked())
    {
        mJogDelta = 100.0;
    }

    mUi.stepSpinner->setSingleStep(mJogDelta);
    mUi.stepSpinner->setValue(mJogDelta);

    emit statusUpdateSignal(QString("Jog by %1 mm").arg(mJogDelta));
}

void JogFormController::onFeedRateValueChanged(int value)
{
    mFeedRate = value;
}

void JogFormController::onStepValueChanged(double value)
{
    emit statusUpdateSignal(QString("Jog by %1 mm").arg(value));
}

void JogFormController::onGotoXButtonClicked()
{

}

void JogFormController::onGotoYButtonClicked()
{

}

void JogFormController::onGotoZButtonClicked()
{

}

double JogFormController::getJogDistance() const
{
    return mJogDistance;
}

void JogFormController::setJogDistance(double jogDistance)
{
    mJogDistance = jogDistance;
}

void JogFormController::onJogDirectionButtonClicked()
{
    QObject* senderBtn = sender();

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    // Top Row
    if (senderBtn == mUi.jogXMinusYPlus)
    {
        x = -getStepValue();
        y = getStepValue();
    }
    else if (senderBtn == mUi.jogYPlus)
    {
        y = getStepValue();
    }
    else if (senderBtn == mUi.jogXPlusYPlus)
    {
        x = getStepValue();
        y = getStepValue();
    }
    // Middle Row
    else if (senderBtn == mUi.jogXMinus)
    {
        x = -getStepValue();
    }
    else if (senderBtn == mUi.jogOrigin)
    {
        // TODO - origin command
    }
    else if (senderBtn == mUi.jogXPlus)
    {
       x = getStepValue();
    }
    // Bottom Row
    else if (senderBtn == mUi.jogXMinusYMinus)
    {
        x = -getStepValue();
        y = -getStepValue();
    }
    else if (senderBtn == mUi.jogYMinus)
    {
        y = -getStepValue();
    }
    else if (senderBtn == mUi.jogXPlusYMinus)
    {
        x = getStepValue();
        y = -getStepValue();
    }
    // Z Axis
    else if (senderBtn == mUi.jogZPlus)
    {
        z = getStepValue();
    }
    else if (senderBtn == mUi.jogZMinus)
    {
        z = -getStepValue();
    }

    qDebug() << QString("JogFormController: Stepping: X:%1 Y:%2 Z:%3 @ F:%4")
                    .arg(x)
                    .arg(y)
                    .arg(z)
                    .arg(getFeedRate()
                );

    GcodeCommand* jogCmd = GcodeCommand::JogCommand(x,y,z,getFeedRate(),false,false);

    qDebug() << "JogFormController:" << jogCmd->getCommand();

    emit gcodeCommandManualSendSignal(jogCmd);
}

double JogFormController::getJogDelta() const
{
    return mJogDelta;
}

void JogFormController::setJogDelta(double jogDelta)
{
    mJogDelta = jogDelta;
}

// Jog

bool JogFormController::keyIsMovement(int key)
{
    return key == Qt::Key_4 || key == Qt::Key_6 || key == Qt::Key_8 || key == Qt::Key_2 || key == Qt::Key_9 || key == Qt::Key_3;
}

double JogFormController::getStepValue()
{
    return mUi.stepSpinner->value();
}

int JogFormController::getFeedRate()
{
   return mUi.feedRateSpinBox->value();
}

void JogFormController::setupSignalSlots()
{
    qDebug() << "JogFormController: Setup Signals/Slots";

    // Jog buttons
    // Top Row
    connect(mUi.jogXMinusYPlus,SIGNAL(clicked()),this,SLOT(onJogDirectionButtonClicked()));
    connect(mUi.jogYPlus,SIGNAL(clicked()),this,SLOT(onJogDirectionButtonClicked()));
    connect(mUi.jogXPlusYPlus,SIGNAL(clicked()),this,SLOT(onJogDirectionButtonClicked()));
    // Middle Row
    connect(mUi.jogXMinus,SIGNAL(clicked()),this,SLOT(onJogDirectionButtonClicked()));
    connect(mUi.jogOrigin,SIGNAL(clicked()),this,SLOT(onJogDirectionButtonClicked()));
    connect(mUi.jogXPlus,SIGNAL(clicked()),this,SLOT(onJogDirectionButtonClicked()));
    // Bottom Row
    connect(mUi.jogXMinusYMinus,SIGNAL(clicked()),this,SLOT(onJogDirectionButtonClicked()));
    connect(mUi.jogYMinus,SIGNAL(clicked()),this,SLOT(onJogDirectionButtonClicked()));
    connect(mUi.jogXPlusYMinus,SIGNAL(clicked()),this,SLOT(onJogDirectionButtonClicked()));
    // Z Axis
    connect(mUi.jogZPlus,SIGNAL(clicked()),this,SLOT(onJogDirectionButtonClicked()));
    connect(mUi.jogZMinus,SIGNAL(clicked()),this,SLOT(onJogDirectionButtonClicked()));

    // Step Spinner
    connect(mUi.stepSpinner,SIGNAL(valueChanged(double)),this,SLOT(onStepValueChanged(double)));

    // Step Preset Buttons
    connect(
        mUi.jogPreset_0_01,SIGNAL(clicked()),
        this, SLOT(onStepPresetButtonClicked())
    );
    connect(
        mUi.jogPreset_0_1,SIGNAL(clicked()),
        this, SLOT(onStepPresetButtonClicked())
    );
    connect(
        mUi.jogPreset_1,SIGNAL(clicked()),
        this, SLOT(onStepPresetButtonClicked())
    );
    connect(
        mUi.jogPreset_10,SIGNAL(clicked()),
        this, SLOT(onStepPresetButtonClicked())
    );
    connect(
        mUi.jogPreset_100,SIGNAL(clicked()),
        this, SLOT(onStepPresetButtonClicked())
    );

    // Feed Rate Spinner
    connect(mUi.feedRateSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onFeedRateValueChanged(int)));

    // Goto x,y,z
    connect(mUi.gotoXButton,SIGNAL(clicked()),this,SLOT(onGotoXButtonClicked()));
    connect(mUi.gotoYButton,SIGNAL(clicked()),this,SLOT(onGotoYButtonClicked()));
    connect(mUi.gotoZButton,SIGNAL(clicked()),this,SLOT(onGotoZButtonClicked()));

    // Keyboard/Joystic control
    connect(mUi.keyboardControlToolButton,SIGNAL(toggled(bool)),this,SLOT(onKeyboardControlToggled(bool)));
    connect(mUi.joystickControlToolButton,SIGNAL(toggled(bool)),this,SLOT(onJoystickControlToggled(bool)));
}
