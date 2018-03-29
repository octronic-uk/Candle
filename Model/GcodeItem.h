/*
 * GcodeItem.h
 *
 * Created: 28 2018 by Ashley
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
#include <QStringList>
#include "GcodeItemState.h"

class GcodeItem
{
public:
    explicit GcodeItem();
    ~GcodeItem();

    QString getCommand() const;
    void setCommand(const QString& command);

    QString getResponse() const;
    void setResponse(const QString& response);

    QStringList getArgs() const;
    void setArgs(const QStringList& args);

    GcodeItemState getState() const;
    void setState(GcodeItemState state);

    int getLine() const;
    void setLine(int line);

private:
    QString mCommand;
    QString mResponse;
    QStringList mArgs;
    GcodeItemState mState;
    int mLine;
};


