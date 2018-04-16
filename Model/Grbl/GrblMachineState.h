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

#include <QMetaType>
#include <QString>

enum class GrblMachineState
{
    Unknown = 0,
    Idle,
    Alarm,
    Run,
    Home,
    Hold,
    Queue,
    Check,
    Door,
    Locked,
    Unlocked
};

static QString stateToString(GrblMachineState state)
{
    switch (state)
    {
        case GrblMachineState::Unknown:
            return "Unknown";
        case GrblMachineState::Idle:
            return "Idle";
        case GrblMachineState::Alarm:
            return "Alarm";
        case GrblMachineState::Run:
            return "Run";
        case GrblMachineState::Home:
            return "Home";
        case GrblMachineState::Hold:
            return "Hold";
        case GrblMachineState::Queue:
            return "Queue";
        case GrblMachineState::Check:
            return "Check";
        case GrblMachineState::Door:
            return "Door";
        case GrblMachineState::Locked:
            return "Locked";
        case GrblMachineState::Unlocked:
            return "Unlocked";
    }
    return "Unknown";
}

static GrblMachineState stateFromString(QString stateStr)
{
    if (stateStr.toUpper() == "IDLE")
    {
        return GrblMachineState::Idle;
    }
    else if (stateStr.toUpper() == "ALARM")
    {
        return GrblMachineState::Alarm;
    }
    else if (stateStr.toUpper() == "RUN")
    {
        return GrblMachineState::Run;
    }
    else if (stateStr.toUpper() == "HOME")
    {
        return GrblMachineState::Home;
    }
    else if (stateStr.toUpper() == "HOLD")
    {
        return GrblMachineState::Hold;
    }
    else if (stateStr.toUpper() == "QUEUE")
    {
        return GrblMachineState::Queue;
    }
    else if (stateStr.toUpper() == "CHECK")
    {
        return GrblMachineState::Check;
    }
    else if (stateStr.toUpper() == "DOOR")
    {
        return GrblMachineState::Door;
    } else if (stateStr.toUpper() == "LOCKED")
    {
        return GrblMachineState::Locked;
    }
    else if (stateStr.toUpper() == "UNLOCKED")
    {
        return GrblMachineState::Unlocked;
    }

    return GrblMachineState::Unknown;
}

Q_DECLARE_METATYPE(GrblMachineState)
