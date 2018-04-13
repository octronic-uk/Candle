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
#include "Model/Settings/Sql/SqlStrings.h"
#include "Model/Settings/ToolHolder/ToolHolderGeometry.h"


SqlSettingsModel::SqlSettingsModel(QObject* parent)
    : AbstractSettingsModel(parent)
{
    mProfilesListModel = QSharedPointer<ProfilesListModel>::create(this);
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
        bool firstRun = createProfilesTable();
        createConnectionSettingsTable();
        createInterfaceSettingsTable();
        createMachineSettingsTable();
        createRecentGcodeFilesTable();
        createRecentHeightMapFilesTable();
        createToolsTable();
        createToolGeometryTable();
        createToolHoldersTable();
        createToolHoldersGeometryTable();

        if (firstRun)
        {
            Profile *defaultProfile = mProfilesListModel->createNewProfile();
            defaultProfile->setSelected(true);
            defaultProfile->setName("Default Profile");
            createNewProfile(defaultProfile);
        }
    }
}

bool SqlSettingsModel::createNewProfile(QString name)
{
    Profile* profile = mProfilesListModel->createNewProfile();
    profile->setName(name);
    return createNewProfile(profile);
}

ProfilesListModel* SqlSettingsModel::getProfilesListModelHandle()
{
    return mProfilesListModel.data();
}

bool SqlSettingsModel::createNewProfile(Profile* profile)
{
    if (!insertProfileInDB(profile))
    {
        qDebug() << "SqlSettingsModel: CreateNewProfile Failed to insert new profile";
        return false;
    }

    mProfilesListModel->setSelected(profile);

    if(!insertConnectionSettingsInDB(profile->getConnectionSettingsHandle()))
    {
        qDebug() << "SqlSettingsModel: CreateNewProfile Failed to insert connection settings";
        return false;
    }

    if(!insertInterfaceSettingsInDB(profile->getInterfaceSettingsHandle()))
    {
        qDebug() << "SqlSettingsModel: CreateNewProfile Failed to insert interface settings";
        return false;
    }

    if(!insertMachineSettingsInDB(profile->getMachineSettingsHandle()))
    {
        qDebug() << "SqlSettingsModel: CreateNewProfile Failed to insert machine settings";
        return false;
    }

    return true;
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
    Profile* profile = item->getParentHandle();
    if (!profileValid(profile))
    {
        return false;
    }
    QSqlQuery query;
    query.prepare(INSERT_TOOL_HOLDER_QUERY);
    query.addBindValue(profile->getID());
    query.addBindValue(item->getName());
    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error inserting tool holder "
                 << query.lastError();
        return false;
    }
    item->setID(query.lastInsertId().toInt());
    qDebug() << "SqlSettingsModel: Successfully inserted and updated ToolHolder to id " << item->getID();
    return true;
}

