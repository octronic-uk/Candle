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
    void updateFeedRateSignal(float);
    void updateSpindleSpeedSignal(float);
    void updateMachinePositionSignal(const QVector3D);
    void updateWorkPositionSignal(const QVector3D);

    void serialPortErrorSignal(QString errorMessage);

    void statusBarUpdateSignal(QString status);
    void statusTextUpdateSignal(QString status, QColor = QColor("Black"), QColor = QColor("White"));

    void toolPositionChangedSignal(QVector3D);
    void machineStateUpdatedSignal(const GrblMachineState&);
    void jobCompletedSignal();

    void appendResponseToConsoleSignal(const GrblResponse&);
    void appendCommandToConsoleSignal(GcodeCommand*);
    void commandResponseSignal(const GrblResponse&);
    void setCompletionProgressSignal(int);
    void setBufferProgressSignal(int);
    void errorSignal(QString);

public slots:
    void onSendProgram(const GcodeFileModel& gcodeFile);
    void onSendProgramFromLine(const GcodeFileModel& gcodeFile, long fromId);
    void onSettingChanged(QString group, QString param, QVariant value);
    void onGcodeCommandManualSend(GcodeCommand*);
    void onUpdateSpindleSpeed(float speed);
    void onUpdateFeedRate(float rate);

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
    int mCommandQueueInitialSize;
    int mUpdateRate;
    float mCurrentFeedRate;
    float mCurrentSpindleSpeed;
    bool mError;
    int mErrorCode;
    QString mErrorString;
    QString mGrblVersion;

private: // Member Functions
    GcodeCommand feedOverride(GcodeCommand* command, double overridePercent);
    GcodeCommand getNextCommand(GcodeFileModel& gcodeFile);
    void processResponse(const GrblResponse& data);
    void updateMachineCoordinates(const GrblResponse& data);
    void updateWorkCoordinates(const GrblResponse& data);
    void updateToolCoordinates();
    void setupSerialPort();
    bool isSpaceInBuffer(GcodeCommand* cmd);
    void startProgramSendTimer();
    void stopProgramSendTimer();
    double getProcessedPercent();
    void parseError(const GrblResponse& error);
    const static std::map<int,QString> ERROR_STRINGS;
    void parseGrblVersion(const GrblResponse& response);
};
