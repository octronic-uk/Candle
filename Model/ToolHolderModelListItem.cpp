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
#include "ToolHolderModelListItem.h"

ToolHolderModelListItem::ToolHolderModelListItem(QString name)
    : mName(name)
{
    mTableModel = QSharedPointer<ToolHolderModelTableModel>::create();
}

QString ToolHolderModelListItem::getName() const
{
    return mName;
}

void ToolHolderModelListItem::setName(const QString& name)
{
    mName = name;
}

QSharedPointer<ToolHolderModelTableModel> ToolHolderModelListItem::getTableModel() const
{
    return mTableModel;
}

void ToolHolderModelListItem::addNewRow()
{
    mTableModel->insertRows(0,1,QModelIndex());
}

void ToolHolderModelListItem::setSelectedRow(int row)
{
    mSelected = mTableModel->getItemAtRow(row);
}

ToolHolderModelGeometryItem& ToolHolderModelListItem::getSelected()
{
    return mSelected;
}
