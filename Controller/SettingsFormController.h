// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include "AbstractFormController.h"
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
    void undo();

    QString getPortName();
    void setPortName(QString port);

    int getBaudRate();
    void setBaudRate(int baud);

    double toolDiameter();
    void setToolDiameter(double diameter);

    double toolLength();
    void setToolLength(double length);

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

    int laserPowerMin();
    void setLaserPowerMin(int value);

    int laserPowerMax();
    void setLaserPowerMax(int value);

    int rapidSpeed();
    void setRapidSpeed(int rapidSpeed);

    int heightmapProbingFeed();
    void setHeightmapProbingFeed(int heightmapProbingFeed);

    int acceleration();
    void setAcceleration(int acceleration);

    int queryStateTime();
    void setQueryStateTime(int queryStateTime);

    int toolType();
    void setToolType(int toolType);

    double toolAngle();
    void setToolAngle(double toolAngle);

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

    //QList<ColorPicker*> colors();
    //QColor colors(QString name);

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

    void applySettings();
signals:
     void serialPortNameChangedSignal(QString);
     void serialPortBaudRateChangedSignal(int);

protected:
    void showEvent(QShowEvent *se) override;
private slots:
    void onCmdRefreshClicked();
    void onCmdOKClicked();
    void onCmdCancelClicked();
    void onCmdDefaultsClicked();
    void onComboBoxToolTypeCurrentIndexChanged(int index);
    void onRadioBtnDrawModeVectorsToggled(bool checked);
    void onRadioBtnDrawModeRasterToggled(bool checked);
    void onRadioBtnGrayscaleSToggled(bool checked);
    void onRadioBtnGrayscaleZToggled(bool checked);
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


    // AbstractFormController interface
public:
    void setupSignalSlots() override;
};

