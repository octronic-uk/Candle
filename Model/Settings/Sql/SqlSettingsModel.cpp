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
        createToolsTable();
        createToolGeometryTable();
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
    query.prepare(CREATE_TOOL_HOLDER_TABLE_QUERY);
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
    query.prepare(CREATE_TOOL_HOLDER_GEOMETRY_TABLE_QUERY);
    if (!query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }
    return true;
}

bool SqlSettingsModel::createToolsTable()
{
    qDebug() << "SqlSettingsModel:createToolsTable Creating table";
    QSqlQuery query;
    query.prepare(CREATE_TOOL_TABLE_QUERY);
    if (!query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }
    return true;
}

bool SqlSettingsModel::createToolGeometryTable()
{
    qDebug() << "SqlSettingsModel:createToolsGeometryTable Creating table";
    QSqlQuery query;
    query.prepare(CREATE_TOOL_GEOMETRY_TABLE_QUERY);
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

bool SqlSettingsModel::insertToolHolderInDB(ToolHolder* item)
{
    qDebug() << "SqlSettingsModel: insertToolHolderIntoDB";
    if (!currentProfileValid())
    {
        return false;
    }
    QSqlQuery query;
    query.prepare(INSERT_TOOL_HOLDER_QUERY);
    query.addBindValue(mCurrentProfileHandle->getID());
    query.addBindValue(item->getName());
    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error inserting tool holder "
                 << query.lastError();
        return false;
    }
    item->setID(query.lastInsertId().toInt());
    qDebug() << "SqlSettingsModel: Updated ToolHolder with id " << item->getID();
    return true;
}

bool SqlSettingsModel::updateToolHolderInDB(ToolHolder* toolHolder)
{
    qDebug() << "SqlSettingsModel::updateToolHolderInDB(ToolHolder& toolHolder)";
    if (!currentProfileValid() || !toolHolder->isIdValid())
    {
        return false;
    }
    QSqlQuery query;
    query.prepare(UPDATE_TOOL_HOLDER_QUERY);
    query.addBindValue(mCurrentProfileHandle->getID());
    query.addBindValue(toolHolder->getName());
    query.addBindValue(toolHolder->getID());

    qDebug() << "UpdateToolHolder"
             << toolHolder->getID()
             << toolHolder->getName()
             << "for"
             << mCurrentProfileHandle->getID();

    if(!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error updating tool holder"
                 << query.lastError();
        return false;
    }

    return true;
}

bool SqlSettingsModel::deleteToolHolderFromDB(ToolHolder* toolHolder)
{
    qDebug() << "SqlSettingsModel: deleteToolHolderFromDB";
    QSqlQuery query;
    query.prepare(DELETE_TOOL_HOLDER_QUERY);
    query.addBindValue(toolHolder->getID());
    return query.exec();
}

int SqlSettingsModel::getToolHoldersFromDB()
{
    qDebug() << "SqlSettingsModel::getToolHoldersFromDB()";
    // Remove existing
    if (mToolHoldersListModel == nullptr)
    {
        qDebug() << "SqlSettingsModel: Creating ToolHoldersListModel";
        mToolHoldersListModel = QSharedPointer<ToolHolderListModel>::create();

        connect
        (
            mToolHoldersListModel.data(),
            SIGNAL(toolHolderCreatedSignal(ToolHolder*)),
            this,
            SLOT(onToolHolderCreated(ToolHolder*))
        );

        connect
        (
            mToolHoldersListModel.data(),
            SIGNAL(toolHolderUpdatedSignal(ToolHolder*)),
            this,
            SLOT(onToolHolderUpdated(ToolHolder*))
        );

        connect
        (
            mToolHoldersListModel.data(),
            SIGNAL(toolHolderDeletedSignal(ToolHolder*)),
            this,
            SLOT(onToolHolderDeleted(ToolHolder*))
        );
    }
    else
    {
        mToolHoldersListModel->clear();
    }

    // Get form DB
    QSqlQuery query;
    query.prepare(SELECT_ALL_FROM_TOOL_HOLDER_BY_PROFILE_ID_QUERY);
    query.addBindValue(mCurrentProfileHandle->getID());
    query.exec();
    int idFieldNum = query.record().indexOf("id");
    int nameFieldNum = query.record().indexOf("name");
    // Populate
    QList<QSharedPointer<ToolHolder>> holders;
    while (query.next())
    {
        QString name = query.value(nameFieldNum).toString();
        int id = query.value(idFieldNum).toInt();
        auto th = QSharedPointer<ToolHolder>::create(id,name);
        holders.append(th);
        qDebug() << "SqlSettingsModel: Inserting ToolHolder into model"
                 << th->getID()
                 << th->getName();
    }
    mToolHoldersListModel->initialise(holders);
    emit toolHolderListModelReadySignal(mToolHoldersListModel.data());
    return mToolHoldersListModel->rowCount();
}

