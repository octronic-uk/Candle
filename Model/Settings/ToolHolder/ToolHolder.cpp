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

#include <QtDebug>

#include "ToolHolder.h"
#include "Model/Settings/ToolHolder/ToolHolderGeometry.h"

ToolHolder::ToolHolder(Profile* parent, int id ,QString name)
    : AbstractDatabaseRecord(id),
      mParentHandle(parent),
      mName(name),
      mSelectedGeometryHandle(nullptr)
{
    qDebug() << "ToolHolder: Constructing(int,QString)"
             << getID()
             << getName();

    mGeometryTableModel = QSharedPointer<ToolHolderGeometryTableModel>::create(this);
}

ToolHolder::ToolHolder(const ToolHolder& other)
    : AbstractDatabaseRecord (other.getID()),
      mName(other.mName),
      mSelectedGeometryHandle(other.mSelectedGeometryHandle),
      mGeometryTableModel(other.mGeometryTableModel)
{
    qDebug() << "ToolHolder: Constructing(ToolHolder&)"
             << getID()
             << getName();
}

QString ToolHolder::getName() const
{
    return mName;
}

void ToolHolder::setName(const QString& name)
{
    mName = name;
}

bool ToolHolder::operator==(const ToolHolder& other)
{
   return getID() == other.getID() && getName() == other.getName();
}

ToolHolderGeometryTableModel* ToolHolder::getGeometryTableModelHandle()
{
    return mGeometryTableModel.data();
}

ToolHolderGeometry* ToolHolder::insertNew()
{
    return mGeometryTableModel->insertNew();
}

void ToolHolder::insertItem(QSharedPointer<ToolHolderGeometry> item)
{
    mGeometryTableModel->insertItem(item);
}

void ToolHolder::disconnect()
{
    if (mGeometryTableModel.data())
    {
        mGeometryTableModel->disconnect();
    }
}

void ToolHolder::removeSelectedGeometryRow()
{
    if (mSelectedGeometryHandle && mSelectedGeometryHandle->getID() < 0)
    {
       qDebug() << "ToolHolder: Cannot remove, selected.id < 0";
       return;
    }
    mGeometryTableModel->deleteItem(mSelectedGeometryHandle);
    mSelectedGeometryHandle = nullptr;
}

Profile* ToolHolder::getParentHandle() const
{
    return mParentHandle;
}

int ToolHolder::getNextGeometryID()
{
    return mGeometryTableModel->rowCount();
}

void ToolHolder::setSelectedGeometryRow(int row)
{
    qDebug() << "Setting selected geometry row" << row;
    mSelectedGeometryHandle = mGeometryTableModel->getToolHolderGeometryHandleAtRow(row);
}

ToolHolderGeometry* ToolHolder::getSelectedGeometryHandle()
{
    return mSelectedGeometryHandle;
}

void ToolHolder::deleteItem(ToolHolderGeometry* item)
{
   mGeometryTableModel->deleteItem(item);
}
