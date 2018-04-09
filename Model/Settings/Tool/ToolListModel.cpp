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

#include "ToolListModel.h"
#include <QtDebug>

ToolListModel::ToolListModel(QObject *parent)
    : QAbstractListModel(parent) {}

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
        QString str = mData.at(index.row())->getName();
        return str;
    }

    return QVariant();
}

void ToolListModel::insert(QSharedPointer<Tool> newItem)
{
    insertRows(mData.count(),1,QModelIndex());
    emit toolCreatedSignal(newItem.data());
    mData.append(newItem);
    emit dataChanged(QModelIndex(),QModelIndex());
}

void ToolListModel::remove(Tool* item)
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
        emit toolDeletedSignal(item);
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

bool ToolListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.row() >= 0 && index.row() < mData.count())
    {
        if (role == Qt::EditRole)
        {
            mData[index.row()]->setName(value.toString());
            emit toolUpdatedSignal(mData[index.row()].data());
            emit dataChanged(QModelIndex(), QModelIndex());
        }
    }
    return true;
}

Tool* ToolListModel::getData(int index)
{
    return mData[index].data();
}