bool SqlSettingsModel::updateToolHolderInDB(ToolHolder* toolHolder)
{
    qDebug() << "SqlSettingsModel::updateToolHolderInDB(ToolHolder& toolHolder)";
    Profile* profile = toolHolder->getParentHandle();
    if (!profileValid(profile) || !toolHolder->isIdValid())
    {
        return false;
    }
    QSqlQuery query;
    query.prepare(UPDATE_TOOL_HOLDER_QUERY);
    query.addBindValue(profile->getID());
    query.addBindValue(toolHolder->getName());
    query.addBindValue(toolHolder->getID());

    qDebug() << "UpdateToolHolder"
             << toolHolder->getID()
             << toolHolder->getName()
             << "for"
             << profile->getID();

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

int SqlSettingsModel::getToolHoldersFromDB(Profile* profile)
{
    qDebug() << "SqlSettingsModel::getToolHoldersFromDB()";
    // Remove existing
    ToolHolderListModel* modelHandle = profile->getToolHolderListModelHandle();
    if (modelHandle == nullptr)
    {
        qDebug() << "SqlSettingsModel: Creating ToolHoldersListModel";

        connect
                (
                    modelHandle,
                    SIGNAL(toolHolderCreatedSignal(ToolHolder*)),
                    this,
                    SLOT(onToolHolderCreated(ToolHolder*))
                    );

        connect
                (
                    modelHandle,
                    SIGNAL(toolHolderUpdatedSignal(ToolHolder*)),
                    this,
                    SLOT(onToolHolderUpdated(ToolHolder*))
                    );

        connect
                (
                    modelHandle,
                    SIGNAL(toolHolderDeletedSignal(ToolHolder*)),
                    this,
                    SLOT(onToolHolderDeleted(ToolHolder*))
                    );
    }
    else
    {
        modelHandle->clear();
    }

    // Get form DB
    QSqlQuery query;
    query.prepare(SELECT_ALL_FROM_TOOL_HOLDER_BY_PROFILE_ID_QUERY);
    query.addBindValue(profile->getID());
    query.exec();
    int idFieldNum = query.record().indexOf("id");
    int nameFieldNum = query.record().indexOf("name");
    // Populate
    QList<QSharedPointer<ToolHolder>> holders;
    while (query.next())
    {
        QString name = query.value(nameFieldNum).toString();
        int id = query.value(idFieldNum).toInt();
        auto th = QSharedPointer<ToolHolder>::create(profile,id,name);
        holders.append(th);
        qDebug() << "SqlSettingsModel: Inserting ToolHolder into model"
                 << th->getID()
                 << th->getName();
    }
    modelHandle->initialise(holders);
    return modelHandle->rowCount();
}

int SqlSettingsModel::getToolHoldersGeometryFromDB(Profile* profile)
{
    int numRecords = 0;
    for (const QSharedPointer<ToolHolder>& nextHolder : profile->getToolHolderListModelHandle()->getAllData())
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
            ToolHolder* parent = profile->getToolHolderListModelHandle()->getToolHolderByID(parentId);
            QSharedPointer<ToolHolderGeometry> nextTHG =
                    QSharedPointer<ToolHolderGeometry>::create
                    (parent, id, index, height, upper, lower);
            nextHolder->insertItem(nextTHG);
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
    Profile *profile = item->getParentHandle();
    if (!profileValid(profile))
    {
        return false;
    }

    QSqlQuery query;
    query.prepare(INSERT_TOOL_QUERY);
    query.addBindValue(profile->getID());
    query.addBindValue(item->getName());
    query.addBindValue(item->getToolNumber());
    query.addBindValue(item->getToolHolderID());

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
    qDebug() << "SqlSettingsModel::updateToolInDB";

    Profile *profile = tool->getParentHandle();
    if (!profileValid(profile) || !tool->isIdValid())
    {
        return false;
    }

    QSqlQuery query;

    query.prepare(UPDATE_TOOL_QUERY);
    query.addBindValue(profile->getID());
    query.addBindValue(tool->getName());
    query.addBindValue(tool->getToolNumber());
    query.addBindValue(tool->getToolHolderID());
    query.addBindValue(tool->getID());

    qDebug() << "SqlSettingsModel: UpdateTool Bind Values"
             << tool->getID()
             << tool->getName()
             << tool->getToolNumber()
             << tool->getToolHolderID()
             << "for"
             << profile->getID();

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

int SqlSettingsModel::getToolsFromDB(Profile* profile)
{
    qDebug() << "SqlSettingsModel::getToolsFromDB()";
    // Remove existing
    ToolListModel* modelHandle = profile->getToolListModelHandle();
    if (modelHandle == nullptr)
    {
        qDebug() << "SqlSettingsModel: Creating ToolsListModel";
        //modelHandle = QSharedPointer<ToolListModel>::create();

        connect
                (
                    modelHandle,
                    SIGNAL(toolCreatedSignal(Tool*)),
                    this,
                    SLOT(onToolCreated(Tool*))
                    );

        connect
                (
                    modelHandle,
                    SIGNAL(toolUpdatedSignal(Tool*)),
                    this,
                    SLOT(onToolUpdated(Tool*))
                    );

        connect
                (
                    modelHandle,
                    SIGNAL(toolDeletedSignal(Tool*)),
                    this,
                    SLOT(onToolDeleted(Tool*))
                    );
    }
    else
    {
        modelHandle->clear();
    }

    // Get form DB
    QSqlQuery query;
    query.prepare(SELECT_ALL_FROM_TOOL_BY_PROFILE_ID_QUERY);
    query.addBindValue(profile->getID());
    query.exec();

    int idFieldNum = query.record().indexOf("id");
    int nameFieldNum = query.record().indexOf("name");
    int toolNumberFieldNum = query.record().indexOf("tool_number");
    int toolHolderIDFieldNum = query.record().indexOf("tool_holder_id");

    // Populate
    QList<QSharedPointer<Tool>> tools;
    while (query.next())
    {
        QString name = query.value(nameFieldNum).toString();
        int id = query.value(idFieldNum).toInt();
        int toolNumber = query.value(toolNumberFieldNum).toInt();
        int toolHolderID = query.value(toolHolderIDFieldNum).toInt();

        auto tool = QSharedPointer<Tool>::create(profile, id,name,toolNumber,toolHolderID);
        tools.append(tool);
        qDebug() << "SqlSettingsModel: Inserting Tool into model"
                 << tool->getID()
                 << tool->getName()
                 << tool->getToolNumber()
                 << tool->getToolHolderID();
    }
    modelHandle->initialise(tools);
    return modelHandle->rowCount();
}

int SqlSettingsModel::getToolGeometryFromDB(Profile* profile)
{
    int numRecords = 0;
    ToolListModel* modelHandle = profile->getToolListModelHandle();
    for (const QSharedPointer<Tool>& next : modelHandle->getAllData())
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
            int index = query.value(indexFieldNum).toInt();
            float height = query.value(heightFieldNum).toFloat();
            float upper = query.value(upperDiameterFieldNum).toFloat();
            float lower = query.value(lowerDiameterFieldNum).toFloat();
            QSharedPointer<ToolGeometry> nextTG =
                    QSharedPointer<ToolGeometry>::create
                    (next.data(), id, index, height, upper, lower);
            next->insertItem(nextTG);
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

    // Sync settings data for each profile

    for (auto p : mProfilesListModel->getDataHandles())
    {
        getConnectionSettingsFromDB(p);
        getRecentGcodeFilesFromDB(p);
        getToolsFromDB(p);
        getToolGeometryFromDB(p);
        getToolHoldersFromDB(p);
        getToolHoldersGeometryFromDB(p);
    }
    return mProfilesListModel->rowCount(QModelIndex());
}

Profile* SqlSettingsModel::getCurrentProfileHandle()
{
    Profile* profile = mProfilesListModel->getCurrentProfileHandle();
    if (!profile)
    {
        return nullptr;
    }

    if (profileValid(profile))
    {
        emit profileChangedSignal(profile);
    }

    return profile;
}

void SqlSettingsModel::setCurrentProfileHandle(Profile* profile)
{
    mProfilesListModel->setSelected(profile);
    for (Profile *p : mProfilesListModel->getDataHandles())
    {
        updateProfileInDB(p);
    }
}

Profile* SqlSettingsModel::getProfileFromModelAtIndex(int index)
{
    return mProfilesListModel->get(index);
}

bool SqlSettingsModel::createInterfaceSettingsTable()
{
    qDebug() << "SqlSettingsModel:createInterfaceTable Creating table";
    QSqlQuery query;
    query.prepare(CREATE_INTERFACE_TABLE_QUERY);
    if (!query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }
    return true;
}

int SqlSettingsModel::getInterfaceSettingsFromDB(Profile* profile)
{
    int numRecords = 0;
    QSqlQuery query;
    query.prepare(SELECT_FROM_INTERFACE_QUERY);
    query.addBindValue(profile->getID());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error getting interface settings"
                 << query.lastError();
        return 0;
    }

    int idFieldNum = query.record().indexOf("id");
    int lineWidthFieldNum = query.record().indexOf("line_width");
    int fpsLockFieldNum = query.record().indexOf("fps_lock");
    int antiAliasingFieldNum = query.record().indexOf("anti_aliasing");
    int vSyncFieldNum = query.record().indexOf("v_sync");
    int msaaFieldNum = query.record().indexOf("msaa");
    int zBufferFieldNum = query.record().indexOf("z_buffer");
    int gcodeDrawModeFieldNum = query.record().indexOf("gcode_draw_mode");
    int simplifyGeomFieldNum = query.record().indexOf("simplify_geometry");
    int simplifyPrecisionFieldNum = query.record().indexOf("simplify_precision");
    int grayscaleSegmentsFieldNum = query.record().indexOf("grayscale_segments");
    int grayscaleCodeFieldNum = query.record().indexOf("grayscale_code");

    while(query.next())
    {
        qDebug() << "SqlSettingsModel: Got Interface settings for" << profile->getID();
        int id = query.value(idFieldNum).toInt();
        float lineWidth = query.value(lineWidthFieldNum).toFloat();
        int fpsLock = query.value(fpsLockFieldNum).toInt();
        bool antiAliasing = query.value(antiAliasingFieldNum).toBool();
        bool vSync = query.value(vSyncFieldNum).toBool();
        bool msaa = query.value(msaaFieldNum).toBool();
        bool zBuffer = query.value(zBufferFieldNum).toBool();
        bool gcodeDrawMode = query.value(gcodeDrawModeFieldNum).toBool();
        bool simplifyGeom = query.value(simplifyGeomFieldNum).toBool();
        float simplifyPrecision = query.value(simplifyPrecisionFieldNum).toFloat();
        int grayscaleSegments = query.value(grayscaleSegmentsFieldNum).toInt();
        bool grayscaleCode = query.value(grayscaleCodeFieldNum).toBool();

        InterfaceSettings* iface = profile->getInterfaceSettingsHandle();
        iface->setID(id);
        iface->setLineWidth(lineWidth);
        iface->setFpsLock(fpsLock);
        iface->setAntiAliasing(antiAliasing);
        iface->setVsync(vSync);
        iface->setMsaa(msaa);
        iface->setZbuffer(zBuffer);
        iface->setGcodeDrawMode(gcodeDrawMode);
        iface->setSimplifyGeometry(simplifyGeom);
        iface->setSimplifyPrecision(simplifyPrecision);
        iface->setGrayscaleSegments(grayscaleSegments);
        iface->setGrayscaleCode(grayscaleCode);
    }
    numRecords++;
    return numRecords;
}

bool SqlSettingsModel::insertInterfaceSettingsInDB(InterfaceSettings* settings)
{
    qDebug() << "SqlSettingsModel: insertConnectionInDB";
    QSqlQuery query;
    query.prepare(INSERT_INTERFACE_QUERY);

    query.addBindValue(settings->getParentID());
    query.addBindValue(settings->getLineWidth());
    query.addBindValue(settings->getFpsLock());
    query.addBindValue(settings->getAntiAliasing());
    query.addBindValue(settings->getVsync());
    query.addBindValue(settings->getMsaa());
    query.addBindValue(settings->getZbuffer());
    query.addBindValue(settings->getGcodeDrawMode());
    query.addBindValue(settings->getSimplifyGeometry());
    query.addBindValue(settings->getSimplifyPrecision());
    query.addBindValue(settings->getGrayscaleSegments());
    query.addBindValue(settings->getGrayscaleCode());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error inserting interface"
                 << query.lastError();
        return false;
    }

    settings->setID(query.lastInsertId().toInt());

    qDebug() << "SqlSettingsModel: Inserted interface with id"
             << settings->getID();

    return true;
}

