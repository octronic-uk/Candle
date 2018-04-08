/*
 * SqlSettingsModel.cpp
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
#include "SqlSettingsModel.h"
#include <QDir>
#include <QStandardPaths>
#include "Model/Settings/ToolHolder/ToolHolder.h"

SqlSettingsModel::SqlSettingsModel(QObject* parent)
    : AbstractSettingsModel(parent)
{
    mSettingsDirectory = QDir
    (
        QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0)
    );

    if (!mSettingsDirectory.exists())
    {
        QDir().mkdir(mSettingsDirectory.absolutePath());
    }

    QString mFilePath = mSettingsDirectory.filePath(DB_FILE_NAME);
    qDebug() << "SqlSettingsModel: Using DB" <<  mFilePath;

    QSqlDatabase db = QSqlDatabase::addDatabase(SQLITE_DB);
    db.setDatabaseName(mFilePath);
    db.open();
    if (db.isOpen())
    {
        qDebug() << "SqlSettingsModel: Database is open";
        createTables();
    }
    else
    {
        qDebug() << "SqlSettingsModel: Database is NOT open";
    }
}

SqlSettingsModel::~SqlSettingsModel()
{

}

void SqlSettingsModel::createTables()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (db.isOpen())
    {
        if (createProfilesTable())
        {
            insertDefaultProfile();
        }

        createRecentGcodeFilesTable();
        createRecentHeightMapFilesTable();
        createToolHoldersTable();
        createToolHoldersGeometryTable();
    }
}

bool SqlSettingsModel::createProfilesTable()
{
   qDebug() << "SqlSettingsModel:createProfilesTable Creating table";
   QSqlQuery query;
   query.prepare(CREATE_PROFILES_TABLE_QUERY);
   if (!query.exec())
   {
       qDebug() << query.lastError();
       return false;
   }
   return true;

}

bool SqlSettingsModel::createToolHoldersTable()
{
   qDebug() << "SqlSettingsModel:createToolHoldersTable Creating table";
   QSqlQuery query;
   query.prepare(CREATE_TOOL_HOLDERS_TABLE_QUERY);
   if (!query.exec())
   {
       qDebug() << query.lastError();
       return false;
   }
   return true;
}

bool SqlSettingsModel::createToolHoldersGeometryTable()
{
    qDebug() << "SqlSettingsModel:createToolHoldersGeometryTable Creating table";
    QSqlQuery query;
    query.prepare(CREATE_TOOL_HOLDERS_GEOMETRY_TABLE_QUERY);
    if (!query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }
    return true;
}

bool SqlSettingsModel::createRecentGcodeFilesTable()
{
    qDebug() << "SqlSettingsModel:createRecentGcodeFilesTable Creating table";
    QSqlQuery query;
    query.prepare(CREATE_RECENT_GCODE_FILES_TABLE_QUERY);
    if (!query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }
    return true;
}

bool SqlSettingsModel::createRecentHeightMapFilesTable()
{
    qDebug() << "SqlSettingsModel:createRecentHeightMapFilesTable Creating table";
    QSqlQuery query;
    query.prepare(CREATE_RECENT_HEIGHT_MAP_FILES_TABLE_QUERY);
    if (!query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }
    return true;
}

bool SqlSettingsModel::insertToolHolderInDB(const ToolHolder& item)
{
    QSqlQuery query;
    query.prepare(INSERT_TOOL_HOLDER_QUERY);
    query.addBindValue(item.getName());
    return query.exec();
}

int SqlSettingsModel::getProfilesFromDB()
{
    mProfilesListModel = QSharedPointer<ProfilesListModel>::create();
    // Get form DB
    QSqlQuery query(SELECT_ALL_PROFILES_QUERY);
    int idFieldNum = query.record().indexOf("id");
    int nameFieldNum = query.record().indexOf("name");
    // Populate
    while (query.next())
    {
       int id = query.value(idFieldNum).toInt();
       QString name = query.value(nameFieldNum).toString();
       Profile nextProfile(id,name);
       mProfilesListModel->insert(nextProfile);
    }
    emit profilesListModel_ListModelReady_Signal(mProfilesListModel);
    return mProfilesListModel->rowCount(QModelIndex());

}

bool SqlSettingsModel::updateProfileInDB(const Profile& profile)
{
    QSqlQuery query;
    query.prepare(UPDATE_PROFILE_WHERE_ID_QUERY);
    query.addBindValue(profile.getName());
    query.addBindValue(profile.getID());
    return query.exec();
}

int SqlSettingsModel::getRecentGcodeFilesFromDB()
{
    if (mRecentGcodeFilesModel == nullptr)
    {
        mRecentGcodeFilesModel = QSharedPointer<RecentFilesModel>::create();
    }
    else
    {
        mRecentGcodeFilesModel->clear();
    }

    // Get form DB
    QSqlQuery query;
    query.prepare(SELECT_RECENT_GCODE_FILES_BY_PROFILE_ID_QUERY);
    query.addBindValue(mCurrentProfile.getID());
    int idFieldNum = query.record().indexOf("id");
    int profileIdFieldNum = query.record().indexOf("profile_id");
    int indexFieldNum = query.record().indexOf("index");
    int pathFieldNum = query.record().indexOf("path");
    // Populate
    while (query.next())
    {
       int id = query.value(idFieldNum).toInt();
       int profile_id = query.value(profileIdFieldNum).toInt();
       int index = query.value(indexFieldNum).toInt();
       QString path = query.value(pathFieldNum).toString();
       mRecentGcodeFilesModel->add(RecentFile(id,index,profile_id,path));
    }
    emit recentGcodeFilesModel_ModelReady_Signal(mRecentGcodeFilesModel);
    return mRecentGcodeFilesModel->count();
}

int SqlSettingsModel::getToolHoldersFromDB()
{
    // Remove existing
    mToolHoldersListModel = QSharedPointer<ToolHolderModelListModel>::create();
    // Get form DB
    QSqlQuery query;
    query.prepare(SELECT_ALL_TOOL_HOLDERS_WHERE_PROFILE_ID_QUERY);
    query.addBindValue(mCurrentProfile.getID());
    int idFieldNum = query.record().indexOf("id");
    int nameFieldNum = query.record().indexOf("name");
    // Populate
    while (query.next())
    {
       QString name = query.value(nameFieldNum).toString();
       int id = query.value(idFieldNum).toInt();
       const QSharedPointer<ToolHolder> nextHolder =
               QSharedPointer<ToolHolder>::create(id,name);
       mToolHoldersListModel->insert(nextHolder);
    }
    emit toolHolderModel_ListModelReady_Signal(mToolHoldersListModel);
    return mToolHoldersListModel->rowCount();
}

int SqlSettingsModel::getToolHoldersGeometryFromDB()
{
    int numRecords = 0;

    for (QSharedPointer<ToolHolder> nextHolder : mToolHoldersListModel->getAllData())
    {
        QSqlQuery query;
        query.prepare(SELECT_TOOL_HOLDERS_GEOMETRY_BY_TOOL_HOLDER_ID_QUERY);
        query.addBindValue(nextHolder->getID());

        if (!query.exec())
           qDebug() << "SqlSettingsModel: Error getting geometry" << query.lastError();

        int idFieldNum = query.record().indexOf("id");
        int toolHolderIdFieldNum = query.record().indexOf("tool_holder_id");
        int indexFieldNum = query.record().indexOf("index");
        int heightFieldNum = query.record().indexOf("height");
        int upperDiameterFieldNum = query.record().indexOf("upper_diameter");
        int lowerDiameterFieldNum = query.record().indexOf("lower_diameter");

        qDebug() << "SqlSettingsModel: Indexes"
                << "tool_holder_id " << toolHolderIdFieldNum
                << ",index" << indexFieldNum
                << ",height" << heightFieldNum
                << ",upper_d" << upperDiameterFieldNum
                << ",lower_d" << lowerDiameterFieldNum;

        while(query.next())
        {
            qDebug() << "SqlSettingsModel: Inserting ToolHolder Model Geometry";
            int id = query.value(idFieldNum).toInt();
            int parentId = query.value(toolHolderIdFieldNum).toInt();
            int index = query.value(indexFieldNum).toInt();
            float height = query.value(heightFieldNum).toFloat();
            float upper = query.value(upperDiameterFieldNum).toFloat();
            float lower = query.value(lowerDiameterFieldNum).toFloat();
            nextHolder->insertGeometry
            (
                ToolHolderGeometry(id, parentId, index, height, upper, lower)
            );
        }
        numRecords += nextHolder->getTableModel()->rowCount();
    }
    return numRecords;
}

void SqlSettingsModel::onCurrentProfileChanged(Profile profile)
{
   mCurrentProfile = profile;
}

bool SqlSettingsModel::insertDefaultProfile()
{
    qDebug() << "SqlSettingsModel::insertDefaultProfile";
    QSqlQuery query;
    query.prepare(INSERT_DEFAULT_PROFILE_QUERY);
    return query.exec();
}

void SqlSettingsModel::onSaveSettings()
{

}

void SqlSettingsModel::onLoadSettings()
{
    getProfilesFromDB();
    getRecentGcodeFilesFromDB();
    getToolHoldersFromDB();
    getToolHoldersGeometryFromDB();
}

void SqlSettingsModel::onSettingChanged(QString groupName, QString settingName, QVariant value)
{

}

const QString SqlSettingsModel::SQLITE_DB = "QSQLITE";

const QString SqlSettingsModel::DB_FILE_NAME = "co.uk.octronic.cocoanut_cnc.sqlite";

// Profiles ----------------------------------------------------------------------

const QString SqlSettingsModel::CREATE_PROFILES_TABLE_QUERY =
"CREATE TABLE 'profiles' ("
    "'id'   INTEGER PRIMARY KEY,"
    "'name' TEXT NOT NULL,"
    "'selected' INTEGER NOT NULL"
")";

const QString SqlSettingsModel::INSERT_DEFAULT_PROFILE_QUERY =
"INSERT INTO 'profiles' ('name','selected') VALUES ('Default',1)";

const QString SqlSettingsModel::SELECT_ALL_PROFILES_QUERY =
"SELECT * FROM PROFILES";

const QString SqlSettingsModel::UPDATE_PROFILE_WHERE_ID_QUERY =
"UPDATE 'profiles' SET name=? WHERE id=?";

// Recent Gcode Files ------------------------------------------------------------
const QString SqlSettingsModel::CREATE_RECENT_GCODE_FILES_TABLE_QUERY =
"CREATE TABLE 'recent_gcode_files' ("
    "'id'           INTEGER PRIMARY KEY,"
    "'profile_id'   INTEGER NOT NULL,"
    "'index'        INTEGER NOT NULL,"
    "'path'         TEXT NOT NULL,"
    "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
")";

const QString SqlSettingsModel::SELECT_RECENT_GCODE_FILES_BY_PROFILE_ID_QUERY =
"SELECT * FROM 'recent_gcode_files WHERE profile_id=?";

// Recent HeightMap Files --------------------------------------------------------
const QString SqlSettingsModel::CREATE_RECENT_HEIGHT_MAP_FILES_TABLE_QUERY =
"CREATE TABLE 'recent_height_map_files' ("
    "'id'           INTEGER PRIMARY KEY,"
    "'profile_id'   INTEGER NOT NULL,"
    "'index'        INTEGER NOT NULL,"
    "'path'         TEXT NOT NULL,"
    "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
")";

const QString SqlSettingsModel::SELECT_RECENT_HEIGHT_MAP_FILES_BY_PROFILE_ID_QUERY =
"SELECT * FROM 'recent_height_map_files WHERE profile_id=?";


// Connection --------------------------------------------------------------------
const QString SqlSettingsModel::CREATE_CONNECTION_TABLE_QUERY =
"CREATE TABLE 'recent_gcode_files' ("
    "'id'           INTEGER PRIMARY KEY,"
    "'profile_id'   INTEGER NOT NULL,"
    "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
")";

// Interface ---------------------------------------------------------------------
const QString SqlSettingsModel::CREATE_INTERFACE_TABLE_QUERY =
"CREATE TABLE 'recent_gcode_files' ("
    "'id'           INTEGER PRIMARY KEY,"
    "'profile_id'   INTEGER NOT NULL,"
    "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
")";

// Machine -----------------------------------------------------------------------
const QString SqlSettingsModel::CREATE_MACHINE_TABLE_QUERY =
"CREATE TABLE 'recent_gcode_files' ("
    "'id'           INTEGER PRIMARY KEY,"
    "'profile_id'   INTEGER NOT NULL,"
    "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
")";

// Tool Holders ------------------------------------------------------------------
const QString SqlSettingsModel::CREATE_TOOL_HOLDERS_TABLE_QUERY =
"CREATE TABLE 'tool_holders' ("
    "'id'           INTEGER PRIMARY KEY,"
    "'profile_id'   INTEGER NOT NULL,"
    "'name'         TEXT,"
    "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
")";

const QString SqlSettingsModel::INSERT_TOOL_HOLDER_QUERY =
"INSERT INTO 'tool_holders' (name) values(?)";

const QString SqlSettingsModel::SELECT_ALL_TOOL_HOLDERS_WHERE_PROFILE_ID_QUERY =
"SELECT * FROM 'tool_holders' WHERE profile_id=?";

const QString SqlSettingsModel::SELECT_TOOL_HOLDER_BY_ID_QUERY =
"SELECT * FROM 'tool_holders' WHERE id=?";

const QString SqlSettingsModel::DELETE_TOOL_HOLDER_BY_ID_QUERY =
"DELETE FROM 'tool_holders' WHERE id=?";

// Tool Holders Geometry ---------------------------------------------------------

const QString SqlSettingsModel::CREATE_TOOL_HOLDERS_GEOMETRY_TABLE_QUERY =
"CREATE TABLE 'tool_holders_geometry' ("
    "'id'             INTEGER PRIMARY KEY,"
    "'tool_holder_id' INTEGER NOT NULL,"
    "'index'          INTEGER NOT NULL,"
    "'height'         REAL NOT NULL,"
    "'upper_diameter' REAL NOT NULL,"
    "'lower_diameter' REAL NOT NULL,"
    "FOREIGN KEY('tool_holder_id') REFERENCES tool_holders('id')"
")";

const QString SqlSettingsModel::INSERT_TOOL_HOLDERS_GEOMETRY_QUERY =
"insert into tool_holders_geometry ("
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

const QString SqlSettingsModel::SELECT_TOOL_HOLDERS_GEOMETRY_BY_TOOL_HOLDER_ID_QUERY =
"SELECT * FROM 'tool_holders_geometry' WHERE tool_holder_id=?";

const QString SqlSettingsModel::DELETE_TOOL_HOLDERS_GEOMETRY_BY_ID_QUERY =
"DELETE FROM 'tool_holders_geometry' WHERE id=?";


