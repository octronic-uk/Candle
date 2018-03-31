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

IniFileSettingsModel::IniFileSettingsModel(QObject* parent)
    : AbstractSettingsModel (parent)
{
    mSettingsDirectory = QDir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0));
    qDebug() << "IniFileSettingsModel: Constructing for directory" << mSettingsDirectory.path();
}

IniFileSettingsModel::~IniFileSettingsModel()
{
    qDebug() << "IniFileSettingsModel: Destructing";
}

void IniFileSettingsModel::preload()
{
    mSettings.setIniCodec("UTF-8");
    // Update v-sync in glformat
    QGLFormat fmt = QGLFormat::defaultFormat();
    fmt.setSwapInterval(mSettings.value("vsync", false).toBool() ? 1 : 0);
    QGLFormat::setDefaultFormat(fmt);
}

void IniFileSettingsModel::onLoadSettings()
{
    mSettingsLoading = true;
    // Global
    emit settingChangedSignal(GLOBAL_IGNORE_ERRORS, mSettings.value(GLOBAL_IGNORE_ERRORS, getDefault(global(GLOBAL_IGNORE_ERRORS))).toBool());
    emit settingChangedSignal(GLOBAL_AUTO_LINE, mSettings.value(GLOBAL_AUTO_LINE, getDefault(global(GLOBAL_AUTO_LINE))).toBool());
    // Serial
    emit settingChangedSignal(SERIAL_PORT_NAME, mSettings.value(SERIAL_PORT_NAME, getDefault(serial(SERIAL_PORT_NAME))).toString());
    emit settingChangedSignal(SERIAL_BAUD_RATE, mSettings.value(SERIAL_BAUD_RATE, getDefault(serial(SERIAL_BAUD_RATE))).toInt());
    // Tool
    emit settingChangedSignal(TOOL_DIAMETER, mSettings.value(TOOL_DIAMETER, getDefault(tool(TOOL_DIAMETER))).toDouble());
    emit settingChangedSignal(TOOL_LENGTH, mSettings.value(TOOL_LENGTH, getDefault(tool(TOOL_LENGTH))).toDouble());
    emit settingChangedSignal(TOOL_ANGLE, mSettings.value(TOOL_ANGLE, getDefault(tool(TOOL_ANGLE))).toDouble());
    emit settingChangedSignal(TOOL_TYPE, mSettings.value(TOOL_TYPE, getDefault(tool(TOOL_TYPE))).toInt());
    // Gfx
    emit settingChangedSignal(GFX_ANTIALIASING, mSettings.value(GFX_ANTIALIASING, getDefault(graphics(GFX_ANTIALIASING))).toBool());
    emit settingChangedSignal(GFX_MSAA, mSettings.value(GFX_MSAA, getDefault(graphics(GFX_MSAA))).toBool());
    emit settingChangedSignal(GFX_VSYNC, mSettings.value(GFX_VSYNC, getDefault(graphics(GFX_VSYNC))).toBool());
    emit settingChangedSignal(GFX_ZBUFFER, mSettings.value(GFX_ZBUFFER, getDefault(graphics(GFX_ZBUFFER))).toBool());
    emit settingChangedSignal(GFX_SIMPLIFY, mSettings.value(GFX_SIMPLIFY, getDefault(graphics(GFX_SIMPLIFY))).toBool());
    emit settingChangedSignal(GFX_SIMPLIFY_PRECISION, mSettings.value(GFX_SIMPLIFY_PRECISION, getDefault(graphics(GFX_SIMPLIFY_PRECISION))).toDouble());
    emit settingChangedSignal(GFX_GRAYSCALE_SEGMENTS, mSettings.value(GFX_GRAYSCALE_SEGMENTS, getDefault(graphics(GFX_GRAYSCALE_SEGMENTS))).toBool());
    emit settingChangedSignal(GFX_GRAYSCALE_S_CODE, mSettings.value(GFX_GRAYSCALE_S_CODE, getDefault(graphics(GFX_GRAYSCALE_S_CODE))).toBool());
    emit settingChangedSignal(GFX_DRAW_MODE_VECTORS, mSettings.value(GFX_DRAW_MODE_VECTORS, getDefault(graphics(GFX_DRAW_MODE_VECTORS))).toBool());
    // Ui
    emit settingChangedSignal(UI_JOG_STEP, mSettings.value(UI_JOG_STEP, getDefault(ui(UI_JOG_STEP))).toDouble());
    emit settingChangedSignal(UI_SPINDLE_SPEED, mSettings.value(UI_SPINDLE_SPEED, getDefault(ui(UI_SPINDLE_SPEED))).toInt());
    emit settingChangedSignal(UI_MOVE_ON_RESTORE, mSettings.value(UI_MOVE_ON_RESTORE, getDefault(ui(UI_MOVE_ON_RESTORE))).toBool());
    emit settingChangedSignal(UI_RESTORE_MODE, mSettings.value(UI_RESTORE_MODE, getDefault(ui(UI_RESTORE_MODE))).toInt());
    emit settingChangedSignal(UI_SHOW_PROGRAM_COMMANDS, mSettings.value(UI_SHOW_PROGRAM_COMMANDS, getDefault(ui(UI_SHOW_PROGRAM_COMMANDS))).toBool());
    emit settingChangedSignal(UI_CONSOLE_SHOW_UI_CMDS, mSettings.value(UI_CONSOLE_SHOW_UI_CMDS, getDefault(ui(UI_CONSOLE_SHOW_UI_CMDS))).toBool());
    emit settingChangedSignal(UI_SPINDLE_SPEED_MIN, mSettings.value(UI_SPINDLE_SPEED_MIN, getDefault(ui(UI_SPINDLE_SPEED_MAX))).toInt());
    emit settingChangedSignal(UI_SPINDLE_SPEED_MAX, mSettings.value(UI_SPINDLE_SPEED_MAX, getDefault(ui(UI_SPINDLE_SPEED_MIN))).toInt());
    emit settingChangedSignal(UI_LASER_POWER_MIN, mSettings.value(UI_LASER_POWER_MIN, getDefault(ui(UI_LASER_POWER_MIN))).toInt());
    emit settingChangedSignal(UI_LASER_POWER_MAX, mSettings.value(UI_LASER_POWER_MAX, getDefault(ui(UI_LASER_POWER_MAX))).toInt());
    emit settingChangedSignal(UI_PANEL_SHOW_USER_CMDS, mSettings.value(UI_PANEL_SHOW_USER_CMDS, getDefault(ui(UI_PANEL_SHOW_USER_CMDS))).toBool());
    emit settingChangedSignal(UI_PANEL_SHOW_SPINDLE, mSettings.value(UI_PANEL_SHOW_SPINDLE, getDefault(ui(UI_PANEL_SHOW_SPINDLE))).toBool());
    emit settingChangedSignal(UI_PANEL_SHOW_FEED, mSettings.value(UI_PANEL_SHOW_FEED, getDefault(ui(UI_PANEL_SHOW_FEED))).toBool());
    emit settingChangedSignal(UI_PANEL_SHOW_JOG, mSettings.value(UI_PANEL_SHOW_JOG, getDefault(ui(UI_PANEL_SHOW_JOG))).toBool());
    emit settingChangedSignal(UI_AUTOSCROLL_CHECKED, mSettings.value(UI_AUTOSCROLL_CHECKED, getDefault(ui(UI_AUTOSCROLL_CHECKED))).toBool());
    emit settingChangedSignal(UI_FEED_OVERRIDE, mSettings.value(UI_FEED_OVERRIDE, getDefault(ui(UI_FEED_OVERRIDE))).toBool());
    emit settingChangedSignal(UI_UNITS, mSettings.value(UI_UNITS, getDefault(ui(UI_UNITS))).toInt());
    emit settingChangedSignal(UI_LAST_FOLDER , mSettings.value(UI_LAST_FOLDER, getDefault(ui(UI_LAST_FOLDER))).toString());
    emit settingChangedSignal(UI_KEYBOARD_CONTROL,mSettings.value(UI_KEYBOARD_CONTROL, getDefault(ui(UI_KEYBOARD_CONTROL))).toBool());
    emit settingChangedSignal(UI_AUTO_COMPLETION, mSettings.value(UI_AUTO_COMPLETION, getDefault(ui(UI_AUTO_COMPLETION))).toBool());
    emit settingChangedSignal(UI_TOUCH_COMMAND, mSettings.value(UI_TOUCH_COMMAND, getDefault(ui(UI_TOUCH_COMMAND))).toString());
    emit settingChangedSignal(UI_SAFE_POSITION, mSettings.value(UI_SAFE_POSITION, getDefault(ui(UI_SAFE_POSITION))).toString());
    emit settingChangedSignal(UI_PRG_TABLE_HEADER, mSettings.value(UI_PRG_TABLE_HEADER, getDefault(ui(UI_PRG_TABLE_HEADER))).toByteArray());
    emit settingChangedSignal(UI_COMMAND_ITEMS, mSettings.value(UI_COMMAND_ITEMS, getDefault(ui(UI_COMMAND_ITEMS))).toStringList());
    emit settingChangedSignal(UI_COMMAND_INDEX, mSettings.value(UI_COMMAND_INDEX, getDefault(ui(UI_COMMAND_INDEX))).toInt());
    // Visualiser
    emit settingChangedSignal(VISUALISER_LINE_WIDTH, mSettings.value(VISUALISER_LINE_WIDTH, getDefault(visualiser(VISUALISER_LINE_WIDTH))).toDouble());
    emit settingChangedSignal(VISUALISER_ARC_LENGTH, mSettings.value(VISUALISER_ARC_LENGTH, getDefault(visualiser(VISUALISER_ARC_LENGTH))).toDouble());
    emit settingChangedSignal(VISUALISER_ARC_DEGREE, mSettings.value(VISUALISER_ARC_DEGREE, getDefault(visualiser(VISUALISER_ARC_DEGREE))).toDouble());
    emit settingChangedSignal(VISUALISER_ARC_DEGREE_MODE, mSettings.value(VISUALISER_ARC_DEGREE_MODE, getDefault(visualiser(VISUALISER_ARC_DEGREE_MODE))).toBool());
    emit settingChangedSignal(VISUALISER_RAPID_SPEED, mSettings.value(VISUALISER_RAPID_SPEED, getDefault(visualiser(VISUALISER_RAPID_SPEED))).toInt());
    emit settingChangedSignal(VISUALISER_ACCELERATION, mSettings.value(VISUALISER_ACCELERATION, getDefault(visualiser(VISUALISER_ACCELERATION))).toInt());
    emit settingChangedSignal(VISUALISER_FPS, mSettings.value(VISUALISER_FPS, getDefault(visualiser(VISUALISER_FPS))).toInt());
    emit settingChangedSignal(VISUALISER_QUERY_STATE_TIME, mSettings.value(VISUALISER_QUERY_STATE_TIME, getDefault(visualiser(VISUALISER_QUERY_STATE_TIME))).toInt());
    // HeightMap
    emit settingChangedSignal(HEIGHT_MAP_PROBING_FEED, mSettings.value(HEIGHT_MAP_PROBING_FEED, getDefault(heightMap(HEIGHT_MAP_PROBING_FEED))).toInt());
    emit settingChangedSignal(HEIGHT_MAP_BORDER_X, mSettings.value(HEIGHT_MAP_BORDER_X, getDefault(heightMap(HEIGHT_MAP_BORDER_X))).toDouble());
    emit settingChangedSignal(HEIGHT_MAP_BORDER_Y, mSettings.value(HEIGHT_MAP_BORDER_Y, getDefault(heightMap(HEIGHT_MAP_BORDER_Y))).toDouble());
    emit settingChangedSignal(HEIGHT_MAP_BORDER_WIDTH, mSettings.value(HEIGHT_MAP_BORDER_WIDTH, getDefault(heightMap(HEIGHT_MAP_BORDER_WIDTH))).toDouble());
    emit settingChangedSignal(HEIGHT_MAP_BORDER_HEIGHT, mSettings.value(HEIGHT_MAP_BORDER_HEIGHT, getDefault(heightMap(HEIGHT_MAP_BORDER_HEIGHT))).toDouble());
    emit settingChangedSignal(HEIGHT_MAP_BORDER_SHOW, mSettings.value(HEIGHT_MAP_BORDER_SHOW, getDefault(heightMap(HEIGHT_MAP_BORDER_SHOW))).toBool());
    emit settingChangedSignal(HEIGHT_MAP_GRID_X, mSettings.value(HEIGHT_MAP_GRID_X, getDefault(heightMap(HEIGHT_MAP_GRID_X))).toDouble());
    emit settingChangedSignal(HEIGHT_MAP_GRID_Y, mSettings.value(HEIGHT_MAP_GRID_Y, getDefault(heightMap(HEIGHT_MAP_GRID_Y))).toDouble());
    emit settingChangedSignal(HEIGHT_MAP_GRID_Z_TOP, mSettings.value(HEIGHT_MAP_GRID_Z_TOP, getDefault(heightMap(HEIGHT_MAP_GRID_Z_TOP))).toDouble());
    emit settingChangedSignal(HEIGHT_MAP_GRID_Z_BOTTOM, mSettings.value(HEIGHT_MAP_GRID_Z_BOTTOM, getDefault(heightMap(HEIGHT_MAP_GRID_Z_BOTTOM))).toDouble());
    emit settingChangedSignal(HEIGHT_MAP_GRID_SHOW, mSettings.value(HEIGHT_MAP_GRID_SHOW, getDefault(heightMap(HEIGHT_MAP_GRID_SHOW))).toBool());
    emit settingChangedSignal(HEIGHT_MAP_INTERPOLATION_STEP_X, mSettings.value(HEIGHT_MAP_INTERPOLATION_STEP_X, getDefault(heightMap(HEIGHT_MAP_INTERPOLATION_STEP_X))).toDouble());
    emit settingChangedSignal(HEIGHT_MAP_INTERPOLATION_STEP_Y, mSettings.value(HEIGHT_MAP_INTERPOLATION_STEP_Y, getDefault(heightMap(HEIGHT_MAP_INTERPOLATION_STEP_Y))).toDouble());
    emit settingChangedSignal(HEIGHT_MAP_INTERPOLATION_TYPE, mSettings.value(HEIGHT_MAP_INTERPOLATION_TYPE, getDefault(heightMap(HEIGHT_MAP_INTERPOLATION_TYPE))).toInt());
    emit settingChangedSignal(HEIGHT_MAP_INTERPOLATION_SHOW, mSettings.value(HEIGHT_MAP_INTERPOLATION_SHOW, getDefault(heightMap(HEIGHT_MAP_INTERPOLATION_SHOW))).toBool());
    mSettingsLoading = false;
}

void IniFileSettingsModel::onSaveSettings()
{
    mSettings.sync();
    mSettingsChanged = false;
}