bool SqlSettingsModel::updateInterfaceSettingsInDB(InterfaceSettings* settings)
{
    qDebug() << "SqlSettingsModel: updateConnectionInDB";
    QSqlQuery query;
    Profile* profile = settings->getParentHandle();
    query.prepare(UPDATE_INTERFACE_QUERY);
    query.addBindValue(profile->getID());
    query.addBindValue(settings->getLineWidth());
    query.addBindValue(settings->getFpsLock());
    query.addBindValue(settings->getAntiAliasing());
    query.addBindValue(settings->getVsync());
    query.addBindValue(settings->getMsaa());
    query.addBindValue(settings->getZbuffer());
    query.addBindValue(settings->getGcodeDrawMode());
    query.addBindValue(settings->getSimplifyGeometry());
    query.addBindValue(settings->getSimplifyPrecision());
    query.addBindValue(settings->getGrayscaleSegments());
    query.addBindValue(settings->getGrayscaleCode());
    query.addBindValue(settings->getID());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error updating interface"
                 << query.lastError();
        return false;
    }

    qDebug() << "SqlSettingsModel: Updated interface with id"
             << settings->getID();
    return true;
}

bool SqlSettingsModel::deleteInterfaceSettingsFromDB(InterfaceSettings* settings)
{
    qDebug() << "SqlSettingsModel: deleteInterfaceFromDB";
    QSqlQuery query;
    query.prepare(DELETE_INTERFACE_QUERY);
    query.addBindValue(settings->getID());
    return query.exec();
}

