/*
 * SerialPortModel.h
 *
 * Created: 24 2018 by Ashley
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

#include <QObject>
#include <QtSerialPort>

class SerialPortModel : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortModel(QObject *parent = nullptr);
    ~SerialPortModel();

    bool openPort();
    bool closePort();

    int bufferLength();
    void sendCommand(QString command, int tableIndex = -1, bool showInConsole = true);
    void sendNextFileCommands();

    void grblReset();
    bool isPortOpen();

    bool isStatusReceived();
    void setStatusReceived(bool);
    void write(QByteArray);


    QString getPortName();
    void setPortName(QString name);

    void setBaudRate(int baud);
    int getBaudRate();

    void restoreOffsets();
    void storeOffsets();
    void restoreParserState();
signals:
    void serialPortErrorSignal(QString errorMessage);
    void statusUpdateSignal(QString status);

public slots:
    void onTimerConnection();

private slots:
    void onSerialPortReadyRead();
    void onSerialPortError(QSerialPort::SerialPortError);

private:
    QSerialPort mSerialPort;
    bool mStatusReceived;

};
