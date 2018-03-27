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

RecentHeightMapFilesModel::RecentHeightMapFilesModel(QObject *parent) : QObject(parent)
{

}

RecentHeightMapFilesModel::~RecentHeightMapFilesModel()
{

}

void RecentHeightMapFilesModel::addRecentHeightmap(QString fileName)
{
    mRecentHeightmaps.removeAll(fileName);
    mRecentHeightmaps.append(fileName);
    if (mRecentHeightmaps.count() > 5) mRecentHeightmaps.takeFirst();
}