bool SqlSettingsModel::createMachineSettingsTable()
{
    qDebug() << "SqlSettingsModel:creatMachineTable";
    QSqlQuery query;
    query.prepare(CREATE_MACHINE_TABLE_QUERY);
    if (!query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }
    return true;
}

int SqlSettingsModel::getMachineSettingsFromDB(Profile* profile)
{
    int numRecords = 0;
    QSqlQuery query;
    query.prepare(SELECT_FROM_MACHINE_QUERY);
    query.addBindValue(profile->getID());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error getting machine settings"
                 << query.lastError();
        return 0;
    }

    int idFieldNum = query.record().indexOf("id");


    while(query.next())
    {
        qDebug() << "SqlSettingsModel: Got Interface settings for" << profile->getID();
        int id = query.value(idFieldNum).toInt();

        MachineSettings* machine = profile->getMachineSettingsHandle();
        machine->setID(id);

    }
    numRecords++;
    return numRecords;
}

bool SqlSettingsModel::insertMachineSettingsInDB(MachineSettings* settings)
{
    qDebug() << "SqlSettingsModel: insertMachineInDB";
    QSqlQuery query;

    query.prepare(INSERT_MACHINE_QUERY);

    query.addBindValue(settings->getParentID());
    query.addBindValue(settings->getQueryPeriod());
    query.addBindValue(settings->getUnits());
    query.addBindValue(settings->getRapidSpeed());
    query.addBindValue(settings->getAcceleration());
    query.addBindValue(settings->getSpindleMin());
    query.addBindValue(settings->getSpindleMax());
    query.addBindValue(settings->getProbeCmds());
    query.addBindValue(settings->getSafePositionCmds());
    query.addBindValue(settings->getRestoreOrigin());
    query.addBindValue(settings->getRestoreType());
    query.addBindValue(settings->getUserCmd1());
    query.addBindValue(settings->getUserCmd2());
    query.addBindValue(settings->getUserCmd3());
    query.addBindValue(settings->getUserCmd4());
    query.addBindValue(settings->getHeightMapProbeFeed());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error inserting machine"
                 << query.lastError();
        return false;
    }

    settings->setID(query.lastInsertId().toInt());

    qDebug() << "SqlSettingsModel: Inserted machine with id"
             << settings->getID();

    return true;
}

