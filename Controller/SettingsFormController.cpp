// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "SettingsFormController.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QScrollBar>
#include <QColorDialog>
#include "Model/SerialBaudRate.h"
#include "ui_SettingsForm.h"

SettingsFormController::SettingsFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "SettingsFormController: Constructing";
    mUi.setupUi(&mDialog);
    setupSignalSlots();
    searchPorts();
    mDialog.setLocale(QLocale::C);

    if (mSerialPortName != "")
    {
        emit settingChangedSignal(Settings::SERIAL, Settings::SERIAL_PORT_NAME, getPortName());
        emit settingChangedSignal(Settings::SERIAL, Settings::SERIAL_BAUD_RATE, getBaudRate());
    }
//    setTabIcons();
}

SettingsFormController::~SettingsFormController()
{
    qDebug() << "SettingsFormController: Destructing";
}

void SettingsFormController::setTabIcons()
{
   mUi.settingsTabs->setTabIcon(0,QIcon(":/Images/SVG/plug.svg"));
   mUi.settingsTabs->setTabIcon(1,QIcon(":/Images/SVG/desktop.svg"));
   mUi.settingsTabs->setTabIcon(2,QIcon(":/Images/SVG/tachometer-alt.svg"));
   mUi.settingsTabs->setTabIcon(3,QIcon(":/Images/SVG/wrench.svg"));
}

int SettingsFormController::exec()
{
    return mDialog.exec();
}

void SettingsFormController::initialise()
{

}

QString SettingsFormController::getPortName()
{
    return mUi.serialPortNameBox->currentText();
}

void SettingsFormController::setPortName(QString port)
{
    mUi.serialPortNameBox->setCurrentText(port);
}

int SettingsFormController::getBaudRate()
{
    return mUi.serialBaudRateBox->currentText().toInt();
}

void SettingsFormController::setBaudRate(int baud)
{
    mUi.serialBaudRateBox->setCurrentText(QString::number(baud));
}

bool SettingsFormController::antialiasing()
{
    return mUi.chkAntialiasing->isChecked();
}

void SettingsFormController::setAntialiasing(bool antialiasing)
{
    mUi.chkAntialiasing->setChecked(antialiasing);
}

bool SettingsFormController::zBuffer()
{
    return mUi.chkZBuffer->isChecked();
}

void SettingsFormController::setZBuffer(bool zBuffer)
{
    mUi.chkZBuffer->setChecked(zBuffer);
}

double SettingsFormController::lineWidth()
{
    return mUi.txtLineWidth->value();
}

void SettingsFormController::setLineWidth(double lineWidth)
{
    mUi.txtLineWidth->setValue(lineWidth);
}

double SettingsFormController::arcLength()
{
    return mUi.txtArcLength->value();
}

void SettingsFormController::setArcLength(double arcPrecision)
{
    mUi.txtArcLength->setValue(arcPrecision);
}

double SettingsFormController::arcDegree()
{
    return mUi.txtArcDegree->value();
}

void SettingsFormController::setArcDegree(double arcDegree)
{
    mUi.txtArcDegree->setValue(arcDegree);
}

double SettingsFormController::arcPrecision()
{
    return mUi.radArcDegreeMode->isChecked() ? mUi.txtArcDegree->value() : mUi.txtArcLength->value();
}

bool SettingsFormController::arcDegreeMode()
{
    return mUi.radArcDegreeMode->isChecked();
}

void SettingsFormController::setArcDegreeMode(bool arcDegreeMode)
{
    mUi.radArcDegreeMode->setChecked(arcDegreeMode);
}

bool SettingsFormController::showProgramCommands()
{
    return mUi.chkShowProgramCommands->isChecked();
}

void SettingsFormController::setShowProgramCommands(bool showAllCommands)
{
    mUi.chkShowProgramCommands->setChecked(showAllCommands);
}

