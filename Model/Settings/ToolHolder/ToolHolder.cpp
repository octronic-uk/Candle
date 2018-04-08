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
#include "ToolHolder.h"

ToolHolder::ToolHolder(int id ,QString name)
    : AbstractDatabaseRecord(id),
      mName(name)
{
    mTableModel = QSharedPointer<ToolHolderGeometryTableModel>::create();
}

QString ToolHolder::getName() const
{
    return mName;
}

void ToolHolder::setName(const QString& name)
{
    mName = name;
}

QSharedPointer<ToolHolderGeometryTableModel> ToolHolder::getTableModel() const
{
    return mTableModel;
}

void ToolHolder::addNewRow()
{
    mTableModel->insertRows(0,1,QModelIndex());
}

void ToolHolder::insertGeometry(ToolHolderGeometry item)
{
    mTableModel->insert(item);
}

void ToolHolder::setSelectedRow(int row)
{
    mSelected = mTableModel->getItemAtRow(row);
}

ToolHolderGeometry& ToolHolder::getSelected()
{
    return mSelected;
}