bool SqlSettingsModel::updateMachineSettingsInDB(MachineSettings* settings)
{
    qDebug() << "SqlSettingsModel: updateMachineInDB";
    QSqlQuery query;
    Profile* profile = settings->getParentHandle();
    query.prepare(UPDATE_MACHINE_QUERY);

    query.addBindValue(settings->getParentID());
    query.addBindValue(settings->getQueryPeriod());
    query.addBindValue(settings->getUnits());
    query.addBindValue(settings->getRapidSpeed());
    query.addBindValue(settings->getAcceleration());
    query.addBindValue(settings->getSpindleMin());
    query.addBindValue(settings->getSpindleMax());
    query.addBindValue(settings->getProbeCmds());
    query.addBindValue(settings->getSafePositionCmds());
    query.addBindValue(settings->getRestoreOrigin());
    query.addBindValue(settings->getRestoreType());
    query.addBindValue(settings->getUserCmd1());
    query.addBindValue(settings->getUserCmd2());
    query.addBindValue(settings->getUserCmd3());
    query.addBindValue(settings->getUserCmd4());
    query.addBindValue(settings->getHeightMapProbeFeed());
    query.addBindValue(profile->getID());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error updating machine"
                 << query.lastError();
        return false;
    }

    qDebug() << "SqlSettingsModel: Updated machine with id"
             << settings->getID();
    return true;
}

