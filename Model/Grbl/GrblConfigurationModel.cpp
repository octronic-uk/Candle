/*
 * GrblConfigurationModel.cpp
 *
 * Created: 18 2018 by Ashley
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
#include "GrblConfigurationModel.h"

GrblConfigurationModel::GrblConfigurationModel(QObject* parent)
    : QAbstractTableModel(parent)
{
   setupHeader();
   setupFields();
}

GrblConfigurationModel::~GrblConfigurationModel()
{

}

void GrblConfigurationModel::setupHeader()
{
   mHeaders << "$ Parameter" << "Field" << "Value";
}

void GrblConfigurationModel::setupFields()
{
    mFields.insert(0,"Step pulse, (µs)");
    mFields.insert(1,"Step idle delay, (ms)");
    mFields.insert(2,"Step port invert (bit-mask)");
    mFields.insert(3,"Direction port invert (bit-mask)");
    mFields.insert(4,"Step enable invert (boolean)");
    mFields.insert(5,"Limit pins invert (boolean)");
    mFields.insert(6,"Probe pin invert (boolean)");
    mFields.insert(10,"Status report (bit-mask)");
    mFields.insert(11,"Junction deviation (mm)");
    mFields.insert(12,"Arc tolerance (mm)");
    mFields.insert(13,"Report inches (boolean)");
    mFields.insert(20,"Soft limits (boolean)");
    mFields.insert(21,"Hard limits (boolean)");
    mFields.insert(22,"Homing cycle (boolean)");
    mFields.insert(23,"Homing dir invert (bit-mask)");
    mFields.insert(24,"Homing feed (mm/min)");
    mFields.insert(25,"Homing seek (mm/min)");
    mFields.insert(26,"Homing debounce (ms)");
    mFields.insert(27,"Homing pull-off (mm)");
    mFields.insert(30,"Max spindle speed (RPM)");
    mFields.insert(31,"Min spindle speed (RPM)");
    mFields.insert(32,"Laser mode (boolean)");

    mFields.insert(100,"X (steps/mm)");
    mFields.insert(101,"Y (steps/mm)");
    mFields.insert(102,"Z (steps/mm)");

    mFields.insert(110,"X Max rate (mm/min)");
    mFields.insert(111,"Y Max rate (mm/min)");
    mFields.insert(112,"Z Max rate (mm/min)");

    mFields.insert(120,"X Acceleration (mm/sec^2)");
    mFields.insert(121,"Y Acceleration (mm/sec^2)");
    mFields.insert(122,"Z Acceleration (mm/sec^2)");

    mFields.insert(130,"X Max travel (mm)");
    mFields.insert(131,"Y Max travel (mm)");
    mFields.insert(132,"Z Max travel (mm)");
}

int GrblConfigurationModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return mFields.count();
}

int GrblConfigurationModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return mHeaders.count();
}

QVariant GrblConfigurationModel::data(const QModelIndex& index, int role) const
{
   if (role == Qt::DisplayRole || role == Qt::EditRole)
   {
        switch (index.column())
        {
            case 0: // Index
                if (index.row() < 0 || index.row() > mFields.keys().count()-1)
                {
                    return QVariant();
                }
                else
                {
                    return "$" + QString::number(mFields.keys().at(index.row()));
                }
            case 1: // Field
                if (index.row() < 0 || index.row() > mFields.keys().count()-1)
                {
                    return QVariant();
                }
                else
                {
                    return mFields[mFields.keys().at(index.row())];
                }
            case 2: // Data
                if (index.row() < 0 || index.row() > mData.keys().count()-1)
                {
                    return QVariant();
                }
                else
                {
                     return mData[mFields.keys().at(index.row())];
                }
        }
    }
   return QVariant();
}

bool GrblConfigurationModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role == Qt::EditRole)
    {
        switch (index.column())
        {
            case 2: // Value
                mData[mFields.keys().at(index.row())] = value.toString();
                return true;
        }
    }
    emit dataChanged(index,index);
    return true;
}

QVariant GrblConfigurationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation)

    if (role == Qt::DisplayRole)
    {
        if (section >= 0 && section < mHeaders.count())
        {
            return mHeaders.at(section);
        }
    }
    return QVariant();
}

Qt::ItemFlags GrblConfigurationModel::flags(const QModelIndex& index) const
{
    switch (index.column())
    {
        case 2: // Value
            return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    }
    return QAbstractTableModel::flags(index);
}

QMap<int, QString> GrblConfigurationModel::getData()
{
   return mData;
}

void GrblConfigurationModel::onFirmwareConfigurationRead(int param, QString value)
{
    mData[param] = value;
    emit dataChanged(QModelIndex(),QModelIndex());
}
