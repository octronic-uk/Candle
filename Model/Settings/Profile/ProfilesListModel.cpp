/*
 * ProfilesListModel.cpp
 *
 * Created: 08 2018 by Ashley
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
#include "ProfilesListModel.h"

ProfilesListModel::ProfilesListModel(QObject *parent)
    : QAbstractListModel (parent) {}

ProfilesListModel::~ProfilesListModel() {}

int ProfilesListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return mData.count();
}

QVariant ProfilesListModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return mData.at(index.row())->getName();
    }
    return QVariant();
}

Profile* ProfilesListModel::createNewProfile()
{
    auto p = QSharedPointer<Profile>::create();
    insert(p);
    return p.data();
}

Profile* ProfilesListModel::get(int index)
{
   return mData.at(index).data();
}

void ProfilesListModel::setSelected(Profile* profile)
{
   for (auto p : mData)
   {
       if (profile == p)
       {
           p->setSelected(true);
       }
       else
       {
           p->setSelected(false);
       }
   }
}

QList<Profile*> ProfilesListModel::getDataHandles()
{
   QList<Profile*> handles;
   for (auto profile : mData)
   {
       handles.append(profile.data());
   }
   return handles;
}

void ProfilesListModel::insert(QSharedPointer<Profile> profile)
{
    insertRows(mData.count(),1,QModelIndex());
    mData.insert(mData.count(),profile);
    emit dataChanged(QModelIndex(), QModelIndex());
}

Profile* ProfilesListModel::getCurrentProfileHandle()
{
    for (QSharedPointer<Profile> p : mData)
    {
        if (p->getSelected())
        {
            return p.data();
        }
    }
    return nullptr;
}

