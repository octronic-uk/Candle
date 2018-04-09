/*
 * Profile.h
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

class Profile : public AbstractDatabaseRecord
{
public:
    Profile(int id = -1, QString name = "", bool selected=false);
    ~Profile();

    Profile(const Profile& other);

    QString getName() const;
    void setName(const QString& name);

    bool getSelected() const;
    void setSelected(bool selected);

private:
    QString mName;
    bool mSelected;
};
