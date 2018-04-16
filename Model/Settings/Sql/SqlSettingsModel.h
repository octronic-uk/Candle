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

    Profile* getCurrentProfileHandle();
    void setCurrentProfileHandle(Profile*);
    Profile* getProfileFromModelAtIndex(int);

    bool updateProfileName(Profile* profile, QString name);
    bool createNewProfile(QString);
    ProfilesListModel* getProfilesListModelHandle();
    ToolHolder* getToolHolderByID(int id);

signals:
    void settingsModelReadySignal(SqlSettingsModel*);
    void profileChangedSignal(Profile* p);

public slots:

    void onCurrentProfileChanged(Profile* profile);

    void onConnecitonSettingsUpdated(ConnectionSettings* settings);
    void onInterfaceSettingsUpdated(InterfaceSettings* settings);
    void onMachineSettingsUpdated(MachineSettings* settings);

    // Tools
    void onToolCreated();
    void onToolUpdated(Tool* tool);
    void onToolDeleted(Tool* tool);
    void onToolGeometryCreated();
    void onToolGeometryUpdated(ToolGeometry* toolGeometry);
    void onToolGeometryDeleted(ToolGeometry* toolGeometry);

    // Tool Holders
    void onToolHolderCreated();
    void onToolHolderUpdated(ToolHolder* toolHolder);
    void onToolHolderDeleted(ToolHolder* toolHolder);
    void onToolHolderGeometryCreated();
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
    bool insertProfileInDB(Profile* profile);
    bool updateProfileInDB(Profile* profile);
    bool deleteProfileFromDB(Profile* profile);
    bool profileValid(Profile* profile);
    // Connection
    bool createConnectionSettingsTable();
    int getConnectionSettingsFromDB(Profile* );
    bool insertConnectionSettingsInDB(ConnectionSettings* connection);
    bool updateConnectionSettingsInDB(ConnectionSettings* connection);
    bool deleteConnectionSettingsFromDB(ConnectionSettings* connection);
    // Interface
    bool createInterfaceSettingsTable();
    int getInterfaceSettingsFromDB(Profile* );
    bool insertInterfaceSettingsInDB(InterfaceSettings* settings);
    bool updateInterfaceSettingsInDB(InterfaceSettings* settings);
    bool deleteInterfaceSettingsFromDB(InterfaceSettings* settings);
    // Machine
    bool createMachineSettingsTable();
    int getMachineSettingsFromDB(Profile*);
    bool insertMachineSettingsInDB(MachineSettings* settings);
    bool updateMachineSettingsInDB(MachineSettings* settings);
    bool deleteMachineSettingsFromDB(MachineSettings* settings);
    // Recent Gcode Files
    bool createRecentGcodeFilesTable();
    int getRecentGcodeFilesFromDB(Profile*);
    bool insertRecentGcodeFileInDB();
    // Recent HeightMap Files
    bool createRecentHeightMapFilesTable();
    // Tool
    bool createToolsTable();
    int getToolsFromDB(Profile*);
    bool insertToolInDB(Tool* tool);
    bool updateToolInDB(Tool* tool);
    bool deleteToolFromDB(Tool* tool);
    // Tool Geometry
    int getToolGeometryFromDB(Profile*);
    bool createToolGeometryTable();
    bool insertToolGeometryInDB(ToolGeometry* tool);
    bool updateToolGeometryInDB(ToolGeometry* tool);
    bool deleteToolGeometryFromDB(ToolGeometry* tool);
    // Tool Holder
    bool createToolHoldersTable();
    int getToolHoldersFromDB(Profile*);
    bool insertToolHolderInDB(ToolHolder* toolHolder);
    bool updateToolHolderInDB(ToolHolder* toolHolder);
    bool deleteToolHolderFromDB(ToolHolder* toolHolder);
    // Tool Holder Geometry
    bool createToolHoldersGeometryTable();
    int getToolHoldersGeometryFromDB(Profile*);
    bool insertToolHolderGeometryInDB(ToolHolderGeometry* toolHolder);
    bool updateToolHolderGeometryInDB(ToolHolderGeometry* toolHolder);
    bool deleteToolHolderGeometryFromDB(ToolHolderGeometry* toolHolder);

private: // Members
    QDir mSettingsDirectory;
    QString mFilePath;
    QSharedPointer<ProfilesListModel> mProfilesListModel;

    bool createNewProfile(Profile*);
};
