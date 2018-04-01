/*
 * AbstractSettingsModel.h
 *
 * Created: 30 2018 by Ashley
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

#include <QSharedPointer>
#include <QSettings>
#include "Model/SerialBaudRate.h"

class AbstractSettingsModel : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSettingsModel(QObject *parent = nullptr);
    virtual ~AbstractSettingsModel() = 0;

signals:
    void settingChangedSignal(QString groupName, QString settingName, QVariant value);

public slots:
    virtual void onSaveSettings() = 0;
    virtual void onLoadSettings() = 0;

    void onSettingChanged(QString groupName, QString settingName, QVariant value);

protected:
    void initialiseDefaults();

    QSharedPointer<QSettings> mSettings;
    bool mSettingsLoading;

public:

    const static QString ORGANISATION;

    // Global
    const static QString GLOBAL;
    const static QString GLOBAL_IGNORE_ERRORS;
    const static QString GLOBAL_AUTO_LINE;

    // Serial
    const static QString SERIAL;
    const static QString SERIAL_PORT_NAME;
    const static QString SERIAL_BAUD_RATE;

    // Tool
    const static QString TOOL;
    const static QString TOOL_DIAMETER;
    const static QString TOOL_LENGTH;
    const static QString TOOL_ANGLE;
    const static QString TOOL_TYPE;

    // Graphics
    const static QString GFX;
    const static QString GFX_ANTIALIASING;
    const static QString GFX_MSAA;
    const static QString GFX_VSYNC;
    const static QString GFX_ZBUFFER;
    const static QString GFX_SIMPLIFY;
    const static QString GFX_SIMPLIFY_PRECISION;
    const static QString GFX_GRAYSCALE_SEGMENTS;
    const static QString GFX_GRAYSCALE_S_CODE;
    const static QString GFX_DRAW_MODE_VECTORS;

    // Ui
    const static QString UI;
    const static QString UI_JOG_STEP;
    const static QString UI_SPINDLE_SPEED;
    const static QString UI_MOVE_ON_RESTORE;
    const static QString UI_RESTORE_MODE;
    const static QString UI_SHOW_PROGRAM_COMMANDS;
    const static QString UI_CONSOLE_SHOW_UI_CMDS;
    const static QString UI_SPINDLE_SPEED_MIN;
    const static QString UI_SPINDLE_SPEED_MAX;
    const static QString UI_LASER_POWER_MIN;
    const static QString UI_LASER_POWER_MAX;
    const static QString UI_PANEL_SHOW_USER_CMDS;
    const static QString UI_PANEL_SHOW_SPINDLE;
    const static QString UI_PANEL_SHOW_FEED;
    const static QString UI_PANEL_SHOW_JOG;
    const static QString UI_AUTOSCROLL_CHECKED;
    const static QString UI_UNITS;
    const static QString UI_LAST_FOLDER;
    const static QString UI_KEYBOARD_CONTROL;
    const static QString UI_AUTO_COMPLETION;
    const static QString UI_TOUCH_COMMAND;
    const static QString UI_SAFE_POSITION;
    const static QString UI_PRG_TABLE_HEADER;
    const static QString UI_COMMAND_ITEMS;
    const static QString UI_COMMAND_INDEX;
    const static QString UI_FEED_OVERRIDE;

    // Visualisser
    const static QString VISUALISER;
    const static QString VISUALISER_LINE_WIDTH;
    const static QString VISUALISER_ARC_LENGTH;
    const static QString VISUALISER_ARC_DEGREE;
    const static QString VISUALISER_ARC_DEGREE_MODE;
    const static QString VISUALISER_RAPID_SPEED;
    const static QString VISUALISER_ACCELERATION;
    const static QString VISUALISER_FPS;
    const static QString VISUALISER_QUERY_STATE_TIME;

    // Height Map
    const static QString HEIGHT_MAP;
    const static QString HEIGHT_MAP_PROBING_FEED;
    const static QString HEIGHT_MAP_BORDER_X;
    const static QString HEIGHT_MAP_BORDER_Y;
    const static QString HEIGHT_MAP_BORDER_WIDTH;
    const static QString HEIGHT_MAP_BORDER_HEIGHT;
    const static QString HEIGHT_MAP_BORDER_SHOW;
    const static QString HEIGHT_MAP_GRID_X;
    const static QString HEIGHT_MAP_GRID_Y;
    const static QString HEIGHT_MAP_GRID_Z_TOP;
    const static QString HEIGHT_MAP_GRID_Z_BOTTOM;
    const static QString HEIGHT_MAP_GRID_SHOW;
    const static QString HEIGHT_MAP_INTERPOLATION_STEP_X;
    const static QString HEIGHT_MAP_INTERPOLATION_STEP_Y;
    const static QString HEIGHT_MAP_INTERPOLATION_TYPE;
    const static QString HEIGHT_MAP_INTERPOLATION_SHOW;
};