bool SettingsFormController::showUICommands()
{
    return mUi.chkShowUICommands->isChecked();
}

void SettingsFormController::setShowUICommands(bool showUICommands)
{
    mUi.chkShowUICommands->setChecked(showUICommands);
}

QString SettingsFormController::safePositionCommand()
{
    return mUi.txtSafeCommand->text();
}

void SettingsFormController::setSafePositionCommand(QString command)
{
    mUi.txtSafeCommand->setText(command);
}

bool SettingsFormController::moveOnRestore()
{
    return mUi.chkMoveOnRestore->isChecked();
}

void SettingsFormController::setMoveOnRestore(bool value)
{
    mUi.chkMoveOnRestore->setChecked(value);
}

int SettingsFormController::restoreMode()
{
    return mUi.cboRestoreMode->currentIndex();
}

void SettingsFormController::setRestoreMode(int value)
{
    mUi.cboRestoreMode->setCurrentIndex(value);
}

int SettingsFormController::spindleSpeedMin()
{
    return mUi.txtSpindleSpeedMin->value();
}

void SettingsFormController::setSpindleSpeedMin(int speed)
{
    mUi.txtSpindleSpeedMin->setValue(speed);
}

int SettingsFormController::spindleSpeedMax()
{
    return mUi.txtSpindleSpeedMax->value();
}

void SettingsFormController::setSpindleSpeedMax(int speed)
{
    mUi.txtSpindleSpeedMax->setValue(speed);
}

int SettingsFormController::rapidSpeed()
{
    return mUi.txtRapidSpeed->value();
}

void SettingsFormController::setRapidSpeed(int rapidSpeed)
{
    mUi.txtRapidSpeed->setValue(rapidSpeed);
}

QString SettingsFormController::heightmapProbingFeed()
{
    return mUi.txtHeightMapProbingFeed->text();
}

void SettingsFormController::setHeightmapProbingFeed(QString heightmapProbingFeed)
{
    mUi.txtHeightMapProbingFeed->setText(heightmapProbingFeed);
}

int SettingsFormController::acceleration()
{
    return mUi.txtAcceleration->value();
}

void SettingsFormController::setAcceleration(int acceleration)
{
    mUi.txtAcceleration->setValue(acceleration);
}

int SettingsFormController::queryStateTime()
{
    return mUi.txtQueryStateTime->value();
}

void SettingsFormController::setQueryStateTime(int queryStateTime)
{
    mUi.txtQueryStateTime->setValue(queryStateTime);
}

int SettingsFormController::fps()
{
    return mUi.cboFps->currentText().toInt();
}

void SettingsFormController::setFps(int fps)
{
    mUi.cboFps->setCurrentText(QString::number(fps));
}

bool SettingsFormController::vsync()
{
    return mUi.chkVSync->isChecked();
}

void SettingsFormController::setVsync(bool value)
{
    mUi.chkVSync->setChecked(value);
}

bool SettingsFormController::msaa()
{
    return mUi.radMSAA->isChecked();
}

void SettingsFormController::setMsaa(bool msaa)
{
    mUi.radMSAA->setChecked(msaa);
}

bool SettingsFormController::autoCompletion()
{
    return mUi.chkAutocompletion->isChecked();
}

void SettingsFormController::setAutoCompletion(bool autoCompletion)
{
    mUi.chkAutocompletion->setChecked(autoCompletion);
}

int SettingsFormController::units()
{
    return mUi.cboUnits->currentIndex();
}

void SettingsFormController::setUnits(int units)
{
    mUi.cboUnits->setCurrentIndex(units);
}

QString SettingsFormController::touchCommand()
{
    return mUi.txtTouchCommand->text();
}

void SettingsFormController::setTouchCommand(QString touchCommand)
{
    mUi.txtTouchCommand->setText(touchCommand);
}

bool SettingsFormController::simplify()
{
    return mUi.chkSimplify->isChecked();
}

