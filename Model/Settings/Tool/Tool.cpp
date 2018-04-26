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
#include "Model/Settings/Profile/Profile.h"

#include <QtDebug>

Tool::Tool
(
    Profile* parent,
    int id,
    QString name,
    int toolNumber,
    int toolHolderID
)
    : AbstractDatabaseRecord(id),
      mParentHandle(parent),
      mSelectedGeometryHandle(nullptr),
      mName(name),
      mToolNumber(toolNumber),
      mToolHolderID(toolHolderID)
{
    qDebug() << "Tool: Constructing(int,QString)"
             << getID()
             << getName()
             << getToolNumber()
             << getToolHolderID();

    mGeometryTableModel = QSharedPointer<ToolGeometryTableModel>::create(this);
}

Tool::Tool(const Tool& other)
    : AbstractDatabaseRecord (other.getID()),
      mName(other.mName),
      mToolNumber(other.mToolNumber),
      mToolHolderID(other.mToolHolderID),
      mGeometryTableModel(other.mGeometryTableModel)
{
    qDebug() << "Tool: Constructing(Tool&)"
             << getID()
             << getName()
             << getToolNumber()
             << getToolHolderID();
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
    mGeometryTableModel->insertRows(mGeometryTableModel->rowCount(),1,QModelIndex());
}

void Tool::insertItem(QSharedPointer<ToolGeometry> item)
{
    mGeometryTableModel->insertItem(item);
}

void Tool::deleteItem(ToolGeometry* geom)
{
   mGeometryTableModel->deleteItem(geom);
}

void Tool::disconnect()
{
    mGeometryTableModel->disconnect();
}

int Tool::getToolNumber() const
{
    return mToolNumber;
}

void Tool::setToolNumber(int toolNumber)
{
    mToolNumber = toolNumber;
}

int Tool::getToolHolderID() const
{
    return mToolHolderID;
}

void Tool::setToolHolderID(int toolHolderID)
{
    mToolHolderID = toolHolderID;
}

float Tool::getTotalHeight()
{
    float height = 0;
   for (auto geom : mGeometryTableModel->getDataHandles())
   {
      height += geom->getHeight();
   }
   return height;
}

Profile* Tool::getParentHandle() const
{
    return mParentHandle;
}

void Tool::setSelectedGeometryRow(int row)
{
    qDebug() << "Setting selected geometry row" << row;
    mSelectedGeometryHandle = mGeometryTableModel->getToolGeometryHandleAtRow(row);
}

QString Tool::toString()
{
    return QString("(%1) %2")
               .arg(QString::number(mToolNumber))
               .arg(mName);
}

ToolGeometry* Tool::getSelectedGeometryHandle()
{
    return mSelectedGeometryHandle;
}

ToolGeometry* Tool::insertNew()
{
    return mGeometryTableModel->insertNew();
}