bool SqlSettingsModel::deleteMachineSettingsFromDB(MachineSettings* settings)
{
    qDebug() << "SqlSettingsModel: deleteMachineFromDB";
    QSqlQuery query;
    query.prepare(DELETE_MACHINE_QUERY);
    query.addBindValue(settings->getID());
    return query.exec();
}

bool SqlSettingsModel::createConnectionSettingsTable()
{
    qDebug() << "SqlSettingsModel:createConnectionTable Creating table";
    QSqlQuery query;
    query.prepare(CREATE_CONNECTION_TABLE_QUERY);
    if (!query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }
    return true;
}

int SqlSettingsModel::getConnectionSettingsFromDB(Profile* profile)
{
    qDebug() << "SqlSettingsModel::getConnectionFromDB for " << profile->getID();
    // Get form DB
    QSqlQuery query;
    query.prepare(SELECT_CONNECTION_BY_PROFILE_ID_QUERY);
    query.addBindValue(profile->getID());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: getConnectionFromDB ERROR" << query.lastError();
    }

    int idFieldNum = query.record().indexOf("id");
    int profileIdFieldNum = query.record().indexOf("profile_id");
    int serialPortFieldNum = query.record().indexOf("serial_port");
    int serialBaudFieldNum = query.record().indexOf("serial_baud");
    int ignoreErrorReponsesFieldNum = query.record().indexOf("ignore_error_responses");
    int setParserStateFieldNum = query.record().indexOf("set_parser_state");
    int arcApproximationFieldNum = query.record().indexOf("arc_approximation");
    int arcApproximationLengthFieldNum = query.record().indexOf("arc_approximation_length");
    int arcApproximationDegreesFieldNum = query.record().indexOf("arc_approximation_degrees");

    // Populate
    int records = 0;
    while (query.next())
    {
        int id = query.value(idFieldNum).toInt();
        int profile_id = query.value(profileIdFieldNum).toInt();
        QString serial_port = query.value(serialPortFieldNum).toString();
        int serial_baud = query.value(serialBaudFieldNum).toInt();
        bool ignore_error_responses = query.value(ignoreErrorReponsesFieldNum).toBool();
        bool set_parser_state = query.value(setParserStateFieldNum).toBool();
        bool arc_approximation = query.value(arcApproximationFieldNum).toBool();
        float arc_approximation_length = query.value(arcApproximationLengthFieldNum).toFloat();
        float arc_approximation_degrees = query.value(arcApproximationDegreesFieldNum).toFloat();

        auto connection = profile->getConnectionSettingsHandle();
        qDebug() << "SqlSettingsModel: Retrieving connection record for new id" << connection->getID();
        connection->setID(id);
        qDebug() << "SqlSettingsModel: Retrieving connection record now with id" << connection->getID();
        connection->setSerialPort(serial_port);
        connection->setSerialBaudRate(serial_baud);
        connection->setIgnoreErrorMessages(ignore_error_responses);
        connection->setSetParserState(set_parser_state);
        connection->setArcApproximation(arc_approximation);
        connection->setArcApproximationLength(arc_approximation_length);
        connection->setArcApproximationDegrees(arc_approximation_degrees);
        records++;
    }

    qDebug() << "SqlSettingsModel: Got " << records << "Connection records";

    return records;
}

bool SqlSettingsModel::insertConnectionSettingsInDB(ConnectionSettings* connection)
{
    qDebug() << "SqlSettingsModel: insertConnectionInDB";
    QSqlQuery query;
    query.prepare(INSERT_CONNECTION_QUERY);
    query.addBindValue(connection->getProfileID());
    query.addBindValue(connection->getSerialPort());
    query.addBindValue(connection->getSerialBaudRate());
    query.addBindValue(connection->getIgnoreErrorMessages());
    query.addBindValue(connection->getSetParserState());
    query.addBindValue(connection->getArcApproximation());
    query.addBindValue(connection->getArcApproximationLength());
    query.addBindValue(connection->getArcApproximationDegrees());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error inserting connection"
                 << query.lastError();
        return false;
    }

    connection->setID(query.lastInsertId().toInt());
    qDebug() << "SqlSettingsModel: Inserted connection with id" << connection->getID();
    return true;
}

