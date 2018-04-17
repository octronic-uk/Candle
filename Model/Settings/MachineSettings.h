/*
 * MachineSettings.h
 *
 * Created: 10 2018 by Ashley
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

class Profile;
class MachineSettings : public AbstractDatabaseRecord
{
public:
    MachineSettings(Profile* parent, int id = -1);

    Profile* getParentHandle() const;
    int getParentID();

    int getQueryPeriod() const;
    void setQueryPeriod(int queryPeriod);

    bool getUnits() const;
    void setUnits(bool units);

    float getRapidSpeed() const;
    void setRapidSpeed(float rapidSpeed);

    float getAcceleration() const;
    void setAcceleration(float acceleration);

    int getSpindleMin() const;
    void setSpindleMin(int spindleMin);

    int getSpindleMax() const;
    void setSpindleMax(int spindleMax);

    QString getProbeCmds() const;
    void setProbeCmds(const QString& probeCmds);

    QString getSafePositionCmds() const;
    void setSafePositionCmds(const QString& safePositionCmds);

    bool getRestoreOrigin() const;
    void setRestoreOrigin(bool restoreOrigin);

    bool getRestoreType() const;
    void setRestoreType(bool restoreType);

    QString getUserCmd1() const;
    void setUserCmd1(const QString& userCmd1);

    QString getUserCmd2() const;
    void setUserCmd2(const QString& userCmd2);

    QString getUserCmd3() const;
    void setUserCmd3(const QString& userCmd3);

    QString getUserCmd4() const;
    void setUserCmd4(const QString& userCmd4);

    int getHeightMapProbeFeed() const;
    void setHeightMapProbeFeed(int heightMapProbeFeed);

    int getWorkAreaWidth() const;
    void setWorkAreaWidth(int workAreaWidth);

    int getWorkAreaHeight() const;
    void setWorkAreaHeight(int workAreaHeight);

private:
    Profile* mParentHandle;
    int mQueryPeriod;
    bool mUnits;
    float mRapidSpeed;
    float mAcceleration;
    int mSpindleMin;
    int mSpindleMax;
    QString mProbeCmds;
    QString mSafePositionCmds;
    bool mRestoreOrigin;
    bool mRestoreType;
    QString mUserCmd1;
    QString mUserCmd2;
    QString mUserCmd3;
    QString mUserCmd4;
    int mHeightMapProbeFeed;
    int mWorkAreaWidth;
    int mWorkAreaHeight;
};