int SqlSettingsModel::getToolHoldersGeometryFromDB()
{
    int numRecords = 0;
    for (const QSharedPointer<ToolHolder>& nextHolder : mToolHoldersListModel->getAllData())
    {
        QSqlQuery query;
        query.prepare(SELECT_TOOL_HOLDER_GEOMETRY_BY_TOOL_HOLDER_ID_QUERY);
        query.addBindValue(nextHolder->getID());

        if (!query.exec())
        {
            qDebug() << "SqlSettingsModel: Error getting geometry" << query.lastError();
            continue;
        }

        int idFieldNum = query.record().indexOf("id");
        int toolHolderIdFieldNum = query.record().indexOf("tool_holder_id");
        int indexFieldNum = query.record().indexOf("index");
        int heightFieldNum = query.record().indexOf("height");
        int upperDiameterFieldNum = query.record().indexOf("upper_diameter");
        int lowerDiameterFieldNum = query.record().indexOf("lower_diameter");

        qDebug() << "SqlSettingsModel: ToolHolderGeometry"
                 << "tool_holder_id " << toolHolderIdFieldNum
                 << "index" << indexFieldNum
                 << "height" << heightFieldNum
                 << "upper_d" << upperDiameterFieldNum
                 << "lower_d" << lowerDiameterFieldNum;

        while(query.next())
        {
            qDebug() << "SqlSettingsModel: Got ToolHolderGeometry";
            int id = query.value(idFieldNum).toInt();
            int parentId = query.value(toolHolderIdFieldNum).toInt();
            int index = query.value(indexFieldNum).toInt();
            float height = query.value(heightFieldNum).toFloat();
            float upper = query.value(upperDiameterFieldNum).toFloat();
            float lower = query.value(lowerDiameterFieldNum).toFloat();
            QSharedPointer<ToolHolderGeometry> nextTHG =
                QSharedPointer<ToolHolderGeometry>::create
                    (id, parentId, index, height, upper, lower);
            nextHolder->insertGeometry(nextTHG);
        }
        numRecords += nextHolder->getGeometryTableModelHandle()->rowCount();
    }
    return numRecords;
}

bool SqlSettingsModel::insertToolHolderGeometryInDB(ToolHolderGeometry* toolHolder)
{
    qDebug() << "SqlSettingsModel: insertToolHolderGeometryInDB";
    QSqlQuery query;
    query.prepare(INSERT_TOOL_HOLDER_GEOMETRY_QUERY);
    query.addBindValue(toolHolder->getToolHolderID());
    query.addBindValue(toolHolder->getIndex());
    query.addBindValue(toolHolder->getHeight());
    query.addBindValue(toolHolder->getUpperDiameter());
    query.addBindValue(toolHolder->getLowerDiameter());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error inserting tool holder geometry"
                 << query.lastError();
        return false;
    }

    toolHolder->setID(query.lastInsertId().toInt());
    qDebug() << "SqlSettingsModel: Inserted tool holder geom with id" << toolHolder->getID();
    return true;
}