void SettingsFormController::setSimplify(bool simplify)
{
    mUi.chkSimplify->setChecked(simplify);
}

double SettingsFormController::simplifyPrecision()
{
    return mUi.txtSimplifyPrecision->value();
}

void SettingsFormController::setSimplifyPrecision(double simplifyPrecision)
{
    mUi.txtSimplifyPrecision->setValue(simplifyPrecision);
}

bool SettingsFormController::grayscaleSegments()
{
    return mUi.chkGrayscale->isChecked();
}

void SettingsFormController::setGrayscaleSegments(bool value)
{
    mUi.chkGrayscale->setChecked(value);
}

bool SettingsFormController::grayscaleSCode()
{
    return mUi.radGrayscaleS->isChecked();
}

void SettingsFormController::setGrayscaleSCode(bool value)
{
    mUi.radGrayscaleS->setChecked(value);
    mUi.radGrayscaleZ->setChecked(!value);
}

bool SettingsFormController::drawModeVectors()
{
    return mUi.radDrawModeVectors->isChecked();
}

void SettingsFormController::setDrawModeVectors(bool value)
{
    mUi.radDrawModeVectors->setChecked(value);
    mUi.radDrawModeRaster->setChecked(!value);
}

QString SettingsFormController::userCommands(int index)
{
    return this->findChild<QLineEdit*>(QString("txtUserCommand%1").arg(index))->text();
}

void SettingsFormController::setUserCommands(int index, QString commands)
{
    this->findChild<QLineEdit*>(QString("txtUserCommand%1").arg(index))->setText(commands);
}

bool SettingsFormController::ignoreErrors()
{
    return mUi.chkIgnoreErrors->isChecked();
}

void SettingsFormController::setIgnoreErrors(bool value)
{
    mUi.chkIgnoreErrors->setChecked(value);
}

bool SettingsFormController::autoLine()
{
    return mUi.chkAutoLine->isChecked();
}

void SettingsFormController::setAutoLine(bool value)
{
    mUi.chkAutoLine->setChecked(value);
}

void SettingsFormController::setFormActive(bool active)
{

}

void SettingsFormController::onSettingChanged(QString group, QString param, QVariant value)
{
    /*
    qDebug() << "SettingsFormController: onSettingChanged"
             << group
             << param
             << value;
    */

   if (group == Settings::GFX)
   {

   }
   else if (group == Settings::GLOBAL)
   {
        if (param == Settings::GLOBAL_AUTO_LINE)
        {
           setAutoLine(value.toBool());
        }
        else if (param == Settings::GLOBAL_IGNORE_ERRORS)
        {
            setIgnoreErrors(value.toBool());
        }
   }
   else if (group == Settings::HEIGHT_MAP)
   {

   }
   else if (group == Settings::SERIAL)
   {
      if (param == Settings::SERIAL_BAUD_RATE)
      {
          setBaudRate(value.toInt());
      }
      else if (param == Settings::SERIAL_PORT_NAME)
      {
          setPortName(value.toString());
      }
   }
   else if (group == Settings::TOOL)
   {

   }
   else if (group == Settings::VISUALISER)
   {

   }
}

void SettingsFormController::searchPorts()
{
    mUi.serialPortNameBox->clear();

    foreach (QSerialPortInfo info, QSerialPortInfo::availablePorts())
    {
        mUi.serialPortNameBox->addItem(info.portName());
    }

    mUi.serialBaudRateBox->clear();

    for (int baud : SERIAL_BAUD_RATES)
    {
        mUi.serialBaudRateBox->addItem(QString::number(baud),baud);
    }
}

