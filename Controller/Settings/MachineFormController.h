/*
 * MachineForm.h
 *
 * Created: 08 2018 by Ashley
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

#include "Controller/AbstractFormController.h"
#include "Model/Settings/Settings.h"
#include "ui_MachineForm.h"

class SqlSettingsModel;
class MachineSettings;
class Profile;

class MachineFormController : public AbstractFormController
{
    Q_OBJECT

public:
    explicit MachineFormController(QWidget *parent = nullptr);
    ~MachineFormController() override;

    void setupSignalSlots() override;
    void setFormActive(bool) override;
    void initialise() override;

    QString safePositionCommand();
    void setSafePositionCommand(QString command);

    bool moveOnRestore();
    void setMoveOnRestore(bool value);

    int restoreMode();
    void setRestoreMode(int value);

    int spindleSpeedMin();
    void setSpindleSpeedMin(int speed);

    int spindleSpeedMax();
    void setSpindleSpeedMax(int speed);

    int rapidSpeed();
    void setRapidSpeed(int rapidSpeed);

    QString heightmapProbingFeed();
    void setHeightmapProbingFeed(QString heightmapProbingFeed);

    int acceleration();
    void setAcceleration(int acceleration);

    int queryStateTime();
    void setQueryStateTime(int queryStateTime);

    int units();
    void setUnits(int units);

    QString touchCommand();
    void setTouchCommand(QString touchCommand);

    QString userCommands(int index);
    void setUserCommands(int index, QString commands);

    void setSettingsModel(SqlSettingsModel* handle);
    void onProfileChanged(Profile*);


private slots:
    void onQueryStateTimeValueChanged(QString);
    void onUnitsCurrentIndexChanged(QString);
    void onRapidSpeedValueChanged(QString);
    void onAccelerationValueChanged(QString);
    void onSpindleSpeedMinValueChanged(QString);
    void onSpindleSpeedMaxValueChanged(QString);
    void onTouchCommandValueChanged(QString);
    void onSafeCommandValueChanged(QString);
    void onMoveOnRestoreToggled(bool);
    void onRestoreModeCurrentIndexChanged(QString);
    void onUserCommand1ValueChanged(QString);
    void onUserCommand2ValueChanged(QString);
    void onUserCommand3ValueChanged(QString);
    void onUserCommand4ValueChanged(QString);
    void onHeightMapProbingFeedValueChanged(QString);
    void onWorkAreaWidthValueChanged(int);
    void onWorkAreaHeightValueChanged(int);
private:
    Ui::MachineForm mUi;
    SqlSettingsModel* mSettingsModelHandle;


    MachineSettings*getSettings();
    bool isModelValid();
    void commit();
};