bool SqlSettingsModel::updateToolHolderGeometryInDB(ToolHolderGeometry* toolHolder)
{
    qDebug() << "SqlSettingsModel: updateToolHolderGeometryInDB with id"
             << toolHolder->getID();

    if (toolHolder->getID() < 0)
    {
        qDebug() << "SqlSettingsModel: Invalid ToolHolderGeometry ID ";
        return false;
    }

    QSqlQuery query;
    query.prepare(UPDATE_TOOL_HOLDER_GEOMETRY_QUERY);
    query.addBindValue(toolHolder->getIndex());
    query.addBindValue(toolHolder->getHeight());
    query.addBindValue(toolHolder->getUpperDiameter());
    query.addBindValue(toolHolder->getLowerDiameter());
    query.addBindValue(toolHolder->getID());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: updateToolHolderGeometry error" << query.lastError();
        return false;
    }

    return true;
}

bool SqlSettingsModel::deleteToolHolderGeometryFromDB(ToolHolderGeometry* toolHolder)
{
    qDebug() << "SqlSettingsModel: deleteToolHolderGeometryInDB";
    QSqlQuery query;
    query.prepare(DELETE_TOOL_HOLDER_GEOMETRY_QUERY);
    query.addBindValue(toolHolder->getID());
    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: deleteToolHolderGeometry error" << query.lastError();
        return false;
    }
    return true;
}

bool SqlSettingsModel::insertToolInDB(Tool* item)
{
    qDebug() << "SqlSettingsModel: insertToolIntoDB";
    if (!currentProfileValid())
    {
        return false;
    }
    QSqlQuery query;
    query.prepare(INSERT_TOOL_QUERY);
    query.addBindValue(mCurrentProfileHandle->getID());
    query.addBindValue(item->getName());
    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error inserting tool"
                 << query.lastError();
        return false;
    }
    item->setID(query.lastInsertId().toInt());
    qDebug() << "SqlSettingsModel: Updated Tool with id " << item->getID();
    return true;
}

bool SqlSettingsModel::updateToolInDB(Tool* tool)
{
    qDebug() << "SqlSettingsModel::updateToolInDB(Tool& tool)";
    if (!currentProfileValid() || !tool->isIdValid())
    {
        return false;
    }
    QSqlQuery query;
    query.prepare(UPDATE_TOOL_QUERY);
    query.addBindValue(mCurrentProfileHandle->getID());
    query.addBindValue(tool->getName());
    query.addBindValue(tool->getID());

    qDebug() << "UpdateTool"
             << tool->getID()
             << tool->getName()
             << "for"
             << mCurrentProfileHandle->getID();

    if(!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error updating tool"
                 << query.lastError();
        return false;
    }

    return true;
}

bool SqlSettingsModel::deleteToolFromDB(Tool* tool)
{
    qDebug() << "SqlSettingsModel: deleteToolFromDB";
    QSqlQuery query;
    query.prepare(DELETE_TOOL_QUERY);
    query.addBindValue(tool->getID());
    return query.exec();
}

int SqlSettingsModel::getToolsFromDB()
{
    qDebug() << "SqlSettingsModel::getToolsFromDB()";
    // Remove existing
    if (mToolsListModel == nullptr)
    {
        qDebug() << "SqlSettingsModel: Creating ToolsListModel";
        mToolsListModel = QSharedPointer<ToolListModel>::create();

        connect
        (
            mToolsListModel.data(),
            SIGNAL(toolCreatedSignal(Tool*)),
            this,
            SLOT(onToolCreated(Tool*))
        );

        connect
        (
            mToolsListModel.data(),
            SIGNAL(toolUpdatedSignal(Tool*)),
            this,
            SLOT(onToolUpdated(Tool*))
        );

        connect
        (
            mToolsListModel.data(),
            SIGNAL(toolDeletedSignal(Tool*)),
            this,
            SLOT(onToolDeleted(Tool*))
        );
    }
    else
    {
        mToolsListModel->clear();
    }

    // Get form DB
    QSqlQuery query;
    query.prepare(SELECT_ALL_FROM_TOOL_BY_PROFILE_ID_QUERY);
    query.addBindValue(mCurrentProfileHandle->getID());
    query.exec();

    int idFieldNum = query.record().indexOf("id");
    int nameFieldNum = query.record().indexOf("name");

    // Populate
    QList<QSharedPointer<Tool>> tools;
    while (query.next())
    {
        QString name = query.value(nameFieldNum).toString();
        int id = query.value(idFieldNum).toInt();
        auto tool = QSharedPointer<Tool>::create(id,name);
        tools.append(tool);
        qDebug() << "SqlSettingsModel: Inserting Tool into model"
                 << tool->getID()
                 << tool->getName();
    }
    mToolsListModel->initialise(tools);
    emit toolListModelReadySignal(mToolsListModel.data());
    return mToolsListModel->rowCount();
}