bool SqlSettingsModel::updateConnectionSettingsInDB(ConnectionSettings* connection)
{
    qDebug() << "SqlSettingsModel: updateConnectionInDB";
    QSqlQuery query;
    Profile* profile = connection->getParentHandle();
    query.prepare(UPDATE_CONNECTION_QUERY);
    query.addBindValue(profile->getID());
    query.addBindValue(connection->getSerialPort());
    query.addBindValue(connection->getSerialBaudRate());
    query.addBindValue(connection->getIgnoreErrorMessages());
    query.addBindValue(connection->getSetParserState());
    query.addBindValue(connection->getArcApproximation());
    query.addBindValue(connection->getArcApproximationLength());
    query.addBindValue(connection->getArcApproximationDegrees());
    query.addBindValue(connection->getID());

    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error updating connection"
                 << query.lastError();
        return false;
    }

    qDebug() << "SqlSettingsModel: Updated connection with id" << connection->getID();
    return true;
}

bool SqlSettingsModel::deleteConnectionSettingsFromDB(ConnectionSettings* connection)
{
    qDebug() << "SqlSettingsModel: deleteConnectionFromDB";
    QSqlQuery query;
    query.prepare(DELETE_CONNECTION_QUERY);
    query.addBindValue(connection->getID());
    return query.exec();
}

bool SqlSettingsModel::updateProfileName(Profile* profile, QString name)
{
    if (profile)
    {
        profile->setName(name);
        return updateProfileInDB(profile);
    }
    return false;
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

bool SqlSettingsModel::deleteProfileFromDB(Profile* profile)
{
    qDebug() << "SqlSettingsModel: deleteProfileFromDB";
    QSqlQuery query;
    query.prepare(DELETE_PROFILE_QUERY);
    query.addBindValue(profile->getID());
    return query.exec();
}

int SqlSettingsModel::getRecentGcodeFilesFromDB(Profile* profile)
{
    qDebug() << "SqlSettingsModel::getRecentGcodeFilesFromDB";
    RecentFilesModel* modelHandle = profile->getRecentGcodeFilesModelHandle();
    modelHandle->clear();
    // Get form DB
    QSqlQuery query;
    query.prepare(SELECT_RECENT_GCODE_FILES_BY_PROFILE_ID_QUERY);
    query.addBindValue(profile->getID());
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
        modelHandle->add(RecentFile(id,index,profile_id,path));
    }
    return modelHandle->count();
}

void SqlSettingsModel::onCurrentProfileChanged(Profile* profile)
{
    getConnectionSettingsFromDB(profile);
    getInterfaceSettingsFromDB(profile);
    getMachineSettingsFromDB(profile);
    getRecentGcodeFilesFromDB(profile);
    getToolsFromDB(profile);
    getToolGeometryFromDB(profile);
    getToolHoldersFromDB(profile);
    getToolHoldersGeometryFromDB(profile);
}

void SqlSettingsModel::onConnecitonSettingsUpdated(ConnectionSettings* settings)
{
    updateConnectionSettingsInDB(settings);
}

void SqlSettingsModel::onInterfaceSettingsUpdated(InterfaceSettings* settings)
{
    updateInterfaceSettingsInDB(settings);
}

void SqlSettingsModel::onMachineSettingsUpdated(MachineSettings* settings)
{
    updateMachineSettingsInDB(settings);
}

void SqlSettingsModel::onToolCreated()
{
    qDebug() << "SqlSettingsModel::onToolCreated";
    auto tool = QSharedPointer<Tool>::create(mProfilesListModel->getCurrentProfileHandle());
    insertToolInDB(tool.data());
    mProfilesListModel
            ->getCurrentProfileHandle()
            ->getToolListModelHandle()
            ->insertItem(tool);
}

void SqlSettingsModel::onToolUpdated(Tool* tool)
{
    qDebug() << "SqlSettingsModel::onToolUpdated";
    updateToolInDB(tool);
}

