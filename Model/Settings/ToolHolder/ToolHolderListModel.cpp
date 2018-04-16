/*
 * ToolHolderModelListModel.cpp
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
#include "Model/Settings/ToolHolder/ToolHolder.h"
#include "ToolHolderListModel.h"
#include <QtDebug>

ToolHolderListModel::ToolHolderListModel(Profile* profile, QObject *parent)
    : QAbstractListModel(parent),
      mProfileHandle(profile),
      mSelected(nullptr)
{}

void ToolHolderListModel::initialise(QList<QSharedPointer<ToolHolder>> data)
{
   mData = data;
}

int ToolHolderListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mData.count();
}

int ToolHolderListModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

Qt::ItemFlags ToolHolderListModel::flags(const QModelIndex& index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ToolHolderListModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        if (index.row() >= 0 && index.row() < mData.count())
        {
            QString str = mData.at(index.row())->getName();
            return str;
        }
    }

    return QVariant();
}

void ToolHolderListModel::insertItem(QSharedPointer<ToolHolder> newItem)
{
    insertRows(mData.count(),1,QModelIndex());
    mData.append(newItem);
    emit dataChanged(QModelIndex(),QModelIndex());
}

void ToolHolderListModel::deleteItem(ToolHolder* item)
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

QList<ToolHolder*> ToolHolderListModel::getDataHandles()
{
    QList<ToolHolder*> holders;
    for (auto holder : mData)
    {
        holders.append(holder.data());
    }
    return holders;
}

void ToolHolderListModel::clear()
{
   mData.clear();
   emit dataChanged(QModelIndex(),QModelIndex());
}

QModelIndex ToolHolderListModel::indexOf(ToolHolder* holder)
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

QModelIndex ToolHolderListModel::getIndexByID(int id)
{
   for (auto holder : mData)
   {
       if (holder->getID() == id)
       {
           return createIndex(mData.indexOf(holder),0);
       }
   }
   return createIndex(-1,-1);
}

ToolHolder* ToolHolderListModel::getToolHolderByID(int id)
{
   for (auto holder : mData)
   {
       if (holder->getID() == id)
       {
           return holder.data();
       }
   }
   return nullptr;
}

ToolHolder* ToolHolderListModel::getSelected() const
{
    return mSelected;
}

void ToolHolderListModel::setSelected(ToolHolder* selected)
{
    mSelected = selected;
}

bool ToolHolderListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.row() >= 0 && index.row() < mData.count())
    {
        if (role == Qt::EditRole)
        {
            mData[index.row()]->setName(value.toString());
            emit dataChanged(QModelIndex(), QModelIndex());
        }
    }
    return true;
}

ToolHolder* ToolHolderListModel::getData(int index)
{
    if (index < 0 || index >= mData.count())
    {
        return nullptr;
    }
    return mData[index].data();
}
