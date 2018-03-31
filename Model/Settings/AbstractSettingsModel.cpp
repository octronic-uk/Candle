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

AbstractSettingsModel::AbstractSettingsModel(QObject *parent) : QObject(parent)
{
    if (!mDefaultsInitialised)
    {
        initialiseDefaults();
    }
}

QVariant AbstractSettingsModel::getDefault(QString setting)
{
    if (!mDefaultsInitialised)
    {
        initialiseDefaults();
    }
    return mDefaultSettings.find(setting).value();
}

AbstractSettingsModel::~AbstractSettingsModel()
{

}

void AbstractSettingsModel::onSettingChanged(QString settingName, QVariant value)
{
   mSettings.setValue(settingName,value);
   mSettingsChanged = true;
}

QString AbstractSettingsModel::global(QString property)
{
   return QString("%1/%2").arg(GLOBAL).arg(property);
}

QString AbstractSettingsModel::tool(QString property)
{
   return QString("%1/%2").arg(TOOL).arg(property);
}

QString AbstractSettingsModel::ui(QString property)
{
   return QString("%1/%2").arg(UI).arg(property);
}

QString AbstractSettingsModel::serial(QString property)
{
   return QString("%1/%2").arg(SERIAL).arg(property);
}

QString AbstractSettingsModel::graphics(QString property)
{
   return QString("%1/%2").arg(GFX).arg(property);
}

QString AbstractSettingsModel::visualiser(QString property)
{
   return QString("%1/%2").arg(VISUALISER).arg(property);
}

