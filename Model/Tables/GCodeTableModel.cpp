// This file is a part of "Cocoanut" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GCodeTableModel.h"

GcodeTableModel::GcodeTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    mHeaders << tr("#") << tr("Command") << tr("State") << tr("Response") << tr("Line") << tr("Args");
}

QVariant GcodeTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.row() >= mData.size())
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
        case 0:
            return index.row() == this->rowCount() - 1 ? QString() : QString::number(index.row() + 1);
        case 1:
            return mData.at(index.row()).getCommand();
        case 2:
            if (index.row() == this->rowCount() - 1)
            {
                return QString();
            }

            switch (mData.at(index.row()).getState())
            {
                case GCODE_ITEM_STATE_IN_QUEUE:
                    return tr("In queue");
                case GCODE_ITEM_STATE_SENT:
                    return tr("Sent");
                case GCODE_ITEM_STATE_PROCESSED:
                    return tr("Processed");
                case GCODE_ITEM_STATE_SKIPPED:
                    return tr("Skipped");
            }
            return tr("Unknown");
        case 3:
            return mData.at(index.row()).getResponse();
        case 4:
            return mData.at(index.row()).getLine();
        case 5:
            return QVariant(mData.at(index.row()).getArgs());
        }
    }

    if (role == Qt::TextAlignmentRole)
    {
        switch (index.column())
        {
        case 0:
            return Qt::AlignCenter;
        default:
            return Qt::AlignVCenter;
        }
    }

    return QVariant();
}

bool GcodeTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        switch (index.column())
        {
        case 0:
            return false;
        case 1:
            mData[index.row()].setCommand(value.toString());
            break;
        case 2:
            mData[index.row()].setState(value.toInt());
            break;
        case 3:
            mData[index.row()].setResponse(value.toString());
            break;
        case 4:
            mData[index.row()].setLine(value.toInt());
            break;
        case 5:
            mData[index.row()].setArgs(value.toStringList());
            break;
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool GcodeTableModel::insertRow(int row, const QModelIndex &parent)
{
    if (row > rowCount()) return false;
    beginInsertRows(parent, row, row);
    mData.insert(row, GcodeItem());
    endInsertRows();
    return true;
}

bool GcodeTableModel::removeRow(int row, const QModelIndex &parent)
{
    if (!index(row, 0).isValid()) return false;
    beginRemoveRows(parent, row, row);
    mData.removeAt(row);
    endRemoveRows();
    return true;
}

bool GcodeTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    mData.erase(mData.begin() + row, mData.begin() + row + count);
    endRemoveRows();
    return true;
}

void GcodeTableModel::clear()
{
    beginResetModel();
    mData.clear();
    endResetModel();
}

int GcodeTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return mData.size();
}

int GcodeTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 6;
}

QVariant GcodeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if (orientation == Qt::Horizontal)
    {
        return mHeaders.at(section);
    }
    else
    {
        return QString::number(section + 1);
    }
}

Qt::ItemFlags GcodeTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return NULL;
    }
    if (index.column() == 1)
    {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }
    else
    {
        return QAbstractTableModel::flags(index);
    }
}

QList<GcodeItem> &GcodeTableModel::data()
{
    return mData;
}

void GcodeTableModel::append(QList<GcodeItem> &items)
{
    int startRow = rowCount();
    int endRow = startRow + items.count() - 1;

    beginInsertRows(QModelIndex(),startRow,endRow);

    for (GcodeItem nextItem : items)
    {
        mData.append(nextItem);
    }

    endInsertRows();

}
