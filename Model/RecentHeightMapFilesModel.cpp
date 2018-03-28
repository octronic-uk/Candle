/*
 * RecentHeightMapFilesModel.cpp
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
#include "RecentHeightMapFilesModel.h"
#include <QtDebug>

RecentHeightMapFilesModel::RecentHeightMapFilesModel(QObject *parent)
    : QObject(parent),
      mMaxRecent(5)
{
    qDebug() << "RecentHeightMapFilesModel: Constructing";
}

RecentHeightMapFilesModel::~RecentHeightMapFilesModel()
{
    qDebug() << "RecentHeightMapFilesModel: Destructing";
}

void RecentHeightMapFilesModel::add(QString fileName)
{
    qDebug() << "RecentHeightMapFilesModel: Adding file %s"
             << fileName;
    mRecentFiles.removeAll(fileName);
    mRecentFiles.append(fileName);
    if (mRecentFiles.count() > mMaxRecent)
    {
        mRecentFiles.takeFirst();
    }

    emit recentFilesChangedSignal();
}

void RecentHeightMapFilesModel::clear()
{
    mRecentFiles.clear();
}

int RecentHeightMapFilesModel::getMaxRecent() const
{
    return mMaxRecent;
}

void RecentHeightMapFilesModel::setMaxRecent(int maxRecent)
{
    mMaxRecent = maxRecent;
}

QStringList RecentHeightMapFilesModel::getRecentFiles()
{
   return mRecentFiles;
}
