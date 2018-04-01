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
#include <map>
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
    mSettings->beginGroup(groupName);
    mSettings->setValue(settingName,value);
    mSettings->endGroup();
}


void AbstractSettingsModel::initialiseDefaults()
{
    // Global
    mSettings->beginGroup(GLOBAL);
    mSettings->setValue(GLOBAL_IGNORE_ERRORS,false);
    mSettings->setValue(GLOBAL_AUTO_LINE,true);
    mSettings->endGroup();
    // Serial
    mSettings->beginGroup(SERIAL);
    mSettings->setValue(SERIAL_PORT_NAME,"");
    mSettings->setValue(SERIAL_BAUD_RATE,SerialBaudRate::BAUD_115200);
    mSettings->endGroup();
    // Tool
    mSettings->beginGroup(TOOL);
    mSettings->setValue(TOOL_DIAMETER,3);
    mSettings->setValue(TOOL_LENGTH,15);
    mSettings->setValue(TOOL_ANGLE,0);
    mSettings->setValue(TOOL_TYPE,0);
    mSettings->endGroup();
    // Gfx
    mSettings->beginGroup(GFX);
    mSettings->setValue(GFX_ANTIALIASING,true);
    mSettings->setValue(GFX_MSAA, true);
    mSettings->setValue(GFX_VSYNC, false);
    mSettings->setValue(GFX_ZBUFFER, false);
    mSettings->setValue(GFX_SIMPLIFY, false);
    mSettings->setValue(GFX_SIMPLIFY_PRECISION, 0);
    mSettings->setValue(GFX_GRAYSCALE_SEGMENTS, false);
    mSettings->setValue(GFX_GRAYSCALE_S_CODE, true);
    mSettings->setValue(GFX_DRAW_MODE_VECTORS,  true);
    mSettings->endGroup();
    // Ui
    mSettings->beginGroup(UI);
    mSettings->setValue(UI_JOG_STEP, 1);
    mSettings->setValue(UI_SPINDLE_SPEED,0);
    mSettings->setValue(UI_MOVE_ON_RESTORE,false);
    mSettings->setValue(UI_RESTORE_MODE,0);
    mSettings->setValue(UI_SHOW_PROGRAM_COMMANDS,false);
    mSettings->setValue(UI_CONSOLE_SHOW_UI_CMDS,false);
    mSettings->setValue(UI_SPINDLE_SPEED_MIN,0);
    mSettings->setValue(UI_SPINDLE_SPEED_MAX,100);
    mSettings->setValue(UI_LASER_POWER_MIN,0);
    mSettings->setValue(UI_LASER_POWER_MAX,100);
    mSettings->setValue(UI_PANEL_SHOW_USER_CMDS,true);
    mSettings->setValue(UI_PANEL_SHOW_SPINDLE,true);
    mSettings->setValue(UI_PANEL_SHOW_FEED, true);
    mSettings->setValue(UI_PANEL_SHOW_JOG,true);
    mSettings->setValue(UI_AUTOSCROLL_CHECKED,false);
    mSettings->setValue(UI_SPINDLE_SPEED,100);
    mSettings->setValue(UI_FEED_OVERRIDE,false);
    mSettings->setValue(UI_UNITS,0);
    mSettings->setValue(UI_LAST_FOLDER,QDir::homePath());
    mSettings->setValue(UI_KEYBOARD_CONTROL,false);
    mSettings->setValue(UI_AUTO_COMPLETION,true);
    mSettings->setValue(UI_TOUCH_COMMAND,"");
    mSettings->setValue(UI_SAFE_POSITION,"");
    mSettings->setValue(UI_PRG_TABLE_HEADER,QByteArray());
    mSettings->setValue(UI_COMMAND_ITEMS,QStringList());
    mSettings->setValue(UI_COMMAND_INDEX,-1);
    mSettings->endGroup();
    // Visualiser
    mSettings->beginGroup(VISUALISER);
    mSettings->setValue(VISUALISER_LINE_WIDTH,1);
    mSettings->setValue(VISUALISER_ARC_LENGTH,0);
    mSettings->setValue(VISUALISER_ARC_DEGREE,0);
    mSettings->setValue(VISUALISER_ARC_DEGREE_MODE,true);
    mSettings->setValue(VISUALISER_RAPID_SPEED,0);
    mSettings->setValue(VISUALISER_ACCELERATION,10);
    mSettings->setValue(VISUALISER_FPS,60);
    mSettings->setValue(VISUALISER_QUERY_STATE_TIME,250);
    mSettings->endGroup();
    // HeightMap
    mSettings->beginGroup(HEIGHT_MAP);
    mSettings->setValue(HEIGHT_MAP_PROBING_FEED,0);
    mSettings->setValue(HEIGHT_MAP_BORDER_X,0);
    mSettings->setValue(HEIGHT_MAP_BORDER_Y,0);
    mSettings->setValue(HEIGHT_MAP_BORDER_WIDTH,1);
    mSettings->setValue(HEIGHT_MAP_BORDER_HEIGHT,1);
    mSettings->setValue(HEIGHT_MAP_BORDER_SHOW,false);
    mSettings->setValue(HEIGHT_MAP_GRID_X,1);
    mSettings->setValue(HEIGHT_MAP_GRID_Y,1);
    mSettings->setValue(HEIGHT_MAP_GRID_Z_TOP,1);
    mSettings->setValue(HEIGHT_MAP_GRID_Z_BOTTOM,-1);
    mSettings->setValue(HEIGHT_MAP_GRID_SHOW,false);
    mSettings->setValue(HEIGHT_MAP_INTERPOLATION_STEP_X,1);
    mSettings->setValue(HEIGHT_MAP_INTERPOLATION_STEP_Y,1);
    mSettings->setValue(HEIGHT_MAP_INTERPOLATION_TYPE, 0);
    mSettings->setValue(HEIGHT_MAP_INTERPOLATION_SHOW,false);
    mSettings->endGroup();
}

