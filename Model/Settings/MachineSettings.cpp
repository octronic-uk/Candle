/*
 * MachineSettings.cpp
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

#include "Model/Settings/Profile/Profile.h"
#include "MachineSettings.h"

#include <QtDebug>

MachineSettings::MachineSettings(Profile* profile,int id)
    : AbstractDatabaseRecord (id),
      mParentHandle(profile),
      mQueryPeriod(100),
      mUnits(false),
      mRapidSpeed(500),
      mAcceleration(500),
      mSpindleMin(0),
      mSpindleMax(10000),
      mProbeCmds("G21G91G38.2Z-30F100; G0Z1; G38.2Z-2F10"),
      mSafePositionCmds("G21G90; G53G0Z0"),
      mRestoreOrigin(false),
      mRestoreType(false),
      mUserCmd1(""),
      mUserCmd2(""),
      mUserCmd3(""),
      mUserCmd4(""),
      mHeightMapProbeFeed(1.0)
{

    qDebug() << "MachineSettings: Constructing";
}

Profile* MachineSettings::getParentHandle() const
{
    return mParentHandle;
}

int MachineSettings::getParentID()
{
   return mParentHandle->getID();
}

int MachineSettings::getQueryPeriod() const
{
    return mQueryPeriod;
}

void MachineSettings::setQueryPeriod(int queryPeriod)
{
    mQueryPeriod = queryPeriod;
}

bool MachineSettings::getUnits() const
{
    return mUnits;
}

void MachineSettings::setUnits(bool units)
{
    mUnits = units;
}

float MachineSettings::getRapidSpeed() const
{
    return mRapidSpeed;
}

void MachineSettings::setRapidSpeed(float rapidSpeed)
{
    mRapidSpeed = rapidSpeed;
}

float MachineSettings::getAcceleration() const
{
    return mAcceleration;
}

void MachineSettings::setAcceleration(float acceleration)
{
    mAcceleration = acceleration;
}

int MachineSettings::getSpindleMin() const
{
    return mSpindleMin;
}

void MachineSettings::setSpindleMin(int spindleMin)
{
    mSpindleMin = spindleMin;
}

int MachineSettings::getSpindleMax() const
{
    return mSpindleMax;
}

void MachineSettings::setSpindleMax(int spindleMax)
{
    mSpindleMax = spindleMax;
}

QString MachineSettings::getProbeCmds() const
{
    return mProbeCmds;
}

void MachineSettings::setProbeCmds(const QString& probeCmds)
{
    mProbeCmds = probeCmds;
}

QString MachineSettings::getSafePositionCmds() const
{
    return mSafePositionCmds;
}

void MachineSettings::setSafePositionCmds(const QString& safePositionCmds)
{
    mSafePositionCmds = safePositionCmds;
}

bool MachineSettings::getRestoreOrigin() const
{
    return mRestoreOrigin;
}

void MachineSettings::setRestoreOrigin(bool restoreOrigin)
{
    mRestoreOrigin = restoreOrigin;
}

bool MachineSettings::getRestoreType() const
{
    return mRestoreType;
}

void MachineSettings::setRestoreType(bool restoreType)
{
    mRestoreType = restoreType;
}

QString MachineSettings::getUserCmd1() const
{
    return mUserCmd1;
}

void MachineSettings::setUserCmd1(const QString& userCmd1)
{
    mUserCmd1 = userCmd1;
}

QString MachineSettings::getUserCmd2() const
{
    return mUserCmd2;
}

void MachineSettings::setUserCmd2(const QString& userCmd2)
{
    mUserCmd2 = userCmd2;
}

QString MachineSettings::getUserCmd3() const
{
    return mUserCmd3;
}

void MachineSettings::setUserCmd3(const QString& userCmd3)
{
    mUserCmd3 = userCmd3;
}

QString MachineSettings::getUserCmd4() const
{
    return mUserCmd4;
}

void MachineSettings::setUserCmd4(const QString& userCmd4)
{
    mUserCmd4 = userCmd4;
}

int MachineSettings::getHeightMapProbeFeed() const
{
    return mHeightMapProbeFeed;
}

void MachineSettings::setHeightMapProbeFeed(int heightMapProbeFeed)
{
    mHeightMapProbeFeed = heightMapProbeFeed;
}
