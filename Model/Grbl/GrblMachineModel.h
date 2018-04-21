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

#include <map>
#include <QObject>
#include <QtSerialPort>
#include <QTimer>
#include <QColor>
#include "Model/Grbl/GrblMachineState.h"
#include <QVector3D>
#include "Model/Gcode/GcodeFileModel.h"
#include "Model/Gcode/GcodeCommand.h"
#include "Model/Grbl/GrblResponse.h"

class SqlSettingsModel;

using namespace std;

class GrblMachineModel : public QObject
{
    Q_OBJECT
public:
    explicit GrblMachineModel(QObject *parent = nullptr);
    ~GrblMachineModel();

    bool openPort();
    bool closePort();
    void initialise();
    int bufferLengthInUse();
    bool sendNextCommandFromQueue();
    bool isPortOpen();
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
    void setSettingsModelHandle(SqlSettingsModel* settingsModelHandle);

signals:
    void updateProgramTableStatusSignal(GcodeCommand* state);

    void updateFeedOverrideSignal(float);
    void updateRapidOverrideSignal(float);
    void updateSpindleOverrideSignal(float);

    void updateMachinePositionSignal(const QVector3D);
    void updateWorkPositionSignal(const QVector3D);
    void updateWCOSignal(const QVector3D);

    void serialPortErrorSignal(QString errorMessage);

    void statusBarUpdateSignal(QString status);

    void toolPositionChangedSignal(QVector3D);
    void machineStateUpdatedSignal(const GrblMachineState&);
    void jobCompletedSignal();
    void alarmSignal(QString);

    void appendResponseToConsoleSignal(const GrblResponse&);
    void appendCommandToConsoleSignal(GcodeCommand*);
    void commandResponseSignal(const GrblResponse&);
    void setCompletionProgressSignal(int);
    void setBufferProgressSignal(int);
    void errorSignal(QString);
    void machineConnectedSigal(bool);

    void firmwareConfigurationReadSignal(int,QString);
    void toolChangeSignal(int);

public slots:
    void onSendProgram(const GcodeFileModel& gcodeFile);
    void onSendProgramFromLine(const GcodeFileModel& gcodeFile, long fromId);
    void onGcodeCommandManualSend(GcodeCommand*);

    void onUpdateSpindleOverride(float speed);
    void onUpdateFeedOverride(float rate);
    void onUpdateRapidOverride(float rate);
    void onToolChangeCompleted();

private slots:
    void onConnect();
    void onSerialPortReadyRead();
    void onSerialPortError(QSerialPort::SerialPortError);
    void onSerialPortNameChanged(QString);
    void onSerialPortBaudRateChanged(int);
    void onProgramSendTimerTimeout();
    void onStatusTimerTimeout();
    void onSerialBytesWritten(qint64 bytes);

private: // Members
    const static int BUFFER_LENGTH_LIMIT;
    QSerialPort mSerialPort;
    QList<GcodeCommand*> mCommandBuffer;
    QList<GcodeCommand*> mCommandQueue;
    GrblMachineState mState;
    GrblMachineState mLastState;
    QVector3D mMachinePosition;
    QVector3D mWorkPosition;
    QVector3D mWorkCoordinateOffset;
    SqlSettingsModel* mSettingsModelHandle;
    QTimer mProgramSendTimer;
    QTimer mStatusTimer;
    int mProgramSendInterval;
    int mStatusInterval;
    int mCountProcessedCommands;
    int mCommandQueueInitialSize;
    float mFeedOverride;
    float mSpindleOverride;
    float mRapidOverride;
    bool mError;
    int mErrorCode;
    QString mErrorString;
    QString mGrblVersion;
    qint64 mBytesWaiting;
    bool mStatusRequested;
    bool mWaitingForStatus;
    QMap<int,QString> mFirmwareConfiguration;
    bool mProgramRunning;
    bool mToolChangeWaiting;

private: // Member Functions
    GcodeCommand feedOverride(GcodeCommand* command, double overridePercent);
    GcodeCommand getNextCommand(GcodeFileModel& gcodeFile);
    void setupSerialPort();

    bool isSpaceInBuffer(GcodeCommand* cmd);
    int getProcessedPercent();

    void startProgramSendTimer();
    void stopProgramSendTimer();

    void startStatusTimer();
    void stopStatusTimer();

    void updateWorkPosition();
    void updateStatus(GrblResponse response);
    void updateOverrides(const GrblResponse& data);
    void updateWorkCoordinateOffset(const GrblResponse& data);
    void updateMachinePosition(const GrblResponse& data);

    void processResponse(const GrblResponse& data);
    void parseError(const GrblResponse& error);
    void parseGrblVersion(const GrblResponse& response);
    void parseConfigurationResponse(GrblResponse response);
    void parseAlarmResponse(const GrblResponse& response);

    const static map<int,QString> ERROR_STRINGS;
    const static map<int,QString> ALARM_STRINGS;
};