QString AbstractSettingsModel::heightMap(QString property)
{
   return QString("%1/%2").arg(HEIGHT_MAP).arg(property);
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
const QString AbstractSettingsModel::UI_AUTOSCROLL_CHECKED = "";
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

// Defaults Map
void AbstractSettingsModel::initialiseDefaults()
{
    // Serial
    mDefaultSettings[serial(SERIAL_PORT_NAME)] = "";
    mDefaultSettings[serial(SERIAL_BAUD_RATE)] = SerialBaudRate::BAUD_115200;
    // Global
    mDefaultSettings[global(GLOBAL_IGNORE_ERRORS)] =  false;
    mDefaultSettings[global(GLOBAL_AUTO_LINE)] =  true;
    // Tool
    mDefaultSettings[tool(TOOL_DIAMETER)] = 3;
    mDefaultSettings[tool(TOOL_LENGTH)] = 15;
    mDefaultSettings[tool(TOOL_ANGLE)] = 0;
    mDefaultSettings[tool(TOOL_TYPE)] = 0;
    // Gfx
    mDefaultSettings[graphics(GFX_ANTIALIASING)] = true;
    mDefaultSettings[graphics(GFX_MSAA)] = true;
    mDefaultSettings[graphics(GFX_VSYNC)] = false;
    mDefaultSettings[graphics(GFX_ZBUFFER)] = false;
    mDefaultSettings[graphics(GFX_SIMPLIFY)] = false;
    mDefaultSettings[graphics(GFX_SIMPLIFY_PRECISION)] = 0;
    mDefaultSettings[graphics(GFX_GRAYSCALE_SEGMENTS)] = false;
    mDefaultSettings[graphics(GFX_GRAYSCALE_S_CODE)] = true;
    mDefaultSettings[graphics(GFX_DRAW_MODE_VECTORS)] = true;
   // Ui
   mDefaultSettings[ui(UI_JOG_STEP)] = 1;
   mDefaultSettings[ui(UI_SPINDLE_SPEED)] = 0;
   mDefaultSettings[ui(UI_MOVE_ON_RESTORE)] = false;
   mDefaultSettings[ui(UI_RESTORE_MODE)] = 0;
   mDefaultSettings[ui(UI_SHOW_PROGRAM_COMMANDS)] = false;
   mDefaultSettings[ui(UI_CONSOLE_SHOW_UI_CMDS)] = false;
   mDefaultSettings[ui(UI_SPINDLE_SPEED_MIN)] = 0;
   mDefaultSettings[ui(UI_SPINDLE_SPEED_MAX)] = 100;
   mDefaultSettings[ui(UI_LASER_POWER_MIN)] = 0;
   mDefaultSettings[ui(UI_LASER_POWER_MAX)] = 100;
   mDefaultSettings[ui(UI_PANEL_SHOW_USER_CMDS)] = true;
   mDefaultSettings[ui(UI_PANEL_SHOW_SPINDLE)] = true;
   mDefaultSettings[ui(UI_PANEL_SHOW_FEED)] =  true;
   mDefaultSettings[ui(UI_PANEL_SHOW_JOG)] = true;
   mDefaultSettings[ui(UI_AUTOSCROLL_CHECKED)] = false;
   mDefaultSettings[ui(UI_SPINDLE_SPEED)] = 100;
   mDefaultSettings[ui(UI_FEED_OVERRIDE)] = false;
   mDefaultSettings[ui(UI_UNITS)] = 0;
   mDefaultSettings[ui(UI_LAST_FOLDER)] = QDir::homePath();
   mDefaultSettings[ui(UI_KEYBOARD_CONTROL)] = false;
   mDefaultSettings[ui(UI_AUTO_COMPLETION)] = true;
   mDefaultSettings[ui(UI_TOUCH_COMMAND)] = "";
   mDefaultSettings[ui(UI_SAFE_POSITION)] = "";
   mDefaultSettings[ui(UI_PRG_TABLE_HEADER)] = QByteArray();
   mDefaultSettings[ui(UI_COMMAND_ITEMS)] = QStringList();
   mDefaultSettings[ui(UI_COMMAND_INDEX)] = -1;
   // Visualiser
   mDefaultSettings[visualiser(VISUALISER_LINE_WIDTH)] = 1;
   mDefaultSettings[visualiser(VISUALISER_ARC_LENGTH)] = 0;
   mDefaultSettings[visualiser(VISUALISER_ARC_DEGREE)] = 0;
   mDefaultSettings[visualiser(VISUALISER_ARC_DEGREE_MODE)] = true;
   mDefaultSettings[visualiser(VISUALISER_RAPID_SPEED)] = 0;
   mDefaultSettings[visualiser(VISUALISER_ACCELERATION)] = 10;
   mDefaultSettings[visualiser(VISUALISER_FPS)] = 60;
   mDefaultSettings[visualiser(VISUALISER_QUERY_STATE_TIME)] = 250;
   // HeightMap
   mDefaultSettings[heightMap(HEIGHT_MAP_PROBING_FEED)] =  0;
   mDefaultSettings[heightMap(HEIGHT_MAP_BORDER_X)] = 0;
   mDefaultSettings[heightMap(HEIGHT_MAP_BORDER_Y)] = 0;
   mDefaultSettings[heightMap(HEIGHT_MAP_BORDER_WIDTH)] = 1;
   mDefaultSettings[heightMap(HEIGHT_MAP_BORDER_HEIGHT)] = 1;
   mDefaultSettings[heightMap(HEIGHT_MAP_BORDER_SHOW)] = false;
   mDefaultSettings[heightMap(HEIGHT_MAP_GRID_X)] = 1;
   mDefaultSettings[heightMap(HEIGHT_MAP_GRID_Y)] = 1;
   mDefaultSettings[heightMap(HEIGHT_MAP_GRID_Z_TOP)] = 1;
   mDefaultSettings[heightMap(HEIGHT_MAP_GRID_Z_BOTTOM)] = -1;
   mDefaultSettings[heightMap(HEIGHT_MAP_GRID_SHOW)] = false;
   mDefaultSettings[heightMap(HEIGHT_MAP_INTERPOLATION_STEP_X)] = 1;
   mDefaultSettings[heightMap(HEIGHT_MAP_INTERPOLATION_STEP_Y)] = 1;
   mDefaultSettings[heightMap(HEIGHT_MAP_INTERPOLATION_TYPE)] =  0;
   mDefaultSettings[heightMap(HEIGHT_MAP_INTERPOLATION_SHOW)] = false;

   mDefaultsInitialised = true;
}

bool AbstractSettingsModel::mDefaultsInitialised = false;

QMap<QString, QVariant> AbstractSettingsModel::mDefaultSettings;