int SqlSettingsModel::getToolsGeometryFromDB()
{
    int numRecords = 0;
    for (const QSharedPointer<Tool>& next : mToolsListModel->getAllData())
    {
        QSqlQuery query;
        query.prepare(SELECT_TOOL_GEOMETRY_BY_TOOL_ID_QUERY);
        query.addBindValue(next->getID());

        if (!query.exec())
        {
            qDebug() << "SqlSettingsModel: Error getting geometry" << query.lastError();
            continue;
        }

        int idFieldNum = query.record().indexOf("id");
        int toolIdFieldNum = query.record().indexOf("tool_id");
        int indexFieldNum = query.record().indexOf("index");
        int heightFieldNum = query.record().indexOf("height");
        int upperDiameterFieldNum = query.record().indexOf("upper_diameter");
        int lowerDiameterFieldNum = query.record().indexOf("lower_diameter");

        qDebug() << "SqlSettingsModel: ToolGeometry"
                 << "tool_id " << toolIdFieldNum
                 << "index" << indexFieldNum
                 << "height" << heightFieldNum
                 << "upper_d" << upperDiameterFieldNum
                 << "lower_d" << lowerDiameterFieldNum;

        while(query.next())
        {
            qDebug() << "SqlSettingsModel: Got ToolGeometry";
            int id = query.value(idFieldNum).toInt();
            int parentId = query.value(toolIdFieldNum).toInt();
            int index = query.value(indexFieldNum).toInt();
            float height = query.value(heightFieldNum).toFloat();
            float upper = query.value(upperDiameterFieldNum).toFloat();
            float lower = query.value(lowerDiameterFieldNum).toFloat();
            QSharedPointer<ToolGeometry> nextTG =
                QSharedPointer<ToolGeometry>::create
                    (id, parentId, index, height, upper, lower);
            next->insertGeometry(nextTG);
        }
        numRecords += next->getGeometryTableModelHandle()->rowCount();
    }
    return numRecords;
}

bool SqlSettingsModel::insertToolGeometryInDB(ToolGeometry* tool)
{
    qDebug() << "SqlSettingsModel: insertToolGeometryInDB";
    QSqlQuery query;
    query.prepare(INSERT_TOOL_GEOMETRY_QUERY);
    query.addBindValue(tool->getToolID());
    query.addBindValue(tool->getIndex());
    query.addBindValue(tool->getHeight());
    query.addBindValue(tool->getUpperDiameter());
    query.addBindValue(tool->getLowerDiameter());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error inserting tool geometry"
                 << query.lastError();
        return false;
    }

    tool->setID(query.lastInsertId().toInt());
    qDebug() << "SqlSettingsModel: Inserted tool geom with id" << tool->getID();
    return true;
}

bool SqlSettingsModel::updateToolGeometryInDB(ToolGeometry* tool)
{
    qDebug() << "SqlSettingsModel: updateToolGeometryInDB with id"
             << tool->getID();

    if (tool->getID() < 0)
    {
        qDebug() << "SqlSettingsModel: Invalid ToolGeometry ID ";
        return false;
    }

    QSqlQuery query;
    query.prepare(UPDATE_TOOL_GEOMETRY_QUERY);
    query.addBindValue(tool->getIndex());
    query.addBindValue(tool->getHeight());
    query.addBindValue(tool->getUpperDiameter());
    query.addBindValue(tool->getLowerDiameter());
    query.addBindValue(tool->getID());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: updateToolGeometry error" << query.lastError();
        return false;
    }

    return true;
}

bool SqlSettingsModel::deleteToolGeometryFromDB(ToolGeometry* tool)
{
    qDebug() << "SqlSettingsModel: deleteToolGeometryInDB";
    QSqlQuery query;
    query.prepare(DELETE_TOOL_GEOMETRY_QUERY);
    query.addBindValue(tool->getID());
    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: deleteToolGeometry error" << query.lastError();
        return false;
    }
    return true;
}

