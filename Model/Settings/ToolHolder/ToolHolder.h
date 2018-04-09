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
#include "Model/Settings/ToolHolder/ToolHolderGeometryTableModel.h"
#include <QSharedPointer>

class ToolHolder : public AbstractDatabaseRecord
{
public:
    explicit ToolHolder(int id = -1, QString name = "");
    ToolHolder(const ToolHolder& other);

    QString getName() const;
    void setName(const QString& name);

    bool operator==(const ToolHolder& other);

    ToolHolderGeometryTableModel* getGeometryTableModelHandle();
    void addNewGeometryRow();
    void setSelectedGeometryRow(int row);
    ToolHolderGeometry* getSelectedGeometryHandle();

    void insertGeometry(QSharedPointer<ToolHolderGeometry> item);
    void disconnect();
    void removeSelectedGeometryRow();

private:
    QString mName;
    ToolHolderGeometry* mSelectedGeometryHandle;
    QSharedPointer<ToolHolderGeometryTableModel> mGeometryTableModel;
};
