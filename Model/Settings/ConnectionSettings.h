/*
 * Connection.h
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

class ConnectionSettings : public AbstractDatabaseRecord
{
public:
    ConnectionSettings(Profile* parent, int id = -1);
    ~ConnectionSettings();

    int getProfileID() const;

    QString getSerialPort() const;
    void setSerialPort(const QString& serialPort);

    int getSerialBaudRate() const;
    void setSerialBaudRate(int serialBaudRate);

    bool getIgnoreErrorMessages() const;
    void setIgnoreErrorMessages(bool ignoreErrorMessages);

    bool getSetParserState() const;
    void setSetParserState(bool setParserState);

    bool getArcApproximation() const;
    void setArcApproximation(bool arcApproximation);

    float getArcApproximationLength() const;
    void setArcApproximationLength(float arcApproximationLength);

    float getArcApproximationDegrees() const;
    void setArcApproximationDegrees(float arcApproximationDegrees);

    Profile* getParentHandle() const;

private:
    Profile* mParentHandle;
    QString mSerialPort;
    int mSerialBaudRate;
    bool mIgnoreErrorMessages;
    bool mSetParserState;
    bool mArcApproximation;
    float mArcApproximationLength;
    float mArcApproximationDegrees;
};
