/*
 * ToolModelListItem.h
 *
 * Created: 06 2018 by Ashley
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

#include "Model/Settings/Sql/AbstractDatabaseRecord.h"
#include "Model/Settings/Tool/ToolGeometryTableModel.h"
#include <QSharedPointer>

class Profile;

class Tool : public AbstractDatabaseRecord
{
public:
    explicit Tool
    (
        Profile* mParentHandle,
        int id = -1,
        QString name = "New Tool",
        int toolNumber = -1,
        int toolHolderID = -1
    );

    Tool(const Tool& other);

    QString getName() const;
    void setName(const QString& name);

    bool operator==(const Tool& other);

    ToolGeometryTableModel* getGeometryTableModelHandle();
    void addNewGeometryRow();
    ToolGeometry* getSelectedGeometryHandle();

    ToolGeometry* insertNew();
    void insertItem(QSharedPointer<ToolGeometry> item);
    void deleteItem(ToolGeometry* geom);
    void disconnect();

    int getToolNumber() const;
    void setToolNumber(int toolNumber);

    int getToolHolderID() const;
    void setToolHolderID(int toolHolderID);

    float getTotalHeight();

    Profile* getParentHandle() const;

    void setSelectedGeometryRow(int row);
    QString toString();
private:
    Profile* mParentHandle;
    ToolGeometry *mSelectedGeometryHandle;
    QString mName;
    int mToolNumber;
    int mToolHolderID;
    QSharedPointer<ToolGeometryTableModel> mGeometryTableModel;
};
