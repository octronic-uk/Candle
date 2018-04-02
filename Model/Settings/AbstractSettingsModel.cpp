/*
 * AbstractSettingsModel.cpp
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

#include "AbstractSettingsModel.h"
#include <QDir>
#include <QtDebug>


AbstractSettingsModel::AbstractSettingsModel(QObject *parent) : QObject(parent)
{

}

AbstractSettingsModel::~AbstractSettingsModel()
{

}

void AbstractSettingsModel::onSettingChanged(QString groupName, QString settingName, QVariant value)
{
    qDebug() << "AbstractSettingsModel: Received setting change "
             << groupName
             << settingName
             << value;

    mSettings->beginGroup(groupName);
    mSettings->setValue(settingName,value);
    mSettings->endGroup();
}


void AbstractSettingsModel::initialiseDefaults()
{
    qDebug() << "AbstractSettingsModel: Initialising Defaults";
    // Settings::GLOBAL
    mSettings->beginGroup(Settings::GLOBAL);
    mSettings->setValue(Settings::GLOBAL_IGNORE_ERRORS,false);
    mSettings->setValue(Settings::GLOBAL_AUTO_LINE,true);
    mSettings->endGroup();
    // Settings::SERIAL
    mSettings->beginGroup(Settings::SERIAL);
    mSettings->setValue(Settings::SERIAL_PORT_NAME,"");
    mSettings->setValue(Settings::SERIAL_BAUD_RATE,SerialBaudRate::BAUD_115200);
    mSettings->endGroup();
    // Settings::TOOL
    mSettings->beginGroup(Settings::TOOL);
    mSettings->setValue(Settings::TOOL_DIAMETER,3);
    mSettings->setValue(Settings::TOOL_LENGTH,15);
    mSettings->setValue(Settings::TOOL_ANGLE,0);
    mSettings->setValue(Settings::TOOL_TYPE,0);
    mSettings->endGroup();
    // Settings::GFX
    mSettings->beginGroup(Settings::GFX);
    mSettings->setValue(Settings::GFX_ANTIALIASING,true);
    mSettings->setValue(Settings::GFX_MSAA, true);
    mSettings->setValue(Settings::GFX_VSYNC, false);
    mSettings->setValue(Settings::GFX_ZBUFFER, false);
    mSettings->setValue(Settings::GFX_SIMPLIFY, false);
    mSettings->setValue(Settings::GFX_SIMPLIFY_PRECISION, 0);
    mSettings->setValue(Settings::GFX_GRAYSCALE_SEGMENTS, false);
    mSettings->setValue(Settings::GFX_GRAYSCALE_S_CODE, true);
    mSettings->setValue(Settings::GFX_DRAW_MODE_VECTORS,  true);
    mSettings->endGroup();
    // Settings::UI
    mSettings->beginGroup(Settings::UI);
    mSettings->setValue(Settings::UI_JOG_STEP, 1);
    mSettings->setValue(Settings::UI_SPINDLE_SPEED,0);
    mSettings->setValue(Settings::UI_MOVE_ON_RESTORE,false);
    mSettings->setValue(Settings::UI_RESTORE_MODE,0);
    mSettings->setValue(Settings::UI_SHOW_PROGRAM_COMMANDS,false);
    mSettings->setValue(Settings::UI_CONSOLE_SHOW_UI_CMDS,false);
    mSettings->setValue(Settings::UI_SPINDLE_SPEED_MIN,0);
    mSettings->setValue(Settings::UI_SPINDLE_SPEED_MAX,100);
    mSettings->setValue(Settings::UI_LASER_POWER_MIN,0);
    mSettings->setValue(Settings::UI_LASER_POWER_MAX,100);
    mSettings->setValue(Settings::UI_PANEL_SHOW_USER_CMDS,true);
    mSettings->setValue(Settings::UI_PANEL_SHOW_SPINDLE,true);
    mSettings->setValue(Settings::UI_PANEL_SHOW_FEED, true);
    mSettings->setValue(Settings::UI_PANEL_SHOW_JOG,true);
    mSettings->setValue(Settings::UI_AUTOSCROLL_CHECKED,false);
    mSettings->setValue(Settings::UI_SPINDLE_SPEED,100);
    mSettings->setValue(Settings::UI_FEED_OVERRIDE,false);
    mSettings->setValue(Settings::UI_UNITS,0);
    mSettings->setValue(Settings::UI_LAST_FOLDER,QDir::homePath());
    mSettings->setValue(Settings::UI_KEYBOARD_CONTROL,false);
    mSettings->setValue(Settings::UI_AUTO_COMPLETION,true);
    mSettings->setValue(Settings::UI_TOUCH_COMMAND,"");
    mSettings->setValue(Settings::UI_SAFE_POSITION,"");
    mSettings->setValue(Settings::UI_PRG_TABLE_HEADER,QByteArray());
    mSettings->setValue(Settings::UI_COMMAND_ITEMS,QStringList());
    mSettings->setValue(Settings::UI_COMMAND_INDEX,-1);
    mSettings->endGroup();
    // Settings::VISUALISER
    mSettings->beginGroup(Settings::VISUALISER);
    mSettings->setValue(Settings::VISUALISER_LINE_WIDTH,1);
    mSettings->setValue(Settings::VISUALISER_ARC_LENGTH,0);
    mSettings->setValue(Settings::VISUALISER_ARC_DEGREE,0);
    mSettings->setValue(Settings::VISUALISER_ARC_DEGREE_MODE,true);
    mSettings->setValue(Settings::VISUALISER_RAPID_SPEED,0);
    mSettings->setValue(Settings::VISUALISER_ACCELERATION,10);
    mSettings->setValue(Settings::VISUALISER_FPS,60);
    mSettings->setValue(Settings::VISUALISER_QUERY_STATE_TIME,250);
    mSettings->endGroup();
    // HeightMap
    mSettings->beginGroup(Settings::HEIGHT_MAP);
    mSettings->setValue(Settings::HEIGHT_MAP_PROBING_FEED,0);
    mSettings->setValue(Settings::HEIGHT_MAP_BORDER_X,0);
    mSettings->setValue(Settings::HEIGHT_MAP_BORDER_Y,0);
    mSettings->setValue(Settings::HEIGHT_MAP_BORDER_WIDTH,1);
    mSettings->setValue(Settings::HEIGHT_MAP_BORDER_HEIGHT,1);
    mSettings->setValue(Settings::HEIGHT_MAP_BORDER_SHOW,false);
    mSettings->setValue(Settings::HEIGHT_MAP_GRID_X,1);
    mSettings->setValue(Settings::HEIGHT_MAP_GRID_Y,1);
    mSettings->setValue(Settings::HEIGHT_MAP_GRID_Z_TOP,1);
    mSettings->setValue(Settings::HEIGHT_MAP_GRID_Z_BOTTOM,-1);
    mSettings->setValue(Settings::HEIGHT_MAP_GRID_SHOW,false);
    mSettings->setValue(Settings::HEIGHT_MAP_INTERPOLATION_STEP_X,1);
    mSettings->setValue(Settings::HEIGHT_MAP_INTERPOLATION_STEP_Y,1);
    mSettings->setValue(Settings::HEIGHT_MAP_INTERPOLATION_TYPE, 0);
    mSettings->setValue(Settings::HEIGHT_MAP_INTERPOLATION_SHOW,false);
    mSettings->endGroup();
}


