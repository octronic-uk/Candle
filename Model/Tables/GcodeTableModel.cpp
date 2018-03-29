// This file is a part of "Cocoanut" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GcodeTableModel.h"
#include <QtDebug>

GcodeTableModel::GcodeTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    qDebug() << "GcodeTableModel: Constructing";
    mHeaders << tr("#") << tr("Command") << tr("State") << tr("Response") << tr("Line") << tr("Args");
}

QVariant GcodeTableModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "GcodeTableModel: data(const QModelIndex &index, int role) const";
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
        case 0: // Number
            if (index.row() == this->rowCount() - 1)
            {
                return QString();
            }
            else
            {
                return  QString::number(index.row() + 1);
            }
        case 1: // Command
            return mData.at(index.row()).getCommand();
        case 2: // Status
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
                default:
                    return tr("Unknown");
            }
        case 3: // Response
            return mData.at(index.row()).getResponse();
        case 4: // Line
            return mData.at(index.row()).getLine();
        case 5: // Args
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
    qDebug() << "GcodeTableModel: setData";
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
    qDebug() << "GcodeTableModel: insertRow";
    if (row > rowCount())
    {
        return false;
    }
    beginInsertRows(parent, row, row);
    mData.insert(row, GcodeItem());
    endInsertRows();
    return true;
}

bool GcodeTableModel::removeRow(int row, const QModelIndex &parent)
{
    qDebug() << "GcodeTableModel: removeRow";
    if (!index(row, 0).isValid())
    {
        return false;
    }
    beginRemoveRows(parent, row, row);
    mData.removeAt(row);
    endRemoveRows();
    return true;
}

bool GcodeTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    qDebug() << "GcodeTableModel: removeRows";
    beginRemoveRows(parent, row, row + count - 1);
    mData.erase(mData.begin() + row, mData.begin() + row + count);
    endRemoveRows();
    return true;
}

void GcodeTableModel::clear()
{
    qDebug() << "GcodeTableModel: clear";
    beginResetModel();
    mData.clear();
    endResetModel();
}

int GcodeTableModel::rowCount(const QModelIndex &parent) const
{
    qDebug() << "GcodeTableModel: rowCount";
    Q_UNUSED(parent)
    return mData.size();
}

int GcodeTableModel::columnCount(const QModelIndex &parent) const
{
    qDebug() << "GcodeTableModel: columnCount";
    Q_UNUSED(parent)
    return mHeaders.size();
}

QVariant GcodeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    qDebug() << "GcodeTableModel: headerData";
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
    qDebug() << "GcodeTableModel: flags";
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
    qDebug() << "GcodeTableModel: data()";
    return mData;
}

void GcodeTableModel::append(QList<GcodeItem> &items)
{
    qDebug() << "GcodeTableModel: append";
    int startRow = rowCount();
    int endRow = startRow + items.count() - 1;

    beginInsertRows(QModelIndex(),startRow,endRow);
    mData << items;
    endInsertRows();
    emit dataChanged(QModelIndex(),QModelIndex());
}
