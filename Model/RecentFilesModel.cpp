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

#include <QtDebug>

RecentFilesModel::RecentFilesModel(QObject *parent)
    : QObject(parent),
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

QStringList RecentFilesModel::getRecentFiles()
{
   return mRecentFiles;
}

void RecentFilesModel::add(QString fileName)
{
    qDebug() << "RecentFilesModel: Adding file " << fileName;
    mRecentFiles.removeAll(fileName);
    mRecentFiles.append(fileName);
    if (mRecentFiles.count() > mMaxRecent)
    {
        mRecentFiles.takeFirst();
    }
    emit recentFilesChangedSignal();
}
