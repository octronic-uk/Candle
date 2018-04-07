// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GcodeTableModel.h"
#include <QtDebug>

GcodeTableModel::GcodeTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    //qDebug() << "GcodeTableModel: Constructing";
    mHeaders /*<< "#" not needed, on the left already */
            << "Command"
            << "State"
            << "Response"
            << "Line"
            << "Args";
}

GcodeTableModel::~GcodeTableModel()
{
   //qDebug() << "GcodeTableModel: Destructing";
   clear();
}

QVariant GcodeTableModel::data(const QModelIndex &index, int role) const
{
    //qDebug() << "GcodeTableModel: data(const QModelIndex &index "
             //<< index << " , int role" << role << " ) const";

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
        case 0: // Command
            return mData.at(index.row()).getCommand();
        case 1: // Status
            if (index.row() == rowCount() - 1)
            {
                return QString();
            }

            switch (mData.at(index.row()).getState())
            {
                case GcodeCommandState::InQueue:
                    return tr("In Queue");
                case GcodeCommandState::Sent:
                    return tr("Sent");
                case GcodeCommandState::Processed:
                    return tr("Processed");
                case GcodeCommandState::Skipped:
                    return tr("Skipped");
                default:
                    return tr("Unknown");
            }
        case 2: // Response
            return QVariant(mData.at(index.row()).getResponse().getData());
        case 3: // Line
            return QVariant(mData.at(index.row()).getLine());
        case 4: // Args
            if (mData.at(index.row()).getArgs().empty())
            {
               return QVariant("---");
            }
            else
            {
                return QVariant(mData.at(index.row()).getArgs());
            }
        default:
            return QVariant();
        }
    }

    if (role == Qt::TextAlignmentRole)
    {
        switch (index.column())
        {
            case 2:
                // Center empty responses
                if (mData.at(index.row()).getResponse().getData() == "")
                {
                    return QVariant::fromValue(Qt::AlignCenter | Qt::AlignVCenter);
                }
                break;
            case 4:
                if (mData.at(index.row()).getArgs().empty())
                {
                   return QVariant::fromValue(Qt::AlignCenter | Qt::AlignVCenter);
                }
                break;
        }
        return QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
    }

    //qDebug() << "GcodeTableModel: Returning empty GcodeCommand for role" << role;
    return QVariant();
}

bool GcodeTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role)

    //qDebug() << "GcodeTableModel: setData";
    if (index.isValid())
    {
        qDebug() << "GcodeTableModel: Index IS valid"
                 << index
                << "Updating...";
        switch (index.column())
        {
        case 0:
            mData[index.row()].setCommand(value.toString());
            break;
        case 1:
            mData[index.row()].setState(value.value<GcodeCommandState>());
            break;
        case 2:
            mData[index.row()].setResponse(value.value<GrblResponse>());
            break;
        case 3:
            mData[index.row()].setLine(value.toInt());
            break;
        case 4:
            mData[index.row()].setArgs(value.toStringList());
            break;
        }
        emit dataChanged(index, index);
    }
    else
    {
        qDebug() << "GcodeTableModel: Index not valid " << index;
    }
    return true;
}

bool GcodeTableModel::insertRow(int row, const QModelIndex &parent)
{
    //qDebug() << "GcodeTableModel: insertRow";
    if (row > rowCount())
    {
        return false;
    }
    beginInsertRows(parent, row, row);
    mData.insert(row, GcodeCommand());
    endInsertRows();
    return true;
}

bool GcodeTableModel::removeRow(int row, const QModelIndex &parent)
{
    //qDebug() << "GcodeTableModel: removeRow";
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
    //qDebug() << "GcodeTableModel: removeRows";
    beginRemoveRows(parent, row, row + count - 1);
    mData.erase(mData.begin() + row, mData.begin() + row + count);
    endRemoveRows();
    return true;
}

void GcodeTableModel::clear()
{
    //qDebug() << "GcodeTableModel: clear";
    beginResetModel();
    mData.clear();
    endResetModel();
}

int GcodeTableModel::rowCount(const QModelIndex &parent) const
{
    //qDebug() << "GcodeTableModel: rowCount" << mData.count();
    //qDebug() << "GcodeTableModel: size" << mData.size() << "count" << mData.count();
    Q_UNUSED(parent)
    return mData.count();
}

int GcodeTableModel::columnCount(const QModelIndex &parent) const
{
    //qDebug() << "GcodeTableModel: columnCount" << mHeaders.count();
    Q_UNUSED(parent)
    return mHeaders.count();
}

QVariant GcodeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //qDebug() << "GcodeTableModel: headerData section" << section << "for" << orientation << "role" << role;
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
    //qDebug() << "GcodeTableModel: flags" << index;
    if (!index.isValid())
    {
        //qDebug() << "GcodeTableModel: requested flags for invalid index";
        return Qt::ItemIsEnabled;
    }
    else if (index.column() == 1)
    {
        //qDebug() << "GcodeTableModel: requested flags for col 1";
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }
    //qDebug() << "GcodeTableModel: Requested flags index.column() != 1" << index.column();
    return QAbstractTableModel::flags(index);
}

QList<GcodeCommand>& GcodeTableModel::data()
{
    //qDebug() << "GcodeTableModel: data()";
    return mData;
}

void GcodeTableModel::setCommandData(QList<GcodeCommand> &items)
{
    mData = items;
    /*j
    for (int i=0; i<mData.count(); i++)
    {
        qDebug() << "GcodeTableModel:" << i << "has index" << mData[i].getTableIndex();
    }
    */
}
