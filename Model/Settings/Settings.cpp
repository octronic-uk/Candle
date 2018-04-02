/*
 * Settings.cpp
 *
 * Created: 01 2018 by Ashley
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
#include "Settings.h"

Settings::Settings(){}

// Global
const QString Settings::GLOBAL = "Global";
const QString Settings::GLOBAL_IGNORE_ERRORS = "IgnoreErrors";
const QString Settings::GLOBAL_AUTO_LINE = "AutoLine";
// Serial
const QString Settings::SERIAL = "Serial";
const QString Settings::SERIAL_PORT_NAME = "PortName";
const QString Settings::SERIAL_BAUD_RATE = "BaudRate";
// Tool
const QString Settings::TOOL = "Tool";
const QString Settings::TOOL_DIAMETER = "Diameter";
const QString Settings::TOOL_LENGTH = "Length";
const QString Settings::TOOL_ANGLE = "Angle";
const QString Settings::TOOL_TYPE = "Type";
// Graphics
const QString Settings::GFX = "Graphics";
const QString Settings::GFX_ANTIALIASING = "AntiAliasing";
const QString Settings::GFX_MSAA = "MSAA";
const QString Settings::GFX_VSYNC = "VSync";
const QString Settings::GFX_ZBUFFER = "ZBuffer";
const QString Settings::GFX_SIMPLIFY = "Simplify";
const QString Settings::GFX_SIMPLIFY_PRECISION = "SimplifyPrecision";
const QString Settings::GFX_GRAYSCALE_SEGMENTS = "GrayscaleSegments";
const QString Settings::GFX_GRAYSCALE_S_CODE = "GrayscaleSCode";
const QString Settings::GFX_DRAW_MODE_VECTORS = "DrawModeVectors";
// Ui
const QString Settings::UI = "Ui";
const QString Settings::UI_JOG_STEP = "JogStep";
const QString Settings::UI_SPINDLE_SPEED = "SpindleSpeed";
const QString Settings::UI_MOVE_ON_RESTORE = "MoveOnRestore";
const QString Settings::UI_RESTORE_MODE = "RestoreMode";
const QString Settings::UI_SHOW_PROGRAM_COMMANDS = "ShowProgramCommands";
const QString Settings::UI_CONSOLE_SHOW_UI_CMDS = "ShowUiCommandsInConsole";
const QString Settings::UI_SPINDLE_SPEED_MIN = "SpindleSpeedMin";
const QString Settings::UI_SPINDLE_SPEED_MAX =  "SpindleSpeedMax";
const QString Settings::UI_LASER_POWER_MIN = "LaserPowerMin";
const QString Settings::UI_LASER_POWER_MAX = "LaserPowerMax";
const QString Settings::UI_PANEL_SHOW_USER_CMDS = "PanelShowUserCommands";
const QString Settings::UI_PANEL_SHOW_SPINDLE = "PandlShowSpindle";
const QString Settings::UI_PANEL_SHOW_FEED = "PanelShowFeed";
const QString Settings::UI_PANEL_SHOW_JOG = "PanelShowJog";
const QString Settings::UI_AUTOSCROLL_CHECKED = "AutoscrollChecked";
const QString Settings::UI_UNITS = "Uints";
const QString Settings::UI_LAST_FOLDER = "LastFolder";
const QString Settings::UI_KEYBOARD_CONTROL= "KeyboardControl";
const QString Settings::UI_AUTO_COMPLETION = "AutoCompletion";
const QString Settings::UI_TOUCH_COMMAND = "TouchCommand";
const QString Settings::UI_SAFE_POSITION = "SafePosition";
const QString Settings::UI_PRG_TABLE_HEADER= "ProgramTableHeader";
const QString Settings::UI_COMMAND_ITEMS = "CommandItems";
const QString Settings::UI_COMMAND_INDEX = "CommandIndex";
const QString Settings::UI_FEED_OVERRIDE = "FeedOverride";
// Visualisser
const QString Settings::VISUALISER = "Visualiser";
const QString Settings::VISUALISER_LINE_WIDTH = "LineWidth";
const QString Settings::VISUALISER_ARC_LENGTH = "ArcLength";
const QString Settings::VISUALISER_ARC_DEGREE = "ArcDegree";
const QString Settings::VISUALISER_ARC_DEGREE_MODE = "ArcDegreeMode";
const QString Settings::VISUALISER_RAPID_SPEED = "RapidSpeed";
const QString Settings::VISUALISER_ACCELERATION = "Acceleration";
const QString Settings::VISUALISER_FPS = "FPS";
const QString Settings::VISUALISER_QUERY_STATE_TIME = "QueryStateTime";
// Height Map
const QString Settings::HEIGHT_MAP  = "HeightMap";
const QString Settings::HEIGHT_MAP_PROBING_FEED = "ProbingFeed";
const QString Settings::HEIGHT_MAP_BORDER_X = "BorderX";
const QString Settings::HEIGHT_MAP_BORDER_Y = "BorderY";
const QString Settings::HEIGHT_MAP_BORDER_WIDTH = "BorderWidth";
const QString Settings::HEIGHT_MAP_BORDER_HEIGHT = "BorderHeight";
const QString Settings::HEIGHT_MAP_BORDER_SHOW = "BorderShow";
const QString Settings::HEIGHT_MAP_GRID_X = "GridX";
const QString Settings::HEIGHT_MAP_GRID_Y = "GridY";
const QString Settings::HEIGHT_MAP_GRID_Z_TOP = "ZTop";
const QString Settings::HEIGHT_MAP_GRID_Z_BOTTOM = "ZBottom";
const QString Settings::HEIGHT_MAP_GRID_SHOW = "GridShow";
const QString Settings::HEIGHT_MAP_INTERPOLATION_STEP_X = "InterpolationStepX";
const QString Settings::HEIGHT_MAP_INTERPOLATION_STEP_Y = "InterpolationStepY";
const QString Settings::HEIGHT_MAP_INTERPOLATION_TYPE = "InterpolationType";
const QString Settings::HEIGHT_MAP_INTERPOLATION_SHOW = "InterpolationShow";
const QString Settings::ORGANISATION = "octronic";