int SqlSettingsModel::getProfilesFromDB()
{
    qDebug() << "SqlSettingsModel::getProfilesFromDB";
    mProfilesListModel = QSharedPointer<ProfilesListModel>::create();
    // Get form DB
    QSqlQuery query(SELECT_ALL_PROFILES_QUERY);
    int idFieldNum = query.record().indexOf("id");
    int nameFieldNum = query.record().indexOf("name");
    int selectedFieldNum = query.record().indexOf("selected");
    // Populate
    while (query.next())
    {
        int id = query.value(idFieldNum).toInt();
        QString name = query.value(nameFieldNum).toString();
        bool selected = query.value(selectedFieldNum).toBool();
        QSharedPointer<Profile> nextProfile = QSharedPointer<Profile>::create(id,name,selected);
        mProfilesListModel->insert(nextProfile);
        qDebug() << "SqlSettings: Added profile to model"
                 << nextProfile->getID()
                 << nextProfile->getName()
                 << nextProfile->getSelected();
    }
    emit profileListModelReadySignal(mProfilesListModel.data());
    return mProfilesListModel->rowCount(QModelIndex());
}

Profile* SqlSettingsModel::getCurrentProfileHandle()
{
    mCurrentProfileHandle = mProfilesListModel->getCurrentProfileHandle();
    qDebug() << "SqlSettingsModel: Setting Current Profile"
             << mCurrentProfileHandle->getID()
             << mCurrentProfileHandle->getName();
    if (currentProfileValid())
    {
        emit currentProfileChangedSignal(mCurrentProfileHandle);
    }
    return mCurrentProfileHandle;
}

bool SqlSettingsModel::updateProfileInDB(Profile* profile)
{
    qDebug() << "SqlSettingsModel::updateProfileInDB";
    QSqlQuery query;
    query.prepare(UPDATE_PROFILE_WHERE_ID_QUERY);
    query.addBindValue(profile->getName());
    query.addBindValue(profile->getSelected());
    query.addBindValue(profile->getID());
    return query.exec();
}

int SqlSettingsModel::getRecentGcodeFilesFromDB()
{
    qDebug() << "SqlSettingsModel::getRecentGcodeFilesFromDB";
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
    query.addBindValue(mCurrentProfileHandle->getID());
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
    emit recentGcodeFilesModelReadySignal(mRecentGcodeFilesModel.data());
    return mRecentGcodeFilesModel->count();
}

void SqlSettingsModel::onCurrentProfileChanged(Profile* profile)
{
    mCurrentProfileHandle = profile;
}

void SqlSettingsModel::onToolCreated(Tool* tool)
{
    qDebug() << "SqlSettingsModel::onToolCreated";
    insertToolInDB(tool);
}

void SqlSettingsModel::onToolUpdated(Tool* tool)
{
    qDebug() << "SqlSettingsModel::onToolUpdated";
    updateToolInDB(tool);
}

void SqlSettingsModel::onToolDeleted(Tool* tool)
{
    qDebug() << "SqlSettingsModel::onToolDeleted";
    deleteToolFromDB(tool);
}

void SqlSettingsModel::onToolHolderCreated(ToolHolder* toolHolder)
{
    qDebug() << "SqlSettingsModel::onToolHolderCreated";
    insertToolHolderInDB(toolHolder);
}

void SqlSettingsModel::onToolHolderUpdated(ToolHolder* toolHolder)
{
    qDebug() << "SqlSettingsModel::onToolHolderUpdated";
   updateToolHolderInDB(toolHolder);
}

void SqlSettingsModel::onToolHolderDeleted(ToolHolder* toolHolder)
{
    qDebug() << "SqlSettingsModel::onToolHolderDeleted";
    deleteToolHolderFromDB(toolHolder);
}

void SqlSettingsModel::onToolHolderGeometryCreated(ToolHolderGeometry* toolHolderGeometry)
{
    qDebug() << "SqlSettingsModel::onToolHolderGeometryCreated"
             << toolHolderGeometry->getID();
   insertToolHolderGeometryInDB(toolHolderGeometry);
}

