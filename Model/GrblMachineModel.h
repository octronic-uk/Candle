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
#include "Model/GrblMachineState.h"
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

    int bufferLengthInUse();
    bool sendNextCommandFromQueue();
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
    void restoreOffsets(GrblMachineState& state);
    void storeOffsets(GrblMachineState& state);
    void restoreParserState();
    void clearCommandQueue();
    void clearCommandBuffer();

    QVector3D getMachinePosition();
    float getMachinePositionX();
    float getMachinePositionY();
    float getMachinePositionZ();
    QVector3D getWorkPosition();
    float getWorkPositionX();
    float getWorkPositionY();
    float getWorkPositionZ();

    void queueCommand(GcodeCommand* command);
    static QString stateToString(GrblMachineState state);

signals:
    void updateProgramTableStatusSignal(GcodeCommand* state);
    void serialPortErrorSignal(QString errorMessage);
    void statusBarUpdateSignal(QString status);
    void statusTextUpdateSignal(QString status, QColor = QColor("Black"), QColor = QColor("White"));
    void appendResponseToConsoleSignal(const GrblResponse&);
    void appendCommandToConsoleSignal(GcodeCommand*);
    void updateMachinePositionSignal(const QVector3D);
    void updateWorkPositionSignal(const QVector3D);
    void spindleSpeedChangedSignal(int speed);
    void jobCompletedSignal();
    void toolPositionChangedSignal(QVector3D);
    void commandResponseSignal(const GrblResponse&);
    void machineStateUpdatedSignal(const GrblMachineState&);
    void setCompletionProgressSignal(int);
    void setBufferProgressSignal(int);

public slots:
    void onSendProgram(GcodeFileModel &gcodeFile);
    void onSendProgramFromLine(GcodeFileModel &gcodeFile,long);
    void onSettingChanged(QString group, QString param, QVariant value);
    void onGcodeCommandManualSend(GcodeCommand*);

private slots:
    void onConnectionTimer();
    void onSerialPortReadyRead();
    void onSerialPortError(QSerialPort::SerialPortError);
    void onProgramSendTimerTimeout();

private: // Members
    const static int BUFFER_LENGTH_LIMIT;
    QSerialPort mSerialPort;
    QList<GcodeCommand*> mCommandBuffer;
    QList<GcodeCommand*> mCommandQueue;
    GrblMachineState mState;
    GrblMachineState mLastState;
    QVector3D mMachinePosition;
    QVector3D mWorkPosition;
    bool mStatusReceived;
    bool mFileEndSent;
    double mFeedOverrideRate;
    bool mProcessingFile;
    bool mTransferCompleted;
    bool mAborting;
    bool mAbsoluteCoordinates;
    QTimer mConnectionTimer;
    QTimer mProgramSendTimer;
    int mProgramSendInterval;
    int mCountProcessedCommands;

private: // Member Functions
    GcodeCommand* feedOverride(GcodeCommand *command, double overridePercent);
    GcodeCommand* getNextCommand(GcodeFileModel& gcodeFile);
    void processResponse(const GrblResponse& data);
    void updateMachineCoordinates(const GrblResponse& data);
    void updateWorkCoordinates(const GrblResponse& data);
    void updateStatus(const GrblResponse& data);
    void updateToolCoordinates();
    void setupSerialPort();
    bool isSpaceInBuffer(GcodeCommand* cmd);
    void startProgramSendTimer();
    void stopProgramSendTimer();
    double getProcessedPercent();
};
