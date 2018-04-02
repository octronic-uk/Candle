/*
 * MachineState.h
 *
 * Created: 22 2018 by Ashley
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

#include <QVector3D>
#include <QMetaType>
#include <QString>

enum MachineStateEnum
{
    Unknown = 0,
    Idle,
    Alarm,
    Run,
    Home,
    Hold,
    Queue,
    Check,
    Door
};

Q_DECLARE_METATYPE(MachineStateEnum)

class MachineState
{
public:
    MachineState()
        : mStatus(Unknown),
          mWorkPosition(QVector3D(0.0,0.0,0.0)),
          mMachinePosition(QVector3D(0.0,0.0,0.0)) {}

    ~MachineState() {}

    QVector3D getMachinePosition() { return mMachinePosition;  }
    float getMachinePositionX() { return mMachinePosition.x(); }
    float getMachinePositionY() { return mMachinePosition.y(); }
    float getMachinePositionZ() { return mMachinePosition.z(); }

    QVector3D getWorkPosition() { return mWorkPosition; }
    float getWorkPositionX() { return mWorkPosition.x(); }
    float getWorkPositionY() { return mWorkPosition.y(); }
    float getWorkPositionZ() { return mWorkPosition.z(); }

    MachineStateEnum getStatus() { return mStatus; }
    void setStatus(MachineStateEnum status) { mStatus = status; }

    static QString enumToQString(MachineStateEnum status)
    {
        switch (status)
        {
            case Unknown:
                return QString("Unknown");
            case Idle:
                return QString("Idle");
            case Alarm:
                return QString("Alarm");
            case Run:
                return QString("Running");
            case Home:
                return QString("Home");
            case Hold:
                return QString("Hold");
            case Queue:
                return QString("Queue");
            case Check:
                return QString("Check");
            case Door:
                return QString("Door");
        }
        return QString("Invalid Status");
    }

private:
    MachineStateEnum mStatus;
    QVector3D mWorkPosition;
    QVector3D mMachinePosition;
};


