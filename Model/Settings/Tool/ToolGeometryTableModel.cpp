/*
 * ToolModelTableModel.cpp
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

#include "ToolGeometryTableModel.h"
#include <QtDebug>



ToolGeometryTableModel::ToolGeometryTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    mTableHeaders << "Height" << "Upper Diameter" << "Lower Diameter";
}

QVariant ToolGeometryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }
    else if (orientation == Qt::Horizontal)
    {
        return mTableHeaders.at(section);
    }
    return QString::number(section + 1);
}


int ToolGeometryTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mData.size();
}

int ToolGeometryTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mTableHeaders.count();
}

QVariant ToolGeometryTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
            case 0: // Height
                return mData.at(index.row())->getHeight();
            case 1: // Upper Diameter
                return mData.at(index.row())->getUpperDiameter();
            case 2: // Lower Diameter
                return mData.at(index.row())->getLowerDiameter();
        }
    }
    return QVariant();
}

bool ToolGeometryTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if (data(index, role) != value)
        {
            switch (index.column())
            {
                case 0: // Height
                    mData[index.row()]->setHeight(value.toFloat());
                    break;
                case 1: // Upper Diameter
                    mData[index.row()]->setUpperDiameter(value.toFloat());
                    break;
                case 2: // Lower Diameter
                    mData[index.row()]->setLowerDiameter(value.toFloat());
                    break;
            }
            emit dataChanged(index, index, QVector<int>() << role);
            emit toolGeometryUpdatedSignal(mData[index.row()].data());
            return true;
        }
    }
    return false;
}

Qt::ItemFlags ToolGeometryTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool ToolGeometryTableModel::insertRows(int parent_id, int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i=0; i<count; i++)
    {
        QSharedPointer<ToolGeometry> geom =
                QSharedPointer<ToolGeometry>::create
                    (-1,parent_id,mData.count()+i, 1.0f,1.0f,1.0f);

        mData.insert(row,geom);
        emit toolGeometryCreatedSignal(geom.data());
    }
    endInsertRows();
    return true;
}

bool ToolGeometryTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i=0; i<count; i++)
    {
        QSharedPointer<ToolGeometry> geom =
            QSharedPointer<ToolGeometry>::create
                (-1,-1,mData.count()+i, 1.0f,1.0f,1.0f);

        mData.insert(row,geom);
        emit toolGeometryCreatedSignal(geom.data());
    }
    endInsertRows();
    return true;
}

bool ToolGeometryTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    qDebug() << "ToolGeometryTableModel::removeRows" << row << count;
    if (row < 0 || row+count > mData.count())
    {
        qDebug() << "ToolGeometryTableModel::removeRows Attempted to remove invalid row";
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    for (int r=row; r<row+count; r++)
    {
        ToolGeometry* geom = mData.at(r).data();
        emit toolGeometryDeletedSignal(geom);
        mData.removeAt(r);
    }
    endRemoveRows();
    return true;
}

ToolGeometry* ToolGeometryTableModel::getToolGeometryHandleAtRow(int row)
{
    return mData[row].data();
}

void ToolGeometryTableModel::insert(QSharedPointer<ToolGeometry> item)
{
   mData.insert(item->getIndex(),item);
   emit dataChanged(QModelIndex(), QModelIndex());
}

void ToolGeometryTableModel::remove(ToolGeometry* geom)
{
    int index = -1;
    for (auto geomData : mData)
    {
        if (geomData.data() == geom)
        {
           index = mData.indexOf(geomData);
           break;
        }
    }

    if (index > -1)
    {
        removeRows(index,1,QModelIndex());
    }
}
