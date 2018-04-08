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

ToolModelListModel::ToolModelListModel(QObject *parent)
    : QAbstractListModel(parent) {}

int ToolModelListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mData.count();
}

int ToolModelListModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

Qt::ItemFlags ToolModelListModel::flags(const QModelIndex& index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ToolModelListModel::data(const QModelIndex &index, int role) const
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

void ToolModelListModel::insert(QSharedPointer<Tool> newItem)
{
    insertRows(mData.count(),1,QModelIndex());
    mData.append(newItem);
    emit dataChanged(QModelIndex(),QModelIndex());
}

void ToolModelListModel::remove(QSharedPointer<Tool> item)
{
    int index = mData.indexOf(item);
    removeRows(index, 1, QModelIndex());
    mData.removeOne(item);
    emit dataChanged(QModelIndex(),QModelIndex());
}

QList<QSharedPointer<Tool>> ToolModelListModel::getAllData()
{
   return mData;
}

QSharedPointer<Tool> ToolModelListModel::getData(int index)
{
    if (index < 0 || index > mData.count())
    {
        return QSharedPointer<Tool>();
    }
    return mData[index];
}