void SqlSettingsModel::onToolHolderGeometryUpdated(ToolHolderGeometry* toolHolderGeometry)
{
    qDebug() << "SqlSettingsModel::onToolHolderGeometryUpdated"
             << toolHolderGeometry->getID();
   updateToolHolderGeometryInDB(toolHolderGeometry);
}

void SqlSettingsModel::onToolHolderGeometryDeleted(ToolHolderGeometry* toolHolderGeometry)
{
    qDebug() << "SqlSettingsModel::onToolHolderGeometryDeleted"
             << toolHolderGeometry->getID();
   deleteToolHolderGeometryFromDB(toolHolderGeometry);
}

void SqlSettingsModel::onToolGeometryCreated(ToolGeometry* toolGeometry)
{
    qDebug() << "SqlSettingsModel::onToolHolderGeometryCreated"
             << toolGeometry->getID();
   insertToolGeometryInDB(toolGeometry);
}

void SqlSettingsModel::onToolGeometryUpdated(ToolGeometry* toolGeometry)
{
    qDebug() << "SqlSettingsModel::onToolGeometryUpdated"
             << toolGeometry->getID();
   updateToolGeometryInDB(toolGeometry);
}

void SqlSettingsModel::onToolGeometryDeleted(ToolGeometry* toolGeometry)
{
    qDebug() << "SqlSettingsModel::onToolGeometryDeleted"
             << toolGeometry->getID();
   deleteToolGeometryFromDB(toolGeometry);
}
bool SqlSettingsModel::insertDefaultProfile()
{
    qDebug() << "SqlSettingsModel::insertDefaultProfile";
    QSqlQuery query;
    query.prepare(INSERT_DEFAULT_PROFILE_QUERY);
    return query.exec();
}

bool SqlSettingsModel::currentProfileValid()
{
    bool valid = QSqlDatabase::database().isOpen();
    if (!valid)
    {
       qDebug() << "SqlSettingsModel: ERROR database is not open";
    }

    valid = (mCurrentProfileHandle->getID() >= 0);

    if (!valid)
    {
       qDebug() << "SqlSettingsModel: ERROR current profile is invalid!";
    }
    return valid;
}

void SqlSettingsModel::onSaveSettings()
{

}

void SqlSettingsModel::onLoadSettings()
{
    getProfilesFromDB();
    getCurrentProfileHandle();
    getRecentGcodeFilesFromDB();
    getToolsFromDB();
    getToolsGeometryFromDB();
    getToolHoldersFromDB();
    getToolHoldersGeometryFromDB();
}

void SqlSettingsModel::onSettingChanged
(
    QString groupName,
    QString settingName,
    QVariant value
)
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
    "UPDATE 'profiles' SET name=?, selected=? WHERE id=?";

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

// Tools -------------------------------------------------------------------------
const QString SqlSettingsModel::CREATE_TOOL_TABLE_QUERY =
    "CREATE TABLE 'tool' ("
        "'id'           INTEGER PRIMARY KEY,"
        "'profile_id'   INTEGER NOT NULL,"
        "'name'         TEXT,"
        "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
    ")";

const QString SqlSettingsModel::INSERT_TOOL_QUERY =
    "INSERT INTO 'tool' (profile_id, name) values(?, ?)";

const QString SqlSettingsModel::SELECT_ALL_FROM_TOOL_BY_PROFILE_ID_QUERY =
    "SELECT * FROM 'tool' WHERE profile_id=?";

const QString SqlSettingsModel::SELECT_TOOL_BY_ID_QUERY =
    "SELECT * FROM 'tool' WHERE id=?";

const QString SqlSettingsModel::UPDATE_TOOL_QUERY =
    "UPDATE 'tool' SET profile_id=?, name=? WHERE id=?";

const QString SqlSettingsModel::DELETE_TOOL_QUERY =
    "DELETE FROM 'tool' WHERE id=?";

// Tool Geometry ---------------------------------------------------------

