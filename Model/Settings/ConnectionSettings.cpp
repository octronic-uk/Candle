/*
 * Connection.cpp
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
#include "ConnectionSettings.h"
#include "Model/Settings/Profile/Profile.h"
#include <QtDebug>

ConnectionSettings::ConnectionSettings(Profile* parent, int id)
    : AbstractDatabaseRecord (id),
      mParentHandle(parent),
      mSerialPort("Not Set"),
      mSerialBaudRate(115200),
      mIgnoreErrorMessages(false),
      mSetParserState(false),
      mArcApproximation(false),
      mArcApproximationLength(0.0),
      mArcApproximationDegrees(0.0)
{
    qDebug() << "ConnectionSettings: Constructing";
}

ConnectionSettings::~ConnectionSettings() {}

int ConnectionSettings::getProfileID() const
{
    return mParentHandle->getID();
}

QString ConnectionSettings::getSerialPort() const
{
    return mSerialPort;
}

void ConnectionSettings::setSerialPort(const QString& serialPort)
{
    mSerialPort = serialPort;
}

int ConnectionSettings::getSerialBaudRate() const
{
    return mSerialBaudRate;
}

void ConnectionSettings::setSerialBaudRate(int serialBaudRate)
{
    mSerialBaudRate = serialBaudRate;
}

bool ConnectionSettings::getIgnoreErrorMessages() const
{
    return mIgnoreErrorMessages;
}

void ConnectionSettings::setIgnoreErrorMessages(bool ignoreErrorMessages)
{
    mIgnoreErrorMessages = ignoreErrorMessages;
}

bool ConnectionSettings::getSetParserState() const
{
    return mSetParserState;
}

void ConnectionSettings::setSetParserState(bool setParserState)
{
    mSetParserState = setParserState;
}

bool ConnectionSettings::getArcApproximation() const
{
    return mArcApproximation;
}

void ConnectionSettings::setArcApproximation(bool arcApproximation)
{
    mArcApproximation = arcApproximation;
}

float ConnectionSettings::getArcApproximationLength() const
{
    return mArcApproximationLength;
}

void ConnectionSettings::setArcApproximationLength(float arcApproximationLength)
{
    mArcApproximationLength = arcApproximationLength;
}

float ConnectionSettings::getArcApproximationDegrees() const
{
    return mArcApproximationDegrees;
}

void ConnectionSettings::setArcApproximationDegrees(float arcApproximationDegrees)
{
    mArcApproximationDegrees = arcApproximationDegrees;
}

Profile* ConnectionSettings::getParentHandle() const
{
    return mParentHandle;
}
