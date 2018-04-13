/*
 * SqlStrings.h
 *
 * Created: 10/4/2018 by Ashley
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

#include <QString>

const static QString SQLITE_DB = "QSQLITE";

const static QString DB_FILE_NAME = "co.uk.octronic.cocoanut_cnc.sqlite";

// Profiles ----------------------------------------------------------------------

const static QString CREATE_PROFILES_TABLE_QUERY =
    "CREATE TABLE 'profiles' ("
        "'id'   INTEGER PRIMARY KEY,"
        "'name' TEXT NOT NULL,"
        "'selected' INTEGER NOT NULL"
    ")";

const static QString INSERT_PROFILE_QUERY =
    "INSERT INTO 'profiles' ('name','selected') VALUES (?,?)";

const static QString SELECT_ALL_PROFILES_QUERY =
    "SELECT * FROM PROFILES";

const static QString UPDATE_PROFILE_WHERE_ID_QUERY =
    "UPDATE 'profiles' SET name=?, selected=? WHERE id=?";

const static QString DELETE_PROFILE_QUERY =
    "DELETE FROM 'profiles' WHERE 'id'=?";

// Recent Gcode Files ------------------------------------------------------------
const static QString CREATE_RECENT_GCODE_FILES_TABLE_QUERY =
    "CREATE TABLE 'recent_gcode_files' ("
        "'id'           INTEGER PRIMARY KEY,"
        "'profile_id'   INTEGER NOT NULL,"
        "'index'        INTEGER NOT NULL,"
        "'path'         TEXT NOT NULL,"
        "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
    ")";

const static QString SELECT_RECENT_GCODE_FILES_BY_PROFILE_ID_QUERY =
    "SELECT * FROM 'recent_gcode_files' WHERE profile_id=?";

// Recent HeightMap Files --------------------------------------------------------
const static QString CREATE_RECENT_HEIGHT_MAP_FILES_TABLE_QUERY =
    "CREATE TABLE 'recent_height_map_files' ("
        "'id'           INTEGER PRIMARY KEY,"
        "'profile_id'   INTEGER NOT NULL,"
        "'index'        INTEGER NOT NULL,"
        "'path'         TEXT NOT NULL,"
        "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
    ")";

const static QString SELECT_RECENT_HEIGHT_MAP_FILES_BY_PROFILE_ID_QUERY =
    "SELECT * FROM 'recent_height_map_files' WHERE profile_id=?";

// Connection --------------------------------------------------------------------
const static QString CREATE_CONNECTION_TABLE_QUERY =
    "CREATE TABLE 'connection' ("
        "'id'                        INTEGER PRIMARY KEY,"
        "'profile_id'                INTEGER NOT NULL,"
        "'serial_port'               TEXT,"
        "'serial_baud'               INTEGER,"
        "'ignore_error_responses'    INTEGER,"
        "'set_parser_state'          INTEGER,"
        "'arc_approximation'         INTEGER,"
        "'arc_approximation_length'  REAL,"
        "'arc_approximation_degrees' REAL,"
        "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
    ")";

const static QString SELECT_CONNECTION_BY_PROFILE_ID_QUERY =
    "SELECT * FROM 'connection' WHERE profile_id=?";

const static QString INSERT_CONNECTION_QUERY =
    "INSERT INTO 'connection' ("
        "'profile_id',"
        "'serial_port',"
        "'serial_baud',"
        "'ignore_error_responses',"
        "'set_parser_state',"
        "'arc_approximation',"
        "'arc_approximation_length',"
        "'arc_approximation_degrees'"
    ")"
    "VALUES ("
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?"
    ")";

const static QString UPDATE_CONNECTION_QUERY =
    "UPDATE 'connection' SET "
        "'profile_id'=?,"
        "'serial_port'=?,"
        "'serial_baud'=?,"
        "'ignore_error_responses'=?,"
        "'set_parser_state'=?,"
        "'arc_approximation'=?,"
        "'arc_approximation_length'=?,"
        "'arc_approximation_degrees'=?"
    " WHERE "
    "id=?";

const static QString DELETE_CONNECTION_QUERY =
    "DELETE FROM 'connection' WHERE 'id'=?";

// Interface ---------------------------------------------------------------------
const static QString CREATE_INTERFACE_TABLE_QUERY =
    "CREATE TABLE 'interface' ("
        "'id'                   INTEGER PRIMARY KEY,"
        "'profile_id'           INTEGER NOT NULL,"
        "'line_width'	        REAL,"
        "'fps_lock'	            INTEGER,"
        "'anti_aliasing'        INTEGER,"
        "'v_sync'	            INTEGER,"
        "'msaa'		            INTEGER,"
        "'z_buffer'	            INTEGER,"
        "'gcode_draw_mode'	    INTEGER,"
        "'simplify_geometry'	INTEGER,"
        "'simplify_precision'	REAL,"
        "'grayscale_segments'	INTEGER,"
        "'grayscale_code'		INTEGER,"
        "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
    ")";

const static QString SELECT_FROM_INTERFACE_QUERY =
    "SELECT * FROM 'interface' WHERE 'id'=?";

const static QString INSERT_INTERFACE_QUERY =
    "INSERT INTO 'interface' ("
        "'profile_id',"
        "'line_width',"
        "'fps_lock',"
        "'anti_aliasing',"
        "'v_sync',"
        "'msaa',"
        "'z_buffer',"
        "'gcode_draw_mode',"
        "'simplify_geometry',"
        "'simplify_precision',"
        "'grayscale_segments',"
        "'grayscale_code'"
    ")"
    "VALUES ("
        "?, "
        "?, "
        "?, "
        "?, "
        "?, "
        "?, "
        "?, "
        "?, "
        "?, "
        "?, "
        "?, "
        "? "
    ")";

const static QString UPDATE_INTERFACE_QUERY =
    "UPDATE 'interface' SET"
        "'profile_id'=?,,"
        "'line_width'=?,"
        "'fps_lock'=?,"
        "'anti_aliasing'=?,"
        "'v_sync'=?,"
        "'msaa'=?,"
        "'z_buffer'=?,"
        "'gcode_draw_mode'=?,"
        "'simplify_geometry'=?,"
        "'simplify_precision'=?,"
        "'grayscale_segments'=?,"
        "'grayscale_code'=?,"
    "WHERE 'id'=?";

const static QString DELETE_INTERFACE_QUERY =
    "DELETE FROM 'interface' WHERE 'id'=?";

// Machine -----------------------------------------------------------------------
const static QString CREATE_MACHINE_TABLE_QUERY =
    "CREATE TABLE 'machine' ("
        "'id'               INTEGER PRIMARY KEY,"
        "'profile_id'       INTEGER NOT NULL,"
        "'query_period' 	INTEGER,"
        "'units'		    INTEGER,"
        "'rapid_speed'	    REAL,"
        "'acceleration' 	REAL,"
        "'spindle_min'	    INTEGER,"
        "'spindle_max'  	INTEGER,"
        "'probe_cmds'	    TEXT,"
        "'safe_pos_cmds'	TEXT,"
        "'restore_origin'	INTEGER,"
        "'restore_type'	    INTEGER,"
        "'user_cmd_1'	    TEXT,"
        "'user_cmd_2'   	TEXT,"
        "'user_cmd_3'   	TEXT,"
        "'user_cmd_4'   	TEXT,"
        "'hm_probe_feed'	REAL,"
        "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
    ")";

const static QString SELECT_FROM_MACHINE_QUERY =
    "SELECT * FROM 'machine' WHERE 'id'=?";

const static QString INSERT_MACHINE_QUERY =
    "INSERT INTO 'machine' ("
        "'profile_id',"
        "'query_period',"
        "'units',"
        "'rapid_speed',"
        "'acceleration',"
        "'spindle_min',"
        "'spindle_max',"
        "'probe_cmds',"
        "'safe_pos_cmds',"
        "'restore_origin',"
        "'restore_type',"
        "'user_cmd_1',"
        "'user_cmd_2',"
        "'user_cmd_3',"
        "'user_cmd_4',"
        "'hm_probe_feed'"
    ")"
    "VALUES ("
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?,"
        "?"
    ")";

const static QString UPDATE_MACHINE_QUERY =
    "UPDATE 'machine' SET"
        "'profile_id'=?,"
        "'query_period'=?,"
        "'units'=?,"
        "'rapid_speed'=?,"
        "'acceleration'=?,"
        "'spindle_min'=?,"
        "'spindle_max'=?,"
        "'probe_cmds'=?,"
        "'safe_pos_cmds'=?,"
        "'restore_origin'=?,"
        "'restore_type'=?,"
        "'user_cmd_1'=?,"
        "'user_cmd_2'=?,"
        "'user_cmd_3'=?,"
        "'user_cmd_4'=?,"
        "'hm_probe_feed'=?"
    "WHERE id=?";

const static QString DELETE_MACHINE_QUERY =
    "DELETE FROM 'machine' WHERE id=?";

// Tools -------------------------------------------------------------------------
const static QString CREATE_TOOL_TABLE_QUERY =
    "CREATE TABLE 'tool' ("
        "'id'           INTEGER PRIMARY KEY,"
        "'profile_id'   INTEGER NOT NULL,"
        "'name'         TEXT,"
        "'tool_number'  INTEGER,"
        "'tool_holder_id' INTEGER,"
        "FOREIGN KEY('tool_holder_id') REFERENCES tool_holder('id')"
        "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
    ")";

const static QString INSERT_TOOL_QUERY =
    "INSERT INTO 'tool' "
        "(profile_id, name, tool_number, tool_holder_id) "
        "values(?, ?, ?, ?)";

const static QString SELECT_ALL_FROM_TOOL_BY_PROFILE_ID_QUERY =
    "SELECT * FROM 'tool' WHERE profile_id=?";

const static QString SELECT_TOOL_BY_ID_QUERY =
    "SELECT * FROM 'tool' WHERE id=?";

const static QString UPDATE_TOOL_QUERY =
    "UPDATE 'tool' SET profile_id=?, name=?, tool_number=?, tool_holder_id=? WHERE id=?";

const static QString DELETE_TOOL_QUERY =
    "DELETE FROM 'tool' WHERE id=?";

// Tool Geometry ---------------------------------------------------------
const static QString CREATE_TOOL_GEOMETRY_TABLE_QUERY =
    "CREATE TABLE 'tool_geometry' ("
        "'id'             INTEGER PRIMARY KEY,"
        "'tool_id'        INTEGER NOT NULL,"
        "'index'          INTEGER NOT NULL,"
        "'height'         REAL NOT NULL,"
        "'upper_diameter' REAL NOT NULL,"
        "'lower_diameter' REAL NOT NULL,"
        "FOREIGN KEY('tool_id') REFERENCES tools('id')"
    ")";

const static QString INSERT_TOOL_GEOMETRY_QUERY =
    "insert into tool_geometry ("
        "'tool_id',"
        "'index',"
        "'height', "
        "'upper_diameter',"
        "'lower_diameter'"
    ")"
    "values ("
        "?, "
        "?, "
        "?, "
        "?, "
        "?"
    ")";

const static QString UPDATE_TOOL_GEOMETRY_QUERY =
    "UPDATE 'tool_geometry' SET "
        "'index'=?, "
        "'height'=?, "
        "'upper_diameter'=?, "
        "'lower_diameter'=?  "
    "WHERE id=?";

const static QString SELECT_TOOL_GEOMETRY_BY_TOOL_ID_QUERY =
    "SELECT * FROM 'tool_geometry' WHERE tool_id=?";

const static QString DELETE_TOOL_GEOMETRY_QUERY =
    "DELETE FROM 'tool_geometry' WHERE id=?";


// Tool Holders ------------------------------------------------------------------
const static QString CREATE_TOOL_HOLDER_TABLE_QUERY =
    "CREATE TABLE 'tool_holder' ("
        "'id'           INTEGER PRIMARY KEY,"
        "'profile_id'   INTEGER NOT NULL,"
        "'name'         TEXT,"
        "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
    ")";

const static QString INSERT_TOOL_HOLDER_QUERY =
    "INSERT INTO 'tool_holder' (profile_id, name) values(?, ?)";

const static QString SELECT_ALL_FROM_TOOL_HOLDER_BY_PROFILE_ID_QUERY =
    "SELECT * FROM 'tool_holder' WHERE profile_id=?";

const static QString SELECT_TOOL_HOLDER_BY_ID_QUERY =
    "SELECT * FROM 'tool_holder' WHERE id=?";

const static QString UPDATE_TOOL_HOLDER_QUERY =
    "UPDATE 'tool_holder' SET profile_id=?, name=? WHERE id=?";

const static QString DELETE_TOOL_HOLDER_QUERY =
    "DELETE FROM 'tool_holder' WHERE id=?";

// Tool Holders Geometry ---------------------------------------------------------

const static QString CREATE_TOOL_HOLDER_GEOMETRY_TABLE_QUERY =
    "CREATE TABLE 'tool_holder_geometry' ("
        "'id'             INTEGER PRIMARY KEY,"
        "'tool_holder_id' INTEGER NOT NULL,"
        "'index'          INTEGER NOT NULL,"
        "'height'         REAL NOT NULL,"
        "'upper_diameter' REAL NOT NULL,"
        "'lower_diameter' REAL NOT NULL,"
        "FOREIGN KEY('tool_holder_id') REFERENCES tool_holder('id')"
    ")";

const static QString INSERT_TOOL_HOLDER_GEOMETRY_QUERY =
    "insert into tool_holder_geometry ("
        "'tool_holder_id',"
        "'index',"
        "'height', "
        "'upper_diameter',"
        "'lower_diameter'"
    ")"
    "values ("
        "?, "
        "?, "
        "?, "
        "?, "
        "?"
    ")";

const static QString UPDATE_TOOL_HOLDER_GEOMETRY_QUERY =
    "UPDATE 'tool_holder_geometry' SET "
        "'index'=?, "
        "'height'=?, "
        "'upper_diameter'=?, "
        "'lower_diameter'=?  "
    "WHERE id=?";

const static QString SELECT_TOOL_HOLDER_GEOMETRY_BY_TOOL_HOLDER_ID_QUERY =
    "SELECT * FROM 'tool_holder_geometry' WHERE tool_holder_id=?";

const static QString DELETE_TOOL_HOLDER_GEOMETRY_QUERY =
    "DELETE FROM 'tool_holder_geometry' WHERE id=?";
