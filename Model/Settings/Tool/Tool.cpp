/*
 * ToolModelListItem.cpp
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
#include "Tool.h"

Tool::Tool(int id ,QString name)
    : AbstractDatabaseRecord(id),
      mName(name)
{
    mTableModel = QSharedPointer<ToolGeometryTableModel>::create();
}

QString Tool::getName() const
{
    return mName;
}

void Tool::setName(const QString& name)
{
    mName = name;
}

QSharedPointer<ToolGeometryTableModel> Tool::getTableModel() const
{
    return mTableModel;
}

void Tool::addNewRow()
{
    mTableModel->insertRows(0,1,QModelIndex());
}

void Tool::insertGeometry(ToolGeometry item)
{
    mTableModel->insert(item);
}

void Tool::setSelectedRow(int row)
{
    mSelected = mTableModel->getItemAtRow(row);
}

ToolGeometry& Tool::getSelected()
{
    return mSelected;
}
