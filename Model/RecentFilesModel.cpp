/*
 * RecentFilesModel.cpp
 *
 * Created: 24 2018 by Ashley
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

#include "RecentFilesModel.h"
#include "Model/Settings/Profile/Profile.h"

#include <QtDebug>

RecentFilesModel::RecentFilesModel(Profile* profile, QObject *parent)
    : QObject(parent),
      mProfileHandle(profile),
      mMaxRecent(5)
{
    qDebug() << "RecentFilesModel: Constructing";
}

RecentFilesModel::~RecentFilesModel()
{
   qDebug() << "RecentFilesModel: Destructing";
}

void RecentFilesModel::clear()
{
    mRecentFiles.clear();
}

int RecentFilesModel::count()
{
   return mRecentFiles.count();
}

QList<RecentFile> RecentFilesModel::getRecentFiles()
{
   return mRecentFiles;
}

Profile* RecentFilesModel::getProfileHandle() const
{
    return mProfileHandle;
}

void RecentFilesModel::add(RecentFile file)
{
    qDebug() << "RecentFilesModel: Adding file " << file.getPath();
    mRecentFiles.removeAll(file);
    mRecentFiles.append(file);
    if (mRecentFiles.count() > mMaxRecent)
    {
        mRecentFiles.takeFirst();
    }
    emit recentFilesChangedSignal();
}