// Global
const QString AbstractSettingsModel::GLOBAL = "Global";
const QString AbstractSettingsModel::GLOBAL_IGNORE_ERRORS = "IgnoreErrors";
const QString AbstractSettingsModel::GLOBAL_AUTO_LINE = "AutoLine";
// Serial
const QString AbstractSettingsModel::SERIAL = "Serial";
const QString AbstractSettingsModel::SERIAL_PORT_NAME = "PortName";
const QString AbstractSettingsModel::SERIAL_BAUD_RATE = "BaudRate";
// Tool
const QString AbstractSettingsModel::TOOL = "Tool";
const QString AbstractSettingsModel::TOOL_DIAMETER = "Diameter";
const QString AbstractSettingsModel::TOOL_LENGTH = "Length";
const QString AbstractSettingsModel::TOOL_ANGLE = "Angle";
const QString AbstractSettingsModel::TOOL_TYPE = "Type";
// Graphics
const QString AbstractSettingsModel::GFX = "Graphics";
const QString AbstractSettingsModel::GFX_ANTIALIASING = "AntiAliasing";
const QString AbstractSettingsModel::GFX_MSAA = "MSAA";
const QString AbstractSettingsModel::GFX_VSYNC = "VSync";
const QString AbstractSettingsModel::GFX_ZBUFFER = "ZBuffer";
const QString AbstractSettingsModel::GFX_SIMPLIFY = "Simplify";
const QString AbstractSettingsModel::GFX_SIMPLIFY_PRECISION = "SimplifyPrecision";
const QString AbstractSettingsModel::GFX_GRAYSCALE_SEGMENTS = "GrayscaleSegments";
const QString AbstractSettingsModel::GFX_GRAYSCALE_S_CODE = "GrayscaleSCode";
const QString AbstractSettingsModel::GFX_DRAW_MODE_VECTORS = "DrawModeVectors";
// Ui
const QString AbstractSettingsModel::UI = "Ui";
const QString AbstractSettingsModel::UI_JOG_STEP = "JogStep";
const QString AbstractSettingsModel::UI_SPINDLE_SPEED = "SpindleSpeed";
const QString AbstractSettingsModel::UI_MOVE_ON_RESTORE = "MoveOnRestore";
const QString AbstractSettingsModel::UI_RESTORE_MODE = "RestoreMode";
const QString AbstractSettingsModel::UI_SHOW_PROGRAM_COMMANDS = "ShowProgramCommands";
const QString AbstractSettingsModel::UI_CONSOLE_SHOW_UI_CMDS = "ShowUiCommandsInConsole";
const QString AbstractSettingsModel::UI_SPINDLE_SPEED_MIN = "SpindleSpeedMin";
const QString AbstractSettingsModel::UI_SPINDLE_SPEED_MAX =  "SpindleSpeedMax";
const QString AbstractSettingsModel::UI_LASER_POWER_MIN = "LaserPowerMin";
const QString AbstractSettingsModel::UI_LASER_POWER_MAX = "LaserPowerMax";
const QString AbstractSettingsModel::UI_PANEL_SHOW_USER_CMDS = "PanelShowUserCommands";
const QString AbstractSettingsModel::UI_PANEL_SHOW_SPINDLE = "PandlShowSpindle";
const QString AbstractSettingsModel::UI_PANEL_SHOW_FEED = "PanelShowFeed";
const QString AbstractSettingsModel::UI_PANEL_SHOW_JOG = "PanelShowJog";
const QString AbstractSettingsModel::UI_AUTOSCROLL_CHECKED = "AutoscrollChecked";
const QString AbstractSettingsModel::UI_UNITS = "Uints";
const QString AbstractSettingsModel::UI_LAST_FOLDER = "LastFolder";
const QString AbstractSettingsModel::UI_KEYBOARD_CONTROL= "KeyboardControl";
const QString AbstractSettingsModel::UI_AUTO_COMPLETION = "AutoCompletion";
const QString AbstractSettingsModel::UI_TOUCH_COMMAND = "TouchCommand";
const QString AbstractSettingsModel::UI_SAFE_POSITION = "SafePosition";
const QString AbstractSettingsModel::UI_PRG_TABLE_HEADER= "ProgramTableHeader";
const QString AbstractSettingsModel::UI_COMMAND_ITEMS = "CommandItems";
const QString AbstractSettingsModel::UI_COMMAND_INDEX = "CommandIndex";
const QString AbstractSettingsModel::UI_FEED_OVERRIDE = "FeedOverride";
// Visualisser
const QString AbstractSettingsModel::VISUALISER = "Visualiser";
const QString AbstractSettingsModel::VISUALISER_LINE_WIDTH = "LineWidth";
const QString AbstractSettingsModel::VISUALISER_ARC_LENGTH = "ArcLength";
const QString AbstractSettingsModel::VISUALISER_ARC_DEGREE = "ArcDegree";
const QString AbstractSettingsModel::VISUALISER_ARC_DEGREE_MODE = "ArcDegreeMode";
const QString AbstractSettingsModel::VISUALISER_RAPID_SPEED = "RapidSpeed";
const QString AbstractSettingsModel::VISUALISER_ACCELERATION = "Acceleration";
const QString AbstractSettingsModel::VISUALISER_FPS = "FPS";
const QString AbstractSettingsModel::VISUALISER_QUERY_STATE_TIME = "QueryStateTime";
// Height Map
const QString AbstractSettingsModel::HEIGHT_MAP  = "HeightMap";
const QString AbstractSettingsModel::HEIGHT_MAP_PROBING_FEED = "ProbingFeed";
const QString AbstractSettingsModel::HEIGHT_MAP_BORDER_X = "BorderX";
const QString AbstractSettingsModel::HEIGHT_MAP_BORDER_Y = "BorderY";
const QString AbstractSettingsModel::HEIGHT_MAP_BORDER_WIDTH = "BorderWidth";
const QString AbstractSettingsModel::HEIGHT_MAP_BORDER_HEIGHT = "BorderHeight";
const QString AbstractSettingsModel::HEIGHT_MAP_BORDER_SHOW = "BorderShow";
const QString AbstractSettingsModel::HEIGHT_MAP_GRID_X = "GridX";
const QString AbstractSettingsModel::HEIGHT_MAP_GRID_Y = "GridY";
const QString AbstractSettingsModel::HEIGHT_MAP_GRID_Z_TOP = "ZTop";
const QString AbstractSettingsModel::HEIGHT_MAP_GRID_Z_BOTTOM = "ZBottom";
const QString AbstractSettingsModel::HEIGHT_MAP_GRID_SHOW = "GridShow";
const QString AbstractSettingsModel::HEIGHT_MAP_INTERPOLATION_STEP_X = "InterpolationStepX";
const QString AbstractSettingsModel::HEIGHT_MAP_INTERPOLATION_STEP_Y = "InterpolationStepY";
const QString AbstractSettingsModel::HEIGHT_MAP_INTERPOLATION_TYPE = "InterpolationType";
const QString AbstractSettingsModel::HEIGHT_MAP_INTERPOLATION_SHOW = "InterpolationShow";
const QString AbstractSettingsModel::ORGANISATION = "octronic";
