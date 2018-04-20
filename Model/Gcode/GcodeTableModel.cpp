// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GcodeTableModel.h"
#include <QtDebug>

GcodeTableModel::GcodeTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    qDebug() << "GcodeTableModel: Constructing";
    mHeaders << "Line"
            << "Command"
            << "Status"
            << "Response";

}

GcodeTableModel::~GcodeTableModel()
{
   clear();
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

        GcodeCommand* gCmd = mData.at(index.row());
        switch (index.column())
        {
        case 0: // Line
            return QVariant(gCmd->getLine());
        case 1: // Command
            if (gCmd->getState() == GcodeCommandState::Marker)
            {
               return gCmd->getMarker();
            }
            else
            {
                return gCmd->getCommand();
            }
        case 2: // Status
            switch (mData.at(index.row())->getState())
            {
                case GcodeCommandState::InQueue:
                    return tr("In Queue");
                case GcodeCommandState::Sent:
                    return tr("Sent");
                case GcodeCommandState::Processed:
                    return tr("Processed");
                case GcodeCommandState::Skipped:
                    return tr("Skipped");
                case GcodeCommandState::Marker:
                    return tr("Marker");
                default:
                    return tr("Unknown");
            }
        case 3: // Response
            return QVariant(gCmd->getResponse().getData());
        default:
            return QVariant();
        }
    }

    if (role == Qt::TextAlignmentRole)
    {
        return QVariant::fromValue(Qt::AlignCenter | Qt::AlignVCenter);
    }
    return QVariant();
}

bool GcodeTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role)
    if (index.isValid())
    {
        switch (index.column())
        {
        case 0:
            mData[index.row()]->setCommand(value.toString());
            break;
        case 1:
            mData[index.row()]->setState(value.value<GcodeCommandState>());
            break;
        case 2:
            mData[index.row()]->setResponse(value.value<GrblResponse>());
            break;
        case 3:
            mData[index.row()]->setLine(value.toInt());
            break;
        case 4:
            mData[index.row()]->setArgs(value.toStringList());
            break;
        }
        emit dataChanged(index, index);
    }
    return true;
}

bool GcodeTableModel::insertRow(int row, const QModelIndex &parent)
{
    if (row > rowCount())
    {
        return false;
    }
    beginInsertRows(parent, row, row);
    mData.insert(row, new GcodeCommand());
    endInsertRows();
    return true;
}

bool GcodeTableModel::removeRow(int row, const QModelIndex &parent)
{
    if (!index(row, 0).isValid())
    {
        return false;
    }
    beginRemoveRows(parent, row, row);
    mData.removeAt(row);
    // TODO - call delete here
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
    return mData.count();
}

int GcodeTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mHeaders.count();
}

QVariant GcodeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }
    else if (orientation == Qt::Horizontal)
    {
        return QVariant::fromValue(mHeaders.at(section));
    }
    return QString::number(section + 1);
}

Qt::ItemFlags GcodeTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::ItemIsEnabled;
    }
    else if (index.column() == 1)
    {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }
    return QAbstractTableModel::flags(index);
}

QList<GcodeCommand*> GcodeTableModel::data()
{
    return mData;
}

void GcodeTableModel::setCommandData(QList<GcodeCommand*> items)
{
    mData = items;
}
