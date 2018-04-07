// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include "AbstractFormController.h"
#include "Model/Settings/Settings.h"
#include "ui_SettingsForm.h"

using namespace Ui;

class SettingsFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit SettingsFormController(QWidget *parent = nullptr);
    ~SettingsFormController() override;

    void saveSettings();
    int exec();

    void initialise() override;

    QString getPortName();
    void setPortName(QString port);

    int getBaudRate();
    void setBaudRate(int baud);

    bool antialiasing();
    void setAntialiasing(bool antialiasing);

    bool zBuffer();
    void setZBuffer(bool zBuffer);

    double lineWidth();
    void setLineWidth(double lineWidth);

    double arcLength();
    void setArcLength(double arcLength);

    double arcDegree();
    void setArcDegree(double arcDegree);

    double arcPrecision();
    bool arcDegreeMode();

    void setArcDegreeMode(bool arcDegreeMode);

    bool showProgramCommands();
    void setShowProgramCommands(bool showProgramCommands);

    bool showUICommands();
    void setShowUICommands(bool showUICommands);

    QString safePositionCommand();
    void setSafePositionCommand(QString command);

    bool moveOnRestore();
    void setMoveOnRestore(bool value);

    int restoreMode();
    void setRestoreMode(int value);

    int spindleSpeedMin();
    void setSpindleSpeedMin(int speed);

    int spindleSpeedMax();
    void setSpindleSpeedMax(int speed);

    int rapidSpeed();
    void setRapidSpeed(int rapidSpeed);

    QString heightmapProbingFeed();
    void setHeightmapProbingFeed(QString heightmapProbingFeed);

    int acceleration();
    void setAcceleration(int acceleration);

    int queryStateTime();
    void setQueryStateTime(int queryStateTime);

    int fps();
    void setFps(int fps);

    bool vsync();
    void setVsync(bool value);

    bool msaa();
    void setMsaa(bool msaa);

    bool autoCompletion();
    void setAutoCompletion(bool autoCompletion);

    int units();
    void setUnits(int units);

    QString touchCommand();
    void setTouchCommand(QString touchCommand);

    bool simplify();
    void setSimplify(bool simplify);

    double simplifyPrecision();
    void setSimplifyPrecision(double simplifyPrecision);

    bool grayscaleSegments();
    void setGrayscaleSegments(bool value);

    bool grayscaleSCode();
    void setGrayscaleSCode(bool value);

    bool drawModeVectors();
    void setDrawModeVectors(bool value);

    QString userCommands(int index);
    void setUserCommands(int index, QString commands);

    bool ignoreErrors();
    void setIgnoreErrors(bool value);

    bool autoLine();
    void setAutoLine(bool value);
    void setFormActive(bool active) override;

public slots:
    void onSettingChanged(QString group, QString param, QVariant value);

signals:
    void settingChangedSignal(QString group, QString param, QVariant value);

private slots:
    void onCloseButtonClicked();
    void onRestoreDefaultsButtonClicked();
    void onSerialPortRefreshClicked();
    void onSerialPortNameChanged(QString);
    void onSerialBaudRateChanged(QString);
    void onIgnoreErrorsToggled(bool);
    void onAutoLineToggled(bool);
    void onArcLengthModeToggled(bool);
    void onArcLengthValueChanged(QString);
    void onArcDegreeModeToggled(bool);
    void onArcDegreeValueChanged(QString);
    void onLineWidthValueChanged(QString);
    void onFpsCurrentIndexChanged(QString);
    void onAntiAliasingToggled(bool);
    void onMsaaToggled(bool);
    void onVsyncToggled(bool);
    void onZBufferToggled(bool);
    void onDrawModeVectorsToggled(bool checked);
    void onDrawModeRasterToggled(bool checked);
    void onSimplifyToggled(bool);
    void onSimplifyPrecisionValueChanged(QString);
    void onGrayscaleToggled(bool);
    void onGrayscaleSToggled(bool checked);
    void onGrayscaleZToggled(bool checked);
    void onShowProgramCommandsToggled(bool);
    void onShowUiCommandsToggled(bool);
    void onAutoCompletionToggled(bool);
    void onQueryStateTimeValueChanged(QString);
    void onUnitsCurrentIndexChanged(QString);
    void onRapidSpeedValueChanged(QString);
    void onAccelerationValueChanged(QString);
    void onSpindleSpeedMinValueChanged(QString);
    void onSpindleSpeedMaxValueChanged(QString);
    void onTouchCommandValueChanged(QString);
    void onSafeCommandValueChanged(QString);
    void onMoveOnRestoreToggled(bool);
    void onRestoreModeCurrentIndexChanged(QString);
    void onUserCommand1ValueChanged(QString);
    void onUserCommand2ValueChanged(QString);
    void onUserCommand3ValueChanged(QString);
    void onUserCommand4ValueChanged(QString);
    void onHeightMapProbingFeedValueChanged(QString);
private:
    QDialog mDialog;
    SettingsForm mUi;
    void searchPorts();
    QList<double> mStoredValues;
    QList<bool> mStoredChecks;
    QList<QString> mStoredCombos;
    QList<QColor> mStoredColors;
    QIntValidator mIntValidator;
    QString mSettingsFileName;

    QString mSerialPortName;
    QString mSerialPortBaudRate;

    void setTabIcons();
public:
    void setupSignalSlots() override;
};

