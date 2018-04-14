/*
 * ControlFormController.h
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
#include "ui_ControlForm.h"
#include "Model/Gcode/GcodeCommand.h"

class SqlSettingsModel;
class MachineSettings;

using namespace Ui;

class ControlFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit ControlFormController(QWidget *parent = nullptr);
    ~ControlFormController() override;

    void updateControlsState();
    void setupSignalSlots() override;
    void setFormActive(bool active) override;
    void initialise() override;
    void highlightUnlockReset(bool highlight);

signals:
    void gcodeCommandManualSendSignal(GcodeCommand*);

public slots:
    void onSettingsModelReady(SqlSettingsModel* model);

private slots:
    void onResetButtonClicked();
    void onUnlockButtonClicked();
    void onSafePositionButtonClicked();
    void onHomeButtonClicked();
    void onProbeButtonClicked();
    void onZeroXYButtonClicked();
    void onRestoreOriginButtonClicked();
    void onZeroZButtonClicked();
    void onUserCommand1ButtonClicked();
    void onUserCommand2ButtonClicked();
    void onUserCommand3ButtonClicked();
    void onUserCommand4ButtonClicked();

private:
    ControlForm mUi;
    SqlSettingsModel* mSettingsModelHandle;

    bool isModelValid();
    MachineSettings* getMachineSettingsHandle();
};
