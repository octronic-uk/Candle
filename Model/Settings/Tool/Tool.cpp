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
#include <QtDebug>

Tool::Tool(int id ,QString name)
    : AbstractDatabaseRecord(id),
      mName(name),
      mSelectedGeometryHandle(nullptr)
{
    qDebug() << "Tool: Constructing(int,QString)"
             << getID()
             << getName();

    mGeometryTableModel = QSharedPointer<ToolGeometryTableModel>::create();
}

Tool::Tool(const Tool& other)
    : AbstractDatabaseRecord (other.getID()),
      mName(other.mName),
      mSelectedGeometryHandle(other.mSelectedGeometryHandle),
      mGeometryTableModel(other.mGeometryTableModel)
{
    qDebug() << "Tool: Constructing(Tool&)"
             << getID()
             << getName();
}

QString Tool::getName() const
{
    return mName;
}

void Tool::setName(const QString& name)
{
    mName = name;
}

bool Tool::operator==(const Tool& other)
{
   return getID() == other.getID() && getName() == other.getName();
}

ToolGeometryTableModel* Tool::getGeometryTableModelHandle()
{
    return mGeometryTableModel.data();
}

void Tool::addNewGeometryRow()
{
    qDebug() << "Tool: addNewGeometryRow";
    if (getID() < 0)
    {
        qDebug() << "Tool: WARNING!! parent id < 0";
    }
    mGeometryTableModel->insertRows(getID(), mGeometryTableModel->rowCount(),1,QModelIndex());
}

void Tool::insertGeometry(QSharedPointer<ToolGeometry> item)
{
    mGeometryTableModel->insert(item);
}

void Tool::disconnect()
{
    mGeometryTableModel->disconnect();
}

void Tool::removeSelectedGeometryRow()
{
    if (mSelectedGeometryHandle && mSelectedGeometryHandle->getID() < 0)
    {
       qDebug() << "Tool: Cannot remove, selected.id < 0";
       return;
    }
    mGeometryTableModel->remove(mSelectedGeometryHandle);
    mSelectedGeometryHandle = nullptr;
}

void Tool::setSelectedGeometryRow(int row)
{
    qDebug() << "Setting selected geometry row" << row;
    mSelectedGeometryHandle = mGeometryTableModel->getToolGeometryHandleAtRow(row);
}

ToolGeometry* Tool::getSelectedGeometryHandle()
{
    return mSelectedGeometryHandle;
}