const QString SqlSettingsModel::CREATE_TOOL_GEOMETRY_TABLE_QUERY =
    "CREATE TABLE 'tool_geometry' ("
        "'id'             INTEGER PRIMARY KEY,"
        "'tool_id'        INTEGER NOT NULL,"
        "'index'          INTEGER NOT NULL,"
        "'height'         REAL NOT NULL,"
        "'upper_diameter' REAL NOT NULL,"
        "'lower_diameter' REAL NOT NULL,"
        "FOREIGN KEY('tool_id') REFERENCES tools('id')"
    ")";

const QString SqlSettingsModel::INSERT_TOOL_GEOMETRY_QUERY =
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

const QString SqlSettingsModel::UPDATE_TOOL_GEOMETRY_QUERY =
    "UPDATE 'tool_geometry' SET "
        "'index'=?, "
        "'height'=?, "
        "'upper_diameter'=?, "
        "'lower_diameter'=?  "
    "WHERE id=?";

const QString SqlSettingsModel::SELECT_TOOL_GEOMETRY_BY_TOOL_ID_QUERY =
    "SELECT * FROM 'tool_geometry' WHERE tool_id=?";

const QString SqlSettingsModel::DELETE_TOOL_GEOMETRY_QUERY =
    "DELETE FROM 'tool_geometry' WHERE id=?";


// Tool Holders ------------------------------------------------------------------
const QString SqlSettingsModel::CREATE_TOOL_HOLDER_TABLE_QUERY =
    "CREATE TABLE 'tool_holder' ("
        "'id'           INTEGER PRIMARY KEY,"
        "'profile_id'   INTEGER NOT NULL,"
        "'name'         TEXT,"
        "FOREIGN KEY('profile_id') REFERENCES profiles('id')"
    ")";

const QString SqlSettingsModel::INSERT_TOOL_HOLDER_QUERY =
    "INSERT INTO 'tool_holder' (profile_id, name) values(?, ?)";

const QString SqlSettingsModel::SELECT_ALL_FROM_TOOL_HOLDER_BY_PROFILE_ID_QUERY =
    "SELECT * FROM 'tool_holder' WHERE profile_id=?";

const QString SqlSettingsModel::SELECT_TOOL_HOLDER_BY_ID_QUERY =
    "SELECT * FROM 'tool_holder' WHERE id=?";

const QString SqlSettingsModel::UPDATE_TOOL_HOLDER_QUERY =
    "UPDATE 'tool_holder' SET profile_id=?, name=? WHERE id=?";

const QString SqlSettingsModel::DELETE_TOOL_HOLDER_QUERY =
    "DELETE FROM 'tool_holder' WHERE id=?";

// Tool Holders Geometry ---------------------------------------------------------

const QString SqlSettingsModel::CREATE_TOOL_HOLDER_GEOMETRY_TABLE_QUERY =
    "CREATE TABLE 'tool_holder_geometry' ("
        "'id'             INTEGER PRIMARY KEY,"
        "'tool_holder_id' INTEGER NOT NULL,"
        "'index'          INTEGER NOT NULL,"
        "'height'         REAL NOT NULL,"
        "'upper_diameter' REAL NOT NULL,"
        "'lower_diameter' REAL NOT NULL,"
        "FOREIGN KEY('tool_holder_id') REFERENCES tool_holder('id')"
    ")";

const QString SqlSettingsModel::INSERT_TOOL_HOLDER_GEOMETRY_QUERY =
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

const QString SqlSettingsModel::UPDATE_TOOL_HOLDER_GEOMETRY_QUERY =
    "UPDATE 'tool_holder_geometry' SET "
        "'index'=?, "
        "'height'=?, "
        "'upper_diameter'=?, "
        "'lower_diameter'=?  "
    "WHERE id=?";

const QString SqlSettingsModel::SELECT_TOOL_HOLDER_GEOMETRY_BY_TOOL_HOLDER_ID_QUERY =
    "SELECT * FROM 'tool_holder_geometry' WHERE tool_holder_id=?";

const QString SqlSettingsModel::DELETE_TOOL_HOLDER_GEOMETRY_QUERY =
    "DELETE FROM 'tool_holder_geometry' WHERE id=?";