void SqlSettingsModel::onToolDeleted(Tool* tool)
{
    qDebug() << "SqlSettingsModel::onToolDeleted";
    for (auto geom : tool->getGeometryTableModelHandle()->getDataHandles())
    {
        onToolGeometryDeleted(geom);
    }
    deleteToolFromDB(tool);
    mProfilesListModel
            ->getCurrentProfileHandle()
            ->getToolListModelHandle()
            ->deleteItem(tool);
}

void SqlSettingsModel::onToolHolderCreated()
{
    qDebug() << "SqlSettingsModel::onToolHolderCreated";
    auto toolHolder = QSharedPointer<ToolHolder>::create
            (mProfilesListModel->getCurrentProfileHandle());
    insertToolHolderInDB(toolHolder.data());
    mProfilesListModel
            ->getCurrentProfileHandle()
            ->getToolHolderListModelHandle()
            ->insertItem(toolHolder);
}

void SqlSettingsModel::onToolHolderUpdated(ToolHolder* toolHolder)
{
    qDebug() << "SqlSettingsModel::onToolHolderUpdated";
    ToolHolderListModel* model = toolHolder->getParentHandle()->getToolHolderListModelHandle();
    if (model)
    {
        model->setData
                (
                    model->indexOf(toolHolder),
                    toolHolder->getName(),
                    Qt::EditRole
                    );
    }
    updateToolHolderInDB(toolHolder);
}

void SqlSettingsModel::onToolHolderDeleted(ToolHolder* toolHolder)
{
    qDebug() << "SqlSettingsModel::onToolHolderDeleted";
    for (auto geom : toolHolder->getGeometryTableModelHandle()->getDataHandles())
    {
        onToolHolderGeometryDeleted(geom);
    }
    deleteToolHolderFromDB(toolHolder);
    mProfilesListModel
            ->getCurrentProfileHandle()
            ->getToolHolderListModelHandle()
            ->deleteItem(toolHolder);
}

void SqlSettingsModel::onToolHolderGeometryCreated()
{
    qDebug() << "SqlSettingsModel::onToolHolderGeometryCreated";
    ToolHolder * holder = mProfilesListModel
            ->getCurrentProfileHandle()
            ->getToolHolderListModelHandle()
            ->getSelected();

    ToolHolderGeometry* newGeom = holder->insertNew();

    insertToolHolderGeometryInDB(newGeom);
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
    mProfilesListModel
            ->getCurrentProfileHandle()
            ->getToolHolderListModelHandle()
            ->getSelected()
            ->deleteItem(toolHolderGeometry);
}

void SqlSettingsModel::onToolGeometryCreated()
{
    qDebug() << "SqlSettingsModel::onToolGeometryCreated";
    Tool * tool = mProfilesListModel
            ->getCurrentProfileHandle()
            ->getToolListModelHandle()
            ->getSelected();

    ToolGeometry* newGeom = tool->insertNew();

    insertToolGeometryInDB(newGeom);

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
    mProfilesListModel
            ->getCurrentProfileHandle()
            ->getToolListModelHandle()
            ->getSelected()
            ->deleteItem(toolGeometry);
}

bool SqlSettingsModel::insertProfileInDB(Profile* profile)
{
    qDebug() << "SqlSettingsModel::insertProfileInDB";
    QSqlQuery query;
    query.prepare(INSERT_PROFILE_QUERY);
    query.addBindValue(profile->getName());
    query.addBindValue(profile->getSelected());
    if (!query.exec())
    {
        qDebug() << "SqlSettingsModel: Error inserting profile"
                 << query.lastError();
        return false;
    }
    profile->setID(query.lastInsertId().toInt());
    qDebug() << "SqlSettingsModel: Profile updated with id" << profile->getID();
    return true;
}

bool SqlSettingsModel::profileValid(Profile* profile)
{
    return (profile->getID() >= 0);
}

void SqlSettingsModel::onSaveSettings()
{

}

void SqlSettingsModel::onLoadSettings()
{
    getProfilesFromDB();
    getCurrentProfileHandle();
    emit settingsModelReadySignal(this);
}

void SqlSettingsModel::onSettingChanged (QString groupName, QString settingName, QVariant value)
{

}
