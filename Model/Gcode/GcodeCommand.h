/*
 * GcodeCommand.h
 *
 * Created: 02 2018 by Ashley
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
 *
 *
 *
 * Every string Grbl receives is assumed to be a G-code block/line for it to
 * execute, except for some special system commands Grbl uses for configuration,
 * provide feedback to the user on what and how it's doing, or perform some task
 * such as a homing cycle. To see a list of these system commands, type $
 * followed by an enter, and Grbl will respond with:
 *
 * $$ (view Grbl settings)
 * $# (view # parameters)
 * $G (view parser state)
 * $I (view build info)
 * $N (view startup blocks)
 * $x=value (save Grbl setting)
 * $Nx=line (save startup block)
 * $C (check gcode mode)
 * $X (kill alarm lock)
 * $H (run homing cycle)
 * ~ (cycle start)
 * ! (feed hold)
 * ? (current status)
 * ctrl-x (reset Grbl)
 *
 */

#pragma once

#include <QString>
#include <QStringList>
#include <QMetaType>
#include "Model/Grbl/GrblResponse.h"

enum GcodeCommandState
{
    None,
    Marker,
    InQueue,
    Sent,
    Processed,
    Skipped
};


Q_DECLARE_METATYPE(GcodeCommandState)

class GcodeCommand
{
public:
    GcodeCommand(
        QString cmd = "",
        int tableIndex = -1,
        bool showInConsole = true,
        int consoleIndex = -1,
        GcodeCommandState state = GcodeCommandState::InQueue
    );

    GcodeCommand(const GcodeCommand& other);
    GcodeCommand(const GcodeCommand* other);
    GcodeCommand(char rawCmd);
    ~GcodeCommand();

    static long ID;
    static GcodeCommand* AbsoluteCoordinatesCommand();
    static GcodeCommand* ControlXCommand();
    static GcodeCommand* UnlockCommand();
    static GcodeCommand* StatusUpdateCommand();
    static GcodeCommand* ResetCommand();
    static GcodeCommand* SpindleCounterClockwiseCommand();
    static GcodeCommand* SpindleClockwiseCommand();
    static GcodeCommand* SpindleStopCommand();
    static GcodeCommand* HomingCommand();
    static GcodeCommand* ZeroXYCommand();
    static GcodeCommand* GetGcodeParamsCommand();
    static GcodeCommand* ZeroZCommand();
    static GcodeCommand* CyclePauseResume();
    static GcodeCommand* StopCommand();

    static GcodeCommand* JogCommand
    (
        double x, double y, double z,
        int feedRate, bool inches, bool machineCoordinates
    );

    bool operator==(const GcodeCommand& other);

    bool isEmpty();

    int getCommandLength() const;

    QString getCommand() const;
    void setCommand(const QString& command);

    int getTableIndex() const;
    void setTableIndex(int tableIndex);

    bool getShowInConsole() const;
    void setShowInConsole(bool showInConsole);

    int getConsoleIndex() const;
    void setConsoleIndex(int consoleIndex);

    GrblResponse getResponse() const;
    void setResponse(const GrblResponse& response);

    QStringList getArgs() const;
    void setArgs(const QStringList& args);

    GcodeCommandState getState() const;
    void setState(GcodeCommandState state);

    int getLine() const;
    void setLine(int line);

    bool isRawCommand();

    long getID();
    bool hasID(long id);
    char getRawCommand();

    QString getMarker() const;
    void setMarker(const QString marker);

private:
    static long nextID();
    char mRawCommand;
    QString mCommand;
    GrblResponse mResponse;
    GcodeCommandState mState;
    QStringList mArgs;
    long mID;
    int mLine;
    int mTableIndex;
    int mConsoleIndex;
    bool mShowInConsole;
    QString mMarker;
};
