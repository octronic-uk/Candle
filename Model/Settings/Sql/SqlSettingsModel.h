/*
 * SqlSettingsModel.h
 *
 * Created: 07 2018 by Ashley
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

#include <QtSql>

#include "Model/Settings/AbstractSettingsModel.h"
#include "Model/RecentFilesModel.h"
// Tool
#include "Model/Settings/Tool/Tool.h"
#include "Model/Settings/Tool/ToolListModel.h"
// Tool Holder
#include "Model/Settings/ToolHolder/ToolHolder.h"
#include "Model/Settings/ToolHolder/ToolHolderListModel.h"
// Profiles
#include "Model/Settings/Profile/ProfilesListModel.h"

class SqlSettingsModel : public AbstractSettingsModel
{
    Q_OBJECT
public:
    SqlSettingsModel(QObject* parent = nullptr);
    ~SqlSettingsModel() override;

    bool getOpen() const;
    void setOpen(bool open);

signals:
    void profileListModelReadySignal(ProfilesListModel*);
    void recentGcodeFilesModelReadySignal(RecentFilesModel*);
    void recentHeightMapFilesModelReadySignal(RecentFilesModel*);
    void toolHolderListModelReadySignal(ToolHolderListModel*);
    void toolListModelReadySignal(ToolListModel*);
    void currentProfileChangedSignal(Profile* p);

public slots:
    void onCurrentProfileChanged(Profile* profile);
    // Tools
    void onToolCreated(Tool* tool);
    void onToolUpdated(Tool* tool);
    void onToolDeleted(Tool* tool);
    void onToolGeometryCreated(ToolGeometry* toolGeometry);
    void onToolGeometryUpdated(ToolGeometry* toolGeometry);
    void onToolGeometryDeleted(ToolGeometry* toolGeometry);

    // Tool Holders
    void onToolHolderCreated(ToolHolder* toolHolder);
    void onToolHolderUpdated(ToolHolder* toolHolder);
    void onToolHolderDeleted(ToolHolder* toolHolder);
    void onToolHolderGeometryCreated(ToolHolderGeometry* toolHolderGeometry);
    void onToolHolderGeometryUpdated(ToolHolderGeometry* toolHolderGeometry);
    void onToolHolderGeometryDeleted(ToolHolderGeometry* toolHolderGeometry);

    void onSaveSettings() override;
    void onLoadSettings() override;
    void onSettingChanged(QString groupName, QString settingName, QVariant value) override;

protected:
    void createTables();
    // Profile
    bool createProfilesTable();
    int getProfilesFromDB();
    bool updateProfileInDB(Profile* profile);
    bool currentProfileValid();
    bool insertDefaultProfile();
    Profile* getCurrentProfileHandle();
    // Recent Gcode Files
    bool createRecentGcodeFilesTable();
    int getRecentGcodeFilesFromDB();
    bool insertRecentGcodeFileInDB();
    // Recent HeightMap Files
    bool createRecentHeightMapFilesTable();
    // Tool
    int getToolsFromDB();
    bool insertToolInDB(Tool* tool);
    bool updateToolInDB(Tool* tool);
    bool deleteToolFromDB(Tool* tool);
    bool createToolsTable();
    bool createToolGeometryTable();
    // Tool Geometry
    int getToolsGeometryFromDB();
    bool insertToolGeometryInDB(ToolGeometry* tool);
    bool updateToolGeometryInDB(ToolGeometry* tool);
    bool deleteToolGeometryFromDB(ToolGeometry* tool);
    // Tool Holder
    bool createToolHoldersTable();
    int getToolHoldersFromDB();
    bool insertToolHolderInDB(ToolHolder* toolHolder);
    bool updateToolHolderInDB(ToolHolder* toolHolder);
    bool deleteToolHolderFromDB(ToolHolder* toolHolder);
    // Tool Holder Geometry
    bool createToolHoldersGeometryTable();
    int getToolHoldersGeometryFromDB();
    bool insertToolHolderGeometryInDB(ToolHolderGeometry* toolHolder);
    bool updateToolHolderGeometryInDB(ToolHolderGeometry* toolHolder);
    bool deleteToolHolderGeometryFromDB(ToolHolderGeometry* toolHolder);

private: // Members
    QDir mSettingsDirectory;
    QString mFilePath;
    Profile* mCurrentProfileHandle;
    QSharedPointer<ToolListModel> mToolsListModel;
    QSharedPointer<ToolHolderListModel> mToolHoldersListModel;
    QSharedPointer<ProfilesListModel> mProfilesListModel;
    QSharedPointer<RecentFilesModel> mRecentGcodeFilesModel;
    QSharedPointer<RecentFilesModel> mRecentHeightMapFilesModel;

private: // Member Functions



private: // Query Strings
    const static QString DB_FILE_NAME;
    const static QString SQLITE_DB;

    // Profiles
    const static QString CREATE_PROFILES_TABLE_QUERY;
    const static QString INSERT_DEFAULT_PROFILE_QUERY;
    const static QString SELECT_ALL_PROFILES_QUERY;
    const static QString UPDATE_PROFILE_WHERE_ID_QUERY;

    // Recent Files
    const static QString CREATE_RECENT_GCODE_FILES_TABLE_QUERY;
    const static QString SELECT_RECENT_GCODE_FILES_BY_PROFILE_ID_QUERY;

    // Recent HeightMep Files
    const static QString CREATE_RECENT_HEIGHT_MAP_FILES_TABLE_QUERY;
    const static QString SELECT_RECENT_HEIGHT_MAP_FILES_BY_PROFILE_ID_QUERY;

    // Connection
    const static QString CREATE_CONNECTION_TABLE_QUERY;

    // Interface
    const static QString CREATE_INTERFACE_TABLE_QUERY;

    // Machine
    const static QString CREATE_MACHINE_TABLE_QUERY;

    // Tools
    const static QString CREATE_TOOL_TABLE_QUERY;
    const static QString INSERT_TOOL_QUERY;
    const static QString SELECT_TOOL_BY_ID_QUERY;
    const static QString SELECT_ALL_FROM_TOOL_BY_PROFILE_ID_QUERY;
    const static QString UPDATE_TOOL_QUERY;
    const static QString DELETE_TOOL_QUERY;

    // Tool Holders Geometry
    const static QString CREATE_TOOL_GEOMETRY_TABLE_QUERY;
    const static QString INSERT_TOOL_GEOMETRY_QUERY;
    const static QString UPDATE_TOOL_GEOMETRY_QUERY;
    const static QString SELECT_TOOL_GEOMETRY_BY_TOOL_ID_QUERY;
    const static QString DELETE_TOOL_GEOMETRY_QUERY;

    // Tool Holders
    const static QString CREATE_TOOL_HOLDER_TABLE_QUERY;
    const static QString INSERT_TOOL_HOLDER_QUERY;
    const static QString SELECT_TOOL_HOLDER_BY_ID_QUERY;
    const static QString SELECT_ALL_FROM_TOOL_HOLDER_BY_PROFILE_ID_QUERY;
    const static QString UPDATE_TOOL_HOLDER_QUERY;
    const static QString DELETE_TOOL_HOLDER_QUERY;

    // Tool Holders Geometry
    const static QString CREATE_TOOL_HOLDER_GEOMETRY_TABLE_QUERY;
    const static QString INSERT_TOOL_HOLDER_GEOMETRY_QUERY;
    const static QString UPDATE_TOOL_HOLDER_GEOMETRY_QUERY;
    const static QString SELECT_TOOL_HOLDER_GEOMETRY_BY_TOOL_HOLDER_ID_QUERY;
    const static QString DELETE_TOOL_HOLDER_GEOMETRY_QUERY;

};
