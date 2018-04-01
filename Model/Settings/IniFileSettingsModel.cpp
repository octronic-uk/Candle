/*
 * IniFileSettingsModel.cpp
 *
 * Created: 24 2018 by Ashley
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

#include "IniFileSettingsModel.h"
#include <QStandardPaths>
#include <QGLFormat>
#include <QtDebug>
#include <QApplication>

IniFileSettingsModel::IniFileSettingsModel(QObject* parent)
    : AbstractSettingsModel (parent)
{
    mSettings.reset(
        new QSettings(
            QSettings::IniFormat, QSettings::UserScope,
            ORGANISATION, qApp->applicationName()
        )
    );
    qDebug() << "IniFileSettingsModel: Constructing for"
             << mSettings->fileName();

    if (!QDir().exists(mSettings->fileName()))
    {
       initialiseDefaults();
    }

    mSettings->setIniCodec("UTF-8");
}

IniFileSettingsModel::~IniFileSettingsModel()
{
    qDebug() << "IniFileSettingsModel: Destructing";
    onSaveSettings();
}

void IniFileSettingsModel::preload()
{
    // Update v-sync in glformat
    QGLFormat fmt = QGLFormat::defaultFormat();
    fmt.setSwapInterval(mSettings->value("vsync", false).toBool() ? 1 : 0);
    QGLFormat::setDefaultFormat(fmt);
}

void IniFileSettingsModel::onLoadSettings()
{
    qDebug() << "IniFileSettingsModel: onLoadSettings()";
    // Global
    emit settingChangedSignal(GLOBAL, GLOBAL_IGNORE_ERRORS, mSettings->value(GLOBAL_IGNORE_ERRORS).toBool());
    emit settingChangedSignal(GLOBAL, GLOBAL_AUTO_LINE, mSettings->value(GLOBAL_AUTO_LINE).toBool());
    // Serial
    emit settingChangedSignal(SERIAL, SERIAL_PORT_NAME, mSettings->value(SERIAL_PORT_NAME).toString());
    emit settingChangedSignal(SERIAL, SERIAL_BAUD_RATE, mSettings->value(SERIAL_BAUD_RATE).toInt());
    // Tool
    emit settingChangedSignal(TOOL, TOOL_DIAMETER, mSettings->value(TOOL_DIAMETER).toDouble());
    emit settingChangedSignal(TOOL, TOOL_LENGTH, mSettings->value(TOOL_LENGTH).toDouble());
    emit settingChangedSignal(TOOL, TOOL_ANGLE, mSettings->value(TOOL_ANGLE).toDouble());
    emit settingChangedSignal(TOOL, TOOL_TYPE, mSettings->value(TOOL_TYPE).toInt());
    // Gfx
    emit settingChangedSignal(GFX, GFX_ANTIALIASING, mSettings->value(GFX_ANTIALIASING).toBool());
    emit settingChangedSignal(GFX, GFX_MSAA, mSettings->value(GFX_MSAA).toBool());
    emit settingChangedSignal(GFX, GFX_VSYNC, mSettings->value(GFX_VSYNC).toBool());
    emit settingChangedSignal(GFX, GFX_ZBUFFER, mSettings->value(GFX_ZBUFFER).toBool());
    emit settingChangedSignal(GFX, GFX_SIMPLIFY, mSettings->value(GFX_SIMPLIFY).toBool());
    emit settingChangedSignal(GFX, GFX_SIMPLIFY_PRECISION, mSettings->value(GFX_SIMPLIFY_PRECISION).toDouble());
    emit settingChangedSignal(GFX, GFX_GRAYSCALE_SEGMENTS, mSettings->value(GFX_GRAYSCALE_SEGMENTS).toBool());
    emit settingChangedSignal(GFX, GFX_GRAYSCALE_S_CODE, mSettings->value(GFX_GRAYSCALE_S_CODE).toBool());
    emit settingChangedSignal(GFX, GFX_DRAW_MODE_VECTORS, mSettings->value(GFX_DRAW_MODE_VECTORS).toBool());
    // Ui
    emit settingChangedSignal(UI, UI_JOG_STEP, mSettings->value(UI_JOG_STEP).toDouble());
    emit settingChangedSignal(UI, UI_SPINDLE_SPEED, mSettings->value(UI_SPINDLE_SPEED).toInt());
    emit settingChangedSignal(UI, UI_MOVE_ON_RESTORE, mSettings->value(UI_MOVE_ON_RESTORE).toBool());
    emit settingChangedSignal(UI, UI_RESTORE_MODE, mSettings->value(UI_RESTORE_MODE).toInt());
    emit settingChangedSignal(UI, UI_SHOW_PROGRAM_COMMANDS, mSettings->value(UI_SHOW_PROGRAM_COMMANDS).toBool());
    emit settingChangedSignal(UI, UI_CONSOLE_SHOW_UI_CMDS, mSettings->value(UI_CONSOLE_SHOW_UI_CMDS).toBool());
    emit settingChangedSignal(UI, UI_SPINDLE_SPEED_MIN, mSettings->value(UI_SPINDLE_SPEED_MIN).toInt());
    emit settingChangedSignal(UI, UI_SPINDLE_SPEED_MAX, mSettings->value(UI_SPINDLE_SPEED_MAX).toInt());
    emit settingChangedSignal(UI, UI_LASER_POWER_MIN, mSettings->value(UI_LASER_POWER_MIN).toInt());
    emit settingChangedSignal(UI, UI_LASER_POWER_MAX, mSettings->value(UI_LASER_POWER_MAX).toInt());
    emit settingChangedSignal(UI, UI_PANEL_SHOW_USER_CMDS, mSettings->value(UI_PANEL_SHOW_USER_CMDS).toBool());
    emit settingChangedSignal(UI, UI_PANEL_SHOW_SPINDLE, mSettings->value(UI_PANEL_SHOW_SPINDLE).toBool());
    emit settingChangedSignal(UI, UI_PANEL_SHOW_FEED, mSettings->value(UI_PANEL_SHOW_FEED).toBool());
    emit settingChangedSignal(UI, UI_PANEL_SHOW_JOG, mSettings->value(UI_PANEL_SHOW_JOG).toBool());
    emit settingChangedSignal(UI, UI_AUTOSCROLL_CHECKED, mSettings->value(UI_AUTOSCROLL_CHECKED).toBool());
    emit settingChangedSignal(UI, UI_FEED_OVERRIDE, mSettings->value(UI_FEED_OVERRIDE).toBool());
    emit settingChangedSignal(UI, UI_UNITS, mSettings->value(UI_UNITS).toInt());
    emit settingChangedSignal(UI, UI_LAST_FOLDER , mSettings->value(UI_LAST_FOLDER).toString());
    emit settingChangedSignal(UI, UI_KEYBOARD_CONTROL,mSettings->value(UI_KEYBOARD_CONTROL).toBool());
    emit settingChangedSignal(UI, UI_AUTO_COMPLETION, mSettings->value(UI_AUTO_COMPLETION).toBool());
    emit settingChangedSignal(UI, UI_TOUCH_COMMAND, mSettings->value(UI_TOUCH_COMMAND).toString());
    emit settingChangedSignal(UI, UI_SAFE_POSITION, mSettings->value(UI_SAFE_POSITION).toString());
    emit settingChangedSignal(UI, UI_PRG_TABLE_HEADER, mSettings->value(UI_PRG_TABLE_HEADER).toByteArray());
    emit settingChangedSignal(UI, UI_COMMAND_ITEMS, mSettings->value(UI_COMMAND_ITEMS).toStringList());
    emit settingChangedSignal(UI, UI_COMMAND_INDEX, mSettings->value(UI_COMMAND_INDEX).toInt());
    // Visualiser
    emit settingChangedSignal(VISUALISER, VISUALISER_LINE_WIDTH, mSettings->value(VISUALISER_LINE_WIDTH).toDouble());
    emit settingChangedSignal(VISUALISER, VISUALISER_ARC_LENGTH, mSettings->value(VISUALISER_ARC_LENGTH).toDouble());
    emit settingChangedSignal(VISUALISER, VISUALISER_ARC_DEGREE, mSettings->value(VISUALISER_ARC_DEGREE).toDouble());
    emit settingChangedSignal(VISUALISER, VISUALISER_ARC_DEGREE_MODE, mSettings->value(VISUALISER_ARC_DEGREE_MODE).toBool());
    emit settingChangedSignal(VISUALISER, VISUALISER_RAPID_SPEED, mSettings->value(VISUALISER_RAPID_SPEED).toInt());
    emit settingChangedSignal(VISUALISER, VISUALISER_ACCELERATION, mSettings->value(VISUALISER_ACCELERATION).toInt());
    emit settingChangedSignal(VISUALISER, VISUALISER_FPS, mSettings->value(VISUALISER_FPS).toInt());
    emit settingChangedSignal(VISUALISER, VISUALISER_QUERY_STATE_TIME, mSettings->value(VISUALISER_QUERY_STATE_TIME).toInt());
    // HeightMap
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_PROBING_FEED, mSettings->value(HEIGHT_MAP_PROBING_FEED).toInt());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_BORDER_X, mSettings->value(HEIGHT_MAP_BORDER_X).toDouble());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_BORDER_Y, mSettings->value(HEIGHT_MAP_BORDER_Y).toDouble());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_BORDER_WIDTH, mSettings->value(HEIGHT_MAP_BORDER_WIDTH).toDouble());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_BORDER_HEIGHT, mSettings->value(HEIGHT_MAP_BORDER_HEIGHT).toDouble());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_BORDER_SHOW, mSettings->value(HEIGHT_MAP_BORDER_SHOW).toBool());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_GRID_X, mSettings->value(HEIGHT_MAP_GRID_X).toDouble());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_GRID_Y, mSettings->value(HEIGHT_MAP_GRID_Y).toDouble());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_GRID_Z_TOP, mSettings->value(HEIGHT_MAP_GRID_Z_TOP).toDouble());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_GRID_Z_BOTTOM, mSettings->value(HEIGHT_MAP_GRID_Z_BOTTOM).toDouble());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_GRID_SHOW, mSettings->value(HEIGHT_MAP_GRID_SHOW).toBool());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_INTERPOLATION_STEP_X, mSettings->value(HEIGHT_MAP_INTERPOLATION_STEP_X).toDouble());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_INTERPOLATION_STEP_Y, mSettings->value(HEIGHT_MAP_INTERPOLATION_STEP_Y).toDouble());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_INTERPOLATION_TYPE, mSettings->value(HEIGHT_MAP_INTERPOLATION_TYPE).toInt());
    emit settingChangedSignal(HEIGHT_MAP, HEIGHT_MAP_INTERPOLATION_SHOW, mSettings->value(HEIGHT_MAP_INTERPOLATION_SHOW).toBool());
}

void IniFileSettingsModel::onSaveSettings()
{
    qDebug() << "IniFileSettingsModel: onSaveSettings()";
    mSettings->sync();
}
