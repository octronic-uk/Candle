/*
 * SpindleFormController.h
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
#include "ui_OverrideForm.h"

class GcodeCommand;

using namespace Ui;

class OverrideFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit OverrideFormController(QWidget *parent = nullptr);
    ~OverrideFormController() override;

    void setupSignalSlots() override;
    void setFormActive(bool active) override;
    void initialise() override;

signals:
    void gcodeCommandManualSendSignal(GcodeCommand*);

public slots:

private slots:
    void onFeedDefaultClicked(bool);
    void onFeedPlusOneClicked(bool);
    void onFeedMinusOneClicked(bool);
    void onFeedPlusTenClicked(bool);
    void onFeedMinusTenClicked(bool);

    void onSpindleDefaultClicked(bool);
    void onSpindlePlusOneClicked(bool);
    void onSpindleMinusOneClicked(bool);
    void onSpindlePlusTenClicked(bool);
    void onSpindleMinusTenClicked(bool);

    void onRapidDefaultClicked(bool);
    void onRapid25PercentClicked(bool);
    void onRapid50PercentClicked(bool);

private:
    OverrideForm mUi;
};
