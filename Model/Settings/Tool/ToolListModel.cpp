/*
 * ToolModelListModel.cpp
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

#include "Model/Settings/Profile/Profile.h"
#include "Model/Settings/Tool/Tool.h"

#include "ToolListModel.h"
#include <QtDebug>

ToolListModel::ToolListModel(Profile* profileHandle, QObject *parent)
    : QAbstractListModel(parent),
      mProfileHandle(profileHandle),
      mSelectedToolHandle(nullptr)
{}

void ToolListModel::initialise(QList<QSharedPointer<Tool>> data)
{
   mData = data;
}

int ToolListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mData.count();
}

int ToolListModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

Qt::ItemFlags ToolListModel::flags(const QModelIndex& index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ToolListModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        return QString("(%1) %2")
                .arg(mData.at(index.row())->getToolNumber())
                .arg(mData.at(index.row())->getName());
    }

    return QVariant();
}

void ToolListModel::insertItem(QSharedPointer<Tool> newItem)
{
    insertRows(mData.count(),1,QModelIndex());
    mData.append(newItem);
    emit dataChanged(QModelIndex(),QModelIndex());
}

void ToolListModel::deleteItem(Tool* item)
{
    int index = -1;
    for (auto data : mData)
    {
        if (data.data() == item)
        {
            index = mData.indexOf(data);
            break;
        }
    }
    if (index > -1)
    {
        removeRows(index, 1, QModelIndex());
        mData.removeAt(index);
        emit dataChanged(QModelIndex(),QModelIndex());
    }
}

QList<QSharedPointer<Tool>>& ToolListModel::getAllData()
{
    return mData;
}

void ToolListModel::clear()
{
   mData.clear();
   emit dataChanged(QModelIndex(),QModelIndex());
}

QModelIndex ToolListModel::indexOf(Tool* holder)
{
    int index = -1;
    for (auto data : mData)
    {
        if (data.data() == holder)
        {
            index = mData.indexOf(data);
            break;
        }
    }
    return createIndex(index,0);
}

QModelIndex ToolListModel::getSelectedToolIndex()
{
    if (mSelectedToolHandle)
    {
        return getToolIndex(mSelectedToolHandle);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex ToolListModel::getToolIndex(Tool* holder)
{
    int index = -1;
    for (auto data : mData)
    {
        if (data.data() == holder)
        {
            index = mData.indexOf(data);
            break;
        }
    }
    return createIndex(index,TOOL_HOLDER_ID_INDEX);
}

QModelIndex ToolListModel::getToolNumberIndex(Tool* holder)
{
    int index = -1;
    for (auto data : mData)
    {
        if (data.data() == holder)
        {
            index = mData.indexOf(data);
            break;
        }
    }
    return createIndex(index,TOOL_NUMBER_INDEX);
}

Tool* ToolListModel::getSelected() const
{
    return mSelectedToolHandle;
}

void ToolListModel::setSelectedToolHandle(Tool* selectedToolHandle)
{
    mSelectedToolHandle = selectedToolHandle;
}

bool ToolListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.row() >= 0 && index.row() < mData.count())
    {
        if (role == Qt::EditRole)
        {
            switch (index.column())
            {
                case TOOL_NAME_INDEX:
                    mData[index.row()]->setName(value.toString());
                    break;
                case TOOL_HOLDER_ID_INDEX:
                    mData[index.row()]->setToolHolderID(value.toInt());
                    break;
                case TOOL_NUMBER_INDEX:
                    mData[index.row()]->setToolNumber(value.toInt());
                    break;
            }
            emit dataChanged(QModelIndex(), QModelIndex());
        }
    }
    return true;
}

Tool* ToolListModel::getData(int index)
{
    return mData[index].data();
}


