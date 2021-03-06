/*
 * JogFormController.h
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

#pragma once

#include "AbstractFormController.h"
#include "ui_JogForm.h"

class GcodeCommand;

using namespace Ui;

class JogFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit JogFormController(QWidget *parent = nullptr);
    ~JogFormController() override;

    void blockJogForRapidMovement(bool repeated = false);
    void setFormActive(bool active) override;
    void initialise() override;
    void setupSignalSlots() override;
    double getJogDelta() const;
    void setJogDelta(double jogDelta);
    double getJogDistance() const;
    void setJogDistance(double jogDistance);

signals:
    void gcodeCommandManualSendSignal(GcodeCommand*);

private slots:
    void onJogDirectionButtonClicked();

    void onStepPresetButtonClicked();
    void onFeedRateValueChanged(int value);
    void onStepValueChanged(double value);

    void onGotoXButtonClicked();
    void onGotoYButtonClicked();
    void onGotoZButtonClicked();

    void onGotoXValueChanged(QString text);
    void onGotoYValueChanged(QString text);
    void onGotoZValueChanged(QString text);


    void onKeyboardControlToggled(bool checked);
    void onJoystickControlToggled(bool checked);
protected:
    void keyPressEvent(QKeyEvent *event) override;


private:
    JogForm mUi;
    double mJogDelta;
    double mJogDistance;
    int mFeedRate;
    QDoubleValidator *mDoubleValidator;

    // Keyboard
    bool mKeyboardControl;
    bool mJoystickControl;

private: // functions
    bool keyIsMovement(int key);
    double getStepValue();
    int getFeedRate();

};
