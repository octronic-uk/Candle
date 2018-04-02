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
            Settings::ORGANISATION, qApp->applicationName()
        )
    );

    mSettings->setIniCodec("UTF-8");

    qDebug() << "IniFileSettingsModel: Constructing for"
             << mSettings->fileName();

    if (!QDir().exists(mSettings->fileName()))
    {
        qDebug() << "IniFileSettingsModel: Settings file not found"
                 << mSettings->fileName();
        initialiseDefaults();
    }

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

    // Settings::GLOBAL
    mSettings->beginGroup(Settings::GLOBAL);
    emit settingChangedSignal(Settings::GLOBAL, Settings::GLOBAL_IGNORE_ERRORS, mSettings->value(Settings::GLOBAL_IGNORE_ERRORS).toBool());
    emit settingChangedSignal(Settings::GLOBAL, Settings::GLOBAL_AUTO_LINE, mSettings->value(Settings::GLOBAL_AUTO_LINE).toBool());
    mSettings->endGroup();
    // Settings::SERIAL
    mSettings->beginGroup(Settings::SERIAL);
    emit settingChangedSignal(Settings::SERIAL, Settings::SERIAL_PORT_NAME, mSettings->value(Settings::SERIAL_PORT_NAME).toString());
    emit settingChangedSignal(Settings::SERIAL, Settings::SERIAL_BAUD_RATE, mSettings->value(Settings::SERIAL_BAUD_RATE).toInt());
    mSettings->endGroup();
    // Settings::TOOL
    mSettings->beginGroup(Settings::TOOL);
    emit settingChangedSignal(Settings::TOOL, Settings::TOOL_DIAMETER, mSettings->value(Settings::TOOL_DIAMETER).toDouble());
    emit settingChangedSignal(Settings::TOOL, Settings::TOOL_LENGTH, mSettings->value(Settings::TOOL_LENGTH).toDouble());
    emit settingChangedSignal(Settings::TOOL, Settings::TOOL_ANGLE, mSettings->value(Settings::TOOL_ANGLE).toDouble());
    emit settingChangedSignal(Settings::TOOL, Settings::TOOL_TYPE, mSettings->value(Settings::TOOL_TYPE).toInt());
    mSettings->endGroup();
    // Settings::GFX
    mSettings->beginGroup(Settings::GFX);
    emit settingChangedSignal(Settings::GFX, Settings::GFX_ANTIALIASING, mSettings->value(Settings::GFX_ANTIALIASING).toBool());
    emit settingChangedSignal(Settings::GFX, Settings::GFX_MSAA, mSettings->value(Settings::GFX_MSAA).toBool());
    emit settingChangedSignal(Settings::GFX, Settings::GFX_VSYNC, mSettings->value(Settings::GFX_VSYNC).toBool());
    emit settingChangedSignal(Settings::GFX, Settings::GFX_ZBUFFER, mSettings->value(Settings::GFX_ZBUFFER).toBool());
    emit settingChangedSignal(Settings::GFX, Settings::GFX_SIMPLIFY, mSettings->value(Settings::GFX_SIMPLIFY).toBool());
    emit settingChangedSignal(Settings::GFX, Settings::GFX_SIMPLIFY_PRECISION, mSettings->value(Settings::GFX_SIMPLIFY_PRECISION).toDouble());
    emit settingChangedSignal(Settings::GFX, Settings::GFX_GRAYSCALE_SEGMENTS, mSettings->value(Settings::GFX_GRAYSCALE_SEGMENTS).toBool());
    emit settingChangedSignal(Settings::GFX, Settings::GFX_GRAYSCALE_S_CODE, mSettings->value(Settings::GFX_GRAYSCALE_S_CODE).toBool());
    emit settingChangedSignal(Settings::GFX, Settings::GFX_DRAW_MODE_VECTORS, mSettings->value(Settings::GFX_DRAW_MODE_VECTORS).toBool());
    mSettings->endGroup();
    // Settings::UI
    mSettings->beginGroup(Settings::UI);
    emit settingChangedSignal(Settings::UI, Settings::UI_JOG_STEP, mSettings->value(Settings::UI_JOG_STEP).toDouble());
    emit settingChangedSignal(Settings::UI, Settings::UI_SPINDLE_SPEED, mSettings->value(Settings::UI_SPINDLE_SPEED).toInt());
    emit settingChangedSignal(Settings::UI, Settings::UI_MOVE_ON_RESTORE, mSettings->value(Settings::UI_MOVE_ON_RESTORE).toBool());
    emit settingChangedSignal(Settings::UI, Settings::UI_RESTORE_MODE, mSettings->value(Settings::UI_RESTORE_MODE).toInt());
    emit settingChangedSignal(Settings::UI, Settings::UI_SHOW_PROGRAM_COMMANDS, mSettings->value(Settings::UI_SHOW_PROGRAM_COMMANDS).toBool());
    emit settingChangedSignal(Settings::UI, Settings::UI_CONSOLE_SHOW_UI_CMDS, mSettings->value(Settings::UI_CONSOLE_SHOW_UI_CMDS).toBool());
    emit settingChangedSignal(Settings::UI, Settings::UI_SPINDLE_SPEED_MIN, mSettings->value(Settings::UI_SPINDLE_SPEED_MIN).toInt());
    emit settingChangedSignal(Settings::UI, Settings::UI_SPINDLE_SPEED_MAX, mSettings->value(Settings::UI_SPINDLE_SPEED_MAX).toInt());
    emit settingChangedSignal(Settings::UI, Settings::UI_LASER_POWER_MIN, mSettings->value(Settings::UI_LASER_POWER_MIN).toInt());
    emit settingChangedSignal(Settings::UI, Settings::UI_LASER_POWER_MAX, mSettings->value(Settings::UI_LASER_POWER_MAX).toInt());
    emit settingChangedSignal(Settings::UI, Settings::UI_PANEL_SHOW_USER_CMDS, mSettings->value(Settings::UI_PANEL_SHOW_USER_CMDS).toBool());
    emit settingChangedSignal(Settings::UI, Settings::UI_PANEL_SHOW_SPINDLE, mSettings->value(Settings::UI_PANEL_SHOW_SPINDLE).toBool());
    emit settingChangedSignal(Settings::UI, Settings::UI_PANEL_SHOW_FEED, mSettings->value(Settings::UI_PANEL_SHOW_FEED).toBool());
    emit settingChangedSignal(Settings::UI, Settings::UI_PANEL_SHOW_JOG, mSettings->value(Settings::UI_PANEL_SHOW_JOG).toBool());
    emit settingChangedSignal(Settings::UI, Settings::UI_AUTOSCROLL_CHECKED, mSettings->value(Settings::UI_AUTOSCROLL_CHECKED).toBool());
    emit settingChangedSignal(Settings::UI, Settings::UI_FEED_OVERRIDE, mSettings->value(Settings::UI_FEED_OVERRIDE).toBool());
    emit settingChangedSignal(Settings::UI, Settings::UI_UNITS, mSettings->value(Settings::UI_UNITS).toInt());
    emit settingChangedSignal(Settings::UI, Settings::UI_LAST_FOLDER , mSettings->value(Settings::UI_LAST_FOLDER).toString());
    emit settingChangedSignal(Settings::UI, Settings::UI_KEYBOARD_CONTROL,mSettings->value(Settings::UI_KEYBOARD_CONTROL).toBool());
    emit settingChangedSignal(Settings::UI, Settings::UI_AUTO_COMPLETION, mSettings->value(Settings::UI_AUTO_COMPLETION).toBool());
    emit settingChangedSignal(Settings::UI, Settings::UI_TOUCH_COMMAND, mSettings->value(Settings::UI_TOUCH_COMMAND).toString());
    emit settingChangedSignal(Settings::UI, Settings::UI_SAFE_POSITION, mSettings->value(Settings::UI_SAFE_POSITION).toString());
    emit settingChangedSignal(Settings::UI, Settings::UI_PRG_TABLE_HEADER, mSettings->value(Settings::UI_PRG_TABLE_HEADER).toByteArray());
    emit settingChangedSignal(Settings::UI, Settings::UI_COMMAND_ITEMS, mSettings->value(Settings::UI_COMMAND_ITEMS).toStringList());
    emit settingChangedSignal(Settings::UI, Settings::UI_COMMAND_INDEX, mSettings->value(Settings::UI_COMMAND_INDEX).toInt());
    mSettings->endGroup();
    // Settings::VISUALISER
    mSettings->beginGroup(Settings::VISUALISER);
    emit settingChangedSignal(Settings::VISUALISER, Settings::VISUALISER_LINE_WIDTH, mSettings->value(Settings::VISUALISER_LINE_WIDTH).toDouble());
    emit settingChangedSignal(Settings::VISUALISER, Settings::VISUALISER_ARC_LENGTH, mSettings->value(Settings::VISUALISER_ARC_LENGTH).toDouble());
    emit settingChangedSignal(Settings::VISUALISER, Settings::VISUALISER_ARC_DEGREE, mSettings->value(Settings::VISUALISER_ARC_DEGREE).toDouble());
    emit settingChangedSignal(Settings::VISUALISER, Settings::VISUALISER_ARC_DEGREE_MODE, mSettings->value(Settings::VISUALISER_ARC_DEGREE_MODE).toBool());
    emit settingChangedSignal(Settings::VISUALISER, Settings::VISUALISER_RAPID_SPEED, mSettings->value(Settings::VISUALISER_RAPID_SPEED).toInt());
    emit settingChangedSignal(Settings::VISUALISER, Settings::VISUALISER_ACCELERATION, mSettings->value(Settings::VISUALISER_ACCELERATION).toInt());
    emit settingChangedSignal(Settings::VISUALISER, Settings::VISUALISER_FPS, mSettings->value(Settings::VISUALISER_FPS).toInt());
    emit settingChangedSignal(Settings::VISUALISER, Settings::VISUALISER_QUERY_STATE_TIME, mSettings->value(Settings::VISUALISER_QUERY_STATE_TIME).toInt());
    mSettings->endGroup();
    // HeightMap
    mSettings->beginGroup(Settings::HEIGHT_MAP);
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_PROBING_FEED, mSettings->value(Settings::HEIGHT_MAP_PROBING_FEED).toInt());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_BORDER_X, mSettings->value(Settings::HEIGHT_MAP_BORDER_X).toDouble());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_BORDER_Y, mSettings->value(Settings::HEIGHT_MAP_BORDER_Y).toDouble());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_BORDER_WIDTH, mSettings->value(Settings::HEIGHT_MAP_BORDER_WIDTH).toDouble());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_BORDER_HEIGHT, mSettings->value(Settings::HEIGHT_MAP_BORDER_HEIGHT).toDouble());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_BORDER_SHOW, mSettings->value(Settings::HEIGHT_MAP_BORDER_SHOW).toBool());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_GRID_X, mSettings->value(Settings::HEIGHT_MAP_GRID_X).toDouble());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_GRID_Y, mSettings->value(Settings::HEIGHT_MAP_GRID_Y).toDouble());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_GRID_Z_TOP, mSettings->value(Settings::HEIGHT_MAP_GRID_Z_TOP).toDouble());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_GRID_Z_BOTTOM, mSettings->value(Settings::HEIGHT_MAP_GRID_Z_BOTTOM).toDouble());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_GRID_SHOW, mSettings->value(Settings::HEIGHT_MAP_GRID_SHOW).toBool());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_INTERPOLATION_STEP_X, mSettings->value(Settings::HEIGHT_MAP_INTERPOLATION_STEP_X).toDouble());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_INTERPOLATION_STEP_Y, mSettings->value(Settings::HEIGHT_MAP_INTERPOLATION_STEP_Y).toDouble());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_INTERPOLATION_TYPE, mSettings->value(Settings::HEIGHT_MAP_INTERPOLATION_TYPE).toInt());
    emit settingChangedSignal(Settings::HEIGHT_MAP, Settings::HEIGHT_MAP_INTERPOLATION_SHOW, mSettings->value(Settings::HEIGHT_MAP_INTERPOLATION_SHOW).toBool());
    mSettings->endGroup();
}

void IniFileSettingsModel::onSaveSettings()
{
    qDebug() << "IniFileSettingsModel: onSaveSettings()";
    mSettings->sync();
}
