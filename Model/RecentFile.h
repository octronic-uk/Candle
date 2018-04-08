/*
 * RecentFile.h
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

#pragma once

#include "Model/Settings/Sql/AbstractDatabaseRecord.h"
#include <QString>

class RecentFile : AbstractDatabaseRecord
{
public:
    RecentFile(QString);
    RecentFile(int id, int index, int profileId, QString path);
    RecentFile(const RecentFile& other);

    bool operator==(const RecentFile& other);

    QString getPath() const;
    void setPath(const QString& path);

    int getIndex() const;
    void setIndex(int index);

    int getProfileId() const;
    void setProfileId(int profileId);

private:
    QString mPath;
    int mIndex;
    int mProfileId;
};
