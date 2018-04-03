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
#include <QTimer>
#include <QColor>
#include "Utils/MachineState.h"
#include <QVector3D>
#include "Model/GcodeFileModel.h"
#include "Model/GcodeCommand.h"
#include "Model/GrblResponse.h"

class GrblMachineModel : public QObject
{
    Q_OBJECT
public:
    explicit GrblMachineModel(QObject *parent = nullptr);
    ~GrblMachineModel();

    bool openPort();
    bool closePort();

    int bufferLength();
    bool sendCommand(GcodeCommand command);
    void grblReset();
    bool isPortOpen();
    bool isStatusReceived();
    void setStatusReceived(bool);
    void write(QByteArray);
    int commandsQueueLength();
    QString getPortName();
    void setPortName(QString name);
    void setBaudRate(int baud);
    int getBaudRate();
    void restoreOffsets(MachineState& state);
    void storeOffsets(MachineState& state);
    void restoreParserState();
    void clearCommandsQueue();
    void clearCommandsList();
    void resetCommandIndex();
    void setCommandIndex(int index);
    int getCommandIndex();

signals:
    void updateProgramTableStatusSignal(GcodeCommand state);
    void serialPortErrorSignal(QString errorMessage);
    void statusBarUpdateSignal(QString status);
    void statusTextUpdateSignal(QString status, QColor = QColor("Black"), QColor = QColor("White"));
    void processingFileSignal(bool);
    void transferCompletedSignal(bool);
    void fileCommandIndexSignal(int);
    void resetingSignal(bool);
    void homingSignal(bool);
    void resetCompletedSignal(bool);
    void updateSpindleSpeedSignal(bool);
    void grblStatusSignal(int);
    void statusReceivedSignal(bool);
    void appendResponseToConsoleSignal(GrblResponse);
    void appendCommandToConsoleSignal(GcodeCommand);
    void updateMachinePositionSignal(QVector3D);
    void updateWorkPositionSignal(QVector3D);
    void spindleSpeedChangedSignal(int speed);
    void setInterfaceLockSignal(bool locked);
    void jobCompletedSignal();
    void toolPositionChangedSignal(QVector3D);

public slots:
    void onSendNextFileCommands(GcodeFileModel &gcodeFile);
    void onSettingChanged(QString group, QString param, QVariant value);
    void onGcodeCommandSend(GcodeCommand);

private slots:
    void onConnectionTimer();
    void onSerialPortReadyRead();
    void onSerialPortError(QSerialPort::SerialPortError);

private: // Members
    const static int BUFFER_LENGTH;
    QSerialPort mSerialPort;
    QList<GcodeCommand> mCommandsHistoryList;
    QList<GcodeCommand> mCommandsQueue;
    MachineStateEnum mLastGrblStatus;
    QVector3D mMachinePosition;
    QVector3D mWorkPosition;
    bool mHoming;
    bool mStatusReceived;
    bool mPortNameSet;
    bool mBaudRateSet;
    bool mResetting;
    bool mFileEndSent;
    double mFeedOverrideRate;
    int  mCommandIndex;
    bool mProcessingFile;
    bool mTransferCompleted;
    bool mAborting;
    bool mAbsoluteCoordinates;
    bool mLocked;
    QTimer mConnectionTimer;

private: // Member Functions
    QString feedOverride(GcodeCommand command, double overridePercent);
    QString getNextCommand(GcodeFileModel& gcodeFile);

    void processResponse(GrblResponse data);
    void updateMachineCoordinates(GrblResponse data);
    void updateWorkCoordinates(GrblResponse data);
    void updateStatus(GrblResponse data);
    void updateToolCoordinates(GrblResponse);

    void setupSerialPort();
    void onSerialResponseUnprocessed(QString data);
    void onSerialResponseProcessed(GcodeCommand response);
    void onSerialResponseStatus(QString data);
};