void SettingsFormController::setupSignalSlots()
{
    qDebug() << "SettingsFormController: Setup Signals/Slots";

    // Main ----------------------------------------------------------------------
    connect(mUi.closeButton, SIGNAL(clicked()),SLOT(onCloseButtonClicked()));
    connect(mUi.restoreDefaultsButton, SIGNAL(clicked()),SLOT(onRestoreDefaultsButtonClicked()));

    // Connection ----------------------------------------------------------------
    // Serial
    connect(mUi.serialPortRefreshButton, SIGNAL(clicked()), SLOT(onSerialPortRefreshClicked()));
    connect(mUi.serialPortNameBox, SIGNAL(currentIndexChanged(QString)), SLOT(onSerialPortNameChanged(QString)));
    connect(mUi.serialBaudRateBox, SIGNAL(currentIndexChanged(QString)), SLOT(onSerialBaudRateChanged(QString)));
    // Sender
    connect(mUi.chkIgnoreErrors, SIGNAL(toggled(bool)), this, SLOT(onIgnoreErrorsToggled(bool)));
    connect(mUi.chkAutoLine, SIGNAL(toggled(bool)), this, SLOT(onAutoLineToggled(bool)));
    // Parser
    connect(mUi.radArcLengthMode, SIGNAL(toggled(bool)), this, SLOT(onArcLengthModeToggled(bool)));
    connect(mUi.txtArcLength, SIGNAL(valueChanged(QString)), this, SLOT(onArcLengthValueChanged(QString)));
    connect(mUi.radArcDegreeMode, SIGNAL(toggled(bool)), this, SLOT(onArcDegreeModeToggled(bool)));
    connect(mUi.txtArcDegree,SIGNAL(valueChanged(QString)),this,SLOT(onArcDegreeValueChanged(QString)));

    // Interface -----------------------------------------------------------------
    // Visualiser
    connect(mUi.txtLineWidth, SIGNAL(valueChanged(QString)), this, SLOT(onLineWidthValueChanged(QString)));
    connect(mUi.cboFps, SIGNAL(currentIndexChanged(QString)), this, SLOT(onFpsCurrentIndexChanged(QString)));
    connect(mUi.chkAntialiasing, SIGNAL(toggled(bool)), this, SLOT(onAntiAliasingToggled(bool)));
    connect(mUi.radMSAA, SIGNAL(toggled(bool)), this, SLOT(onMsaaToggled(bool)));
    connect(mUi.chkVSync, SIGNAL(toggled(bool)), this, SLOT(onVsyncToggled(bool)));
    connect(mUi.chkZBuffer, SIGNAL(toggled(bool)), this, SLOT(onZBufferToggled(bool)));
    connect(mUi.radDrawModeVectors, SIGNAL(toggled(bool)), this, SLOT(onDrawModeVectorsToggled(bool)));
    connect(mUi.radDrawModeRaster,  SIGNAL(toggled(bool)), this, SLOT(onDrawModeRasterToggled(bool)));
    connect(mUi.chkSimplify, SIGNAL(toggled(bool)), this, SLOT(onSimplifyToggled(bool)));
    connect(mUi.txtSimplifyPrecision, SIGNAL(valueChanged(QString)), this, SLOT(onSimplifyPrecisionValueChanged(QString)));
    connect(mUi.chkGrayscale, SIGNAL(toggled(bool)), this, SLOT(onGrayscaleToggled(bool)));
    connect(mUi.radGrayscaleS, SIGNAL(toggled(bool)), this, SLOT(onGrayscaleSToggled(bool)));
    connect(mUi.radGrayscaleZ, SIGNAL(toggled(bool)), this, SLOT(onGrayscaleZToggled(bool)));
    // Console
    connect(mUi.chkShowProgramCommands, SIGNAL(toggled(bool)), this, SLOT(onShowProgramCommandsToggled(bool)));
    connect(mUi.chkShowUICommands, SIGNAL(toggled(bool)), this, SLOT(onShowUiCommandsToggled(bool)));
    connect(mUi.chkAutocompletion, SIGNAL(toggled(bool)), this, SLOT(onAutoCompletionToggled(bool)));
    // Colors
    // Machine -------------------------------------------------------------------
    // Machine Information
    connect(mUi.txtQueryStateTime, SIGNAL(valueChanged(QString)), this, SLOT(onQueryStateTimeValueChanged(QString)));
    connect(mUi.cboUnits, SIGNAL(currentIndexChanged(QString)), this, SLOT(onUnitsCurrentIndexChanged(QString)));
    connect(mUi.txtRapidSpeed, SIGNAL(valueChanged(QString)), this, SLOT(onRapidSpeedValueChanged(QString)));
    connect(mUi.txtAcceleration, SIGNAL(valueChanged(QString)), this, SLOT(onAccelerationValueChanged(QString)));
    connect(mUi.txtSpindleSpeedMin, SIGNAL(valueChanged(QString)), this, SLOT(onSpindleSpeedMinValueChanged(QString)));
    connect(mUi.txtSpindleSpeedMax, SIGNAL(valueChanged(QString)), this, SLOT(onSpindleSpeedMaxValueChanged(QString)));
    // Control
    connect(mUi.txtTouchCommand, SIGNAL(textChanged(QString)), this, SLOT(onTouchCommandValueChanged(QString)));
    connect(mUi.txtSafeCommand, SIGNAL(textChanged(QString)), this, SLOT(onSafeCommandValueChanged(QString)));
    connect(mUi.chkMoveOnRestore, SIGNAL(toggled(bool)), this, SLOT(onMoveOnRestoreToggled(bool)));
    connect(mUi.cboRestoreMode, SIGNAL(currentIndexChanged(QString)), this, SLOT(onRestoreModeCurrentIndexChanged(QString)));
    // User Commands
    connect(mUi.txtUserCommand0, SIGNAL(textChanged(QString)), this, SLOT(onUserCommand1ValueChanged(QString)));
    connect(mUi.txtUserCommand1, SIGNAL(textChanged(QString)), this, SLOT(onUserCommand2ValueChanged(QString)));
    connect(mUi.txtUserCommand2, SIGNAL(textChanged(QString)), this, SLOT(onUserCommand3ValueChanged(QString)));
    connect(mUi.txtUserCommand3, SIGNAL(textChanged(QString)), this, SLOT(onUserCommand4ValueChanged(QString)));
    // Height Mep
    connect(mUi.txtHeightMapProbingFeed, SIGNAL(textChanged(QString)), this, SLOT(onHeightMapProbingFeedValueChanged(QString)));
}

