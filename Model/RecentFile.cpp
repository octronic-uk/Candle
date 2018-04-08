/*
 * RecentFile.cpp
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
#include "RecentFile.h"

RecentFile::RecentFile(QString path)
    : AbstractDatabaseRecord (-1),
      mPath(path),
      mIndex(-1),
      mProfileId(-1)
{

}

RecentFile::RecentFile(int id, int index, int profile, QString path)
    : AbstractDatabaseRecord (id),
      mPath(path),
      mIndex(index),
      mProfileId(profile)
{}

RecentFile::RecentFile(const RecentFile& other)
    : AbstractDatabaseRecord (other.getID()),
      mPath(other.mPath),
      mIndex(other.mIndex),
      mProfileId(other.mProfileId)
{}

bool RecentFile::operator==(const RecentFile& other)
{
   return mPath == other.mPath;
}

QString RecentFile::getPath() const
{
    return mPath;
}

void RecentFile::setPath(const QString& path)
{
    mPath = path;
}

int RecentFile::getIndex() const
{
    return mIndex;
}

void RecentFile::setIndex(int index)
{
    mIndex = index;
}

int RecentFile::getProfileId() const
{
    return mProfileId;
}

void RecentFile::setProfileId(int profileId)
{
    mProfileId = profileId;
}
