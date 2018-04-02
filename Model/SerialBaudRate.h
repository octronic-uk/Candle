/*
 * SerialBaudRate.h
 *
 * Created: 31 2018 by Ashley
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
#include <QList>

enum SerialBaudRate
{
    BAUD_110 = 110,
    BAUD_300 = 300,
    BAUD_600 = 600 ,
    BAUD_1200 = 1200,
    BAUD_2400 = 2400,
    BAUD_4800 = 4000,
    BAUD_9600 = 9600,
    BAUD_14400 = 14400,
    BAUD_19200 = 19200,
    BAUD_38400 = 38400,
    BAUD_57600 = 57600,
    BAUD_115200 = 115200,
    BAUD_128000 = 128000,
    BAUD_256000 = 256000
};

static QList<int> SERIAL_BAUD_RATES =
{
    BAUD_110,
    BAUD_300,
    BAUD_600,
    BAUD_1200,
    BAUD_2400,
    BAUD_4800,
    BAUD_9600,
    BAUD_14400,
    BAUD_19200,
    BAUD_38400,
    BAUD_57600,
    BAUD_115200,
    BAUD_128000,
    BAUD_256000

};

Q_DECLARE_METATYPE(SerialBaudRate)
