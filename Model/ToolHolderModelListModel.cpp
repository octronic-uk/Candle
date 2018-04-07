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

#include "ToolHolderModelListModel.h"
#include <QtDebug>

ToolHolderModelListModel::ToolHolderModelListModel(QObject *parent)
    : QAbstractListModel(parent) {}

int ToolHolderModelListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //qDebug() << "ToolHolderModelListModel::rowCount()";
    return mData.count();
}

int ToolHolderModelListModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    //qDebug() << "ToolHolderModelListModel::columnCount()";
    return 1;
}

Qt::ItemFlags ToolHolderModelListModel::flags(const QModelIndex& index) const
{
    Q_UNUSED(index)
    //qDebug() << "ToolHolderModelListModel::flags";
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ToolHolderModelListModel::data(const QModelIndex &index, int role) const
{
    //qDebug() << "ToolHolderModelListModel::data()" << index;

    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        QString str = mData.at(index.row())->getName();
        //qDebug() << "ToolHolderModelListModel: Got" << str;
        return str;
    }

    return QVariant();
}

void ToolHolderModelListModel::insert(QSharedPointer<ToolHolderModelListItem> newItem)
{
    //qDebug() << "ToolHolderModelListModel::insert()";
    insertRows(mData.count(),1,QModelIndex());
    mData.append(newItem);
    //qDebug() << "ToolHolderModelListModel: data now has " << mData.count();
    emit dataChanged(QModelIndex(),QModelIndex());
}

void ToolHolderModelListModel::remove(QSharedPointer<ToolHolderModelListItem> item)
{
    //qDebug() << "ToolHolderModelListModel::remove()";
    int index = mData.indexOf(item);
    removeRows(index, 1, QModelIndex());
    mData.removeOne(item);
    emit dataChanged(QModelIndex(),QModelIndex());
}

QSharedPointer<ToolHolderModelListItem> ToolHolderModelListModel::getData(int index)
{
    if (index < 0 || index > mData.count())
    {
        //qDebug() << "ToolHolderModelListModel: No item at index " << index;
        return QSharedPointer<ToolHolderModelListItem>();
    }
    return mData[index];
}