void SettingsFormController::onSerialPortRefreshClicked()
{
    searchPorts();
}

void SettingsFormController::onSerialPortNameChanged(QString port)
{
   emit settingChangedSignal(Settings::SERIAL, Settings::SERIAL_PORT_NAME, port);
}

void SettingsFormController::onSerialBaudRateChanged(QString baud)
{
    emit settingChangedSignal(Settings::SERIAL, Settings::SERIAL_BAUD_RATE, baud.toInt());
}

void SettingsFormController::onIgnoreErrorsToggled(bool)
{

}

void SettingsFormController::onAutoLineToggled(bool)
{

}

void SettingsFormController::onArcLengthModeToggled(bool)
{

}

void SettingsFormController::onArcLengthValueChanged(QString)
{

}

void SettingsFormController::onArcDegreeModeToggled(bool)
{

}

void SettingsFormController::onArcDegreeValueChanged(QString)
{

}

void SettingsFormController::onLineWidthValueChanged(QString)
{

}

void SettingsFormController::onFpsCurrentIndexChanged(QString)
{

}

void SettingsFormController::onAntiAliasingToggled(bool)
{

}

void SettingsFormController::onMsaaToggled(bool)
{

}

void SettingsFormController::onVsyncToggled(bool)
{

}

void SettingsFormController::onZBufferToggled(bool)
{

}

void SettingsFormController::onCloseButtonClicked()
{
    mDialog.accept();
}

