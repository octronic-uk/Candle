/*
 * GcodeCommand.cpp
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
 */
#include "GcodeCommand.h"
#include <QRegExp>

GcodeCommand::GcodeCommand
(
        QString cmd,
        int tableIndex,
        bool showInConsole,
        int consoleIndex,
        GcodeCommandState state
        )
    : mRawCommand(0),
      mCommand(cmd),
      mResponse(GrblResponse()),
      mState(state),
      mArgs(QStringList()),
      mID(nextID()),
      mLine(-1),
      mTableIndex(tableIndex),
      mConsoleIndex(consoleIndex),
      mShowInConsole(showInConsole) {}

GcodeCommand::GcodeCommand(const GcodeCommand& other)
    : mRawCommand(other.mRawCommand),
      mCommand(other.mCommand),
      mResponse(other.mResponse),
      mState(other.mState),
      mArgs(other.mArgs),
      mID(other.mID),
      mLine(other.mLine),
      mTableIndex(other.mTableIndex),
      mConsoleIndex(other.mConsoleIndex),
      mShowInConsole(other.mShowInConsole) {}

GcodeCommand::GcodeCommand(const GcodeCommand* other)
    : mRawCommand(other->mRawCommand),
      mCommand(other->mCommand),
      mResponse(other->mResponse),
      mState(other->mState),
      mArgs(other->mArgs),
      mID(other->mID),
      mLine(other->mLine),
      mTableIndex(other->mTableIndex),
      mConsoleIndex(other->mConsoleIndex),
      mShowInConsole(other->mShowInConsole) {}


GcodeCommand::GcodeCommand(char rawCmd) : mRawCommand(rawCmd) {}

GcodeCommand::~GcodeCommand() {}

GcodeCommand* GcodeCommand::AbsoluteCoordinatesCommand()
{
    static GcodeCommand gc("G90");
    return &gc;
}

GcodeCommand* GcodeCommand::ControlXCommand()
{
    static GcodeCommand gc("[CTRL+X]");
    return &gc;
}


GcodeCommand* GcodeCommand::UnlockCommand()
{
    static GcodeCommand gc("$X");
    return &gc;
}

GcodeCommand* GcodeCommand::ResetCommand()
{
    static GcodeCommand gc(0x18);
    return &gc;
}

GcodeCommand* GcodeCommand::StatusUpdateCommand()
{
    static GcodeCommand gc("?");
    return &gc;
}

GcodeCommand* GcodeCommand::SpindleCounterClockwiseCommand()
{
    static GcodeCommand gc("M4");
    return &gc;
}

GcodeCommand* GcodeCommand::SpindleClockwiseCommand()
{
    static GcodeCommand gc("M3");
    return &gc;
}

GcodeCommand* GcodeCommand::SpindleStopCommand()
{
    static GcodeCommand gc("M5");
    return &gc;
}

bool GcodeCommand::operator==(const GcodeCommand& other)
{
    return mID == other.mID;
}

bool GcodeCommand::isEmpty()
{
    return mCommand.isEmpty();
}

int GcodeCommand::getCommandLength() const
{
    return mCommand.length();
}

QString GcodeCommand::getCommand() const
{
    return mCommand;
}

void GcodeCommand::setCommand(const QString& command)
{
    mCommand = command.toUpper();
}

int GcodeCommand::getTableIndex() const
{
    return mTableIndex;
}

void GcodeCommand::setTableIndex(int tableIndex)
{
    mTableIndex = tableIndex;
}

bool GcodeCommand::getShowInConsole() const
{
    return mShowInConsole;
}

void GcodeCommand::setShowInConsole(bool showInConsole)
{
    mShowInConsole = showInConsole;
}

int GcodeCommand::getConsoleIndex() const
{
    return mConsoleIndex;
}

void GcodeCommand::setConsoleIndex(int consoleIndex)
{
    mConsoleIndex = consoleIndex;
}

long GcodeCommand::getID()
{
    return mID;
}

bool GcodeCommand::hasID(long id)
{
    return mID == id;
}

long GcodeCommand::nextID()
{
    return ID++;
}

QString GcodeCommand::getMarker() const
{
    return mMarker;
}

void GcodeCommand::setMarker(const QString marker)
{
    mMarker = marker;
}

GrblResponse GcodeCommand::getResponse() const
{
    return mResponse;
}

void GcodeCommand::setResponse(const GrblResponse& response)
{
    mResponse = response;
}

int GcodeCommand::getLine() const
{
    return mLine;
}

void GcodeCommand::setLine(int line)
{
    mLine = line;
}

bool GcodeCommand::isRawCommand()
{
    return mRawCommand > 0;
}

QStringList GcodeCommand::getArgs() const
{
    return mArgs;
}

void GcodeCommand::setArgs(const QStringList& args)
{
    mArgs= args;
}

GcodeCommandState GcodeCommand::getState() const
{
    return mState;
}

void GcodeCommand::setState(const GcodeCommandState state)
{
    mState = state;
}

char GcodeCommand::getRawCommand()
{
    return mRawCommand;
}

long GcodeCommand::ID = 0;
