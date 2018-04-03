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
    int length,
    int tableIndex,
    bool showInConsole,
    int consoleIndex
)
    : mCommand(cmd),
      mLength(length),
      mTableIndex(tableIndex),
      mShowInConsole(showInConsole),
      mConsoleIndex(consoleIndex)
{

}

GcodeCommand::GcodeCommand(const GcodeCommand& other)
    : mCommand(other.mCommand),
      mLength(other.mLength),
      mTableIndex(other.mTableIndex),
      mShowInConsole(other.mShowInConsole),
      mConsoleIndex(other.mConsoleIndex)
{

}

GcodeCommand::~GcodeCommand()
{

}

GcodeCommand GcodeCommand::AbsoluteCoordinatesCommand()
{
    GcodeCommand gc("G90");
    return gc;
}

GcodeCommand GcodeCommand::ControlXCommand()
{
   GcodeCommand gc("[CTRL+X]");
   return gc;
}


GcodeCommand GcodeCommand::UnlockCommand()
{
    GcodeCommand gc("$X");
    return gc;
}

QString GcodeCommand::getCommand() const
{
    return mCommand;
}

void GcodeCommand::setCommand(const QString& command)
{
    mCommand = command.toUpper();
}

int GcodeCommand::getLength() const
{
    return mLength;
}

void GcodeCommand::setLength(int length)
{
    mLength = length;
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

bool GcodeCommand::isParserStateResponse()
{
    return mCommand == "$G";
}

bool GcodeCommand::isParametersResponse()
{
    return mCommand == "$#";
}

bool GcodeCommand::isSpindleSpeedCommand()
{
    return spindleSpeedExpression.indexIn(mCommand) != -1;
}

bool GcodeCommand::isFileEndCommand()
{
    return mCommand.contains(fileEndExpression);
}

int GcodeCommand::getSpindleSpeed()
{
    if (spindleSpeedExpression.indexIn(mCommand) != -1)
    {
        return spindleSpeedExpression.cap(1).toInt();
    }
    return 0;
}

bool GcodeCommand::isCtrlXResponse()
{
    return mCommand == "[CTRL+X]";
}

bool GcodeCommand::isAbsoluteCoordinatesResponse()
{
    return mCommand.contains("G90");
}

bool GcodeCommand::isSpindleCounterClockwiseCommand()
{
    return mCommand.contains("M4");
}

bool GcodeCommand::isSpindleClockwiseCommand()
{
    return mCommand.contains("M3");
}

bool GcodeCommand::isSpindleStopCommand()
{
    return mCommand.contains("M5");
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

const QRegExp GcodeCommand::spindleSpeedExpression = QRegExp("[Ss]0*(\\d+)");
const QRegExp GcodeCommand::fileEndExpression = QRegExp("M0*2|M30");
const QRegExp GcodeCommand::feedRateExpression = QRegExp(".*F([\\d\\.]+)");