void SettingsFormController::onRestoreDefaultsButtonClicked()
{
    QMessageBox::StandardButton result = QMessageBox::warning
    (
        &mDialog,
        qApp->applicationDisplayName(),
        tr("Restore Default Settings?"),
        QMessageBox::Cancel | QMessageBox::Ok
    );

    if (result != QMessageBox::Ok)
    {
        return;
    }

    setPortName("");
    setBaudRate(SerialBaudRate::BAUD_115200);
    setIgnoreErrors(false);
    setQueryStateTime(40);
    setRapidSpeed(2000);
    setAcceleration(100);
    setSpindleSpeedMin(0);
    setSpindleSpeedMax(10000);
    setTouchCommand("G21G91G38.2Z-30F100; G0Z1; G38.2Z-2F10");
    setSafePositionCommand("G21G90; G53G0Z0");
    setMoveOnRestore(false);
    setRestoreMode(0);
    setHeightmapProbingFeed("10");
    setUnits(0);
    setArcLength(0.0);
    setArcDegreeMode(true);
    setArcDegree(5.0);
    setLineWidth(1.5);
    setAntialiasing(true);
    setMsaa(true);
    setSimplify(true);
    setSimplifyPrecision(0.0);
    setFps(60);
    setZBuffer(false);
    setGrayscaleSegments(false);
    setGrayscaleSCode(true);
    setDrawModeVectors(true);
    setShowProgramCommands(false);
    setAutoCompletion(true);
}

void SettingsFormController::onDrawModeVectorsToggled(bool checked)
{
    mUi.chkSimplify->setEnabled(checked);
    mUi.lblSimpilyPrecision->setEnabled(checked && mUi.chkSimplify->isChecked());
    mUi.txtSimplifyPrecision->setEnabled(checked && mUi.chkSimplify->isChecked());
    mUi.radDrawModeRaster->setChecked(!checked);
}

void SettingsFormController::onDrawModeRasterToggled(bool checked)
{
    mUi.radDrawModeVectors->setChecked(!checked);
}

void SettingsFormController::onSimplifyToggled(bool)
{

}

void SettingsFormController::onSimplifyPrecisionValueChanged(QString)
{

}

void SettingsFormController::onGrayscaleToggled(bool)
{

}

void SettingsFormController::onGrayscaleSToggled(bool checked)
{
    mUi.radGrayscaleZ->setChecked(!checked);
}

void SettingsFormController::onGrayscaleZToggled(bool checked)
{
    mUi.radGrayscaleS->setChecked(!checked);
}

void SettingsFormController::onShowProgramCommandsToggled(bool)
{

}

void SettingsFormController::onShowUiCommandsToggled(bool)
{

}

void SettingsFormController::onAutoCompletionToggled(bool)
{

}

void SettingsFormController::onQueryStateTimeValueChanged(QString)
{

}

void SettingsFormController::onUnitsCurrentIndexChanged(QString)
{

}

void SettingsFormController::onRapidSpeedValueChanged(QString)
{

}

void SettingsFormController::onAccelerationValueChanged(QString)
{

}

void SettingsFormController::onSpindleSpeedMinValueChanged(QString)
{

}

void SettingsFormController::onSpindleSpeedMaxValueChanged(QString)
{

}

void SettingsFormController::onTouchCommandValueChanged(QString)
{

}

void SettingsFormController::onSafeCommandValueChanged(QString)
{

}

void SettingsFormController::onMoveOnRestoreToggled(bool)
{

}

void SettingsFormController::onRestoreModeCurrentIndexChanged(QString)
{

}

void SettingsFormController::onUserCommand1ValueChanged(QString)
{

}

void SettingsFormController::onUserCommand2ValueChanged(QString)
{

}

void SettingsFormController::onUserCommand3ValueChanged(QString)
{

}

void SettingsFormController::onUserCommand4ValueChanged(QString)
{

}

void SettingsFormController::onHeightMapProbingFeedValueChanged(QString)
{

}

