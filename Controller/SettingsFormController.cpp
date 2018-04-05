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
}

SettingsFormController::~SettingsFormController()
{
    qDebug() << "SettingsFormController: Destructing";
}

int SettingsFormController::exec()
{
    return mDialog.exec();
}

void SettingsFormController::undo()
{
    foreach (QAbstractSpinBox* sb, this->findChildren<QAbstractSpinBox*>())
    {
        sb->setProperty("value", mStoredValues.takeFirst());
    }

    foreach (QAbstractButton* cb, this->findChildren<QAbstractButton*>())
    {
        cb->setChecked(mStoredChecks.takeFirst());
    }
    foreach (QComboBox* cb, this->findChildren<QComboBox*>())
    {
        cb->setCurrentText(mStoredCombos.takeFirst());
    }
    /*foreach (ColorPicker* pick, this->findChildren<ColorPicker*>())
    {
        pick->setColor(mStoredColors.takeFirst());
    }
    */
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

double SettingsFormController::toolDiameter()
{
    return mUi.txtToolDiameter->value();
}

void SettingsFormController::setToolDiameter(double diameter)
{
    mUi.txtToolDiameter->setValue(diameter);
}

double SettingsFormController::toolLength()
{
    return mUi.txtToolLength->value();
}

void SettingsFormController::setToolLength(double length)
{
    mUi.txtToolLength->setValue(length);
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

int SettingsFormController::laserPowerMin()
{
    return mUi.txtLaserPowerMin->value();
}

void SettingsFormController::setLaserPowerMin(int value)
{
    mUi.txtLaserPowerMin->setValue(value);
}

int SettingsFormController::laserPowerMax()
{
    return mUi.txtLaserPowerMax->value();
}

void SettingsFormController::setLaserPowerMax(int value)
{
    mUi.txtLaserPowerMax->setValue(value);
}

int SettingsFormController::rapidSpeed()
{
    return mUi.txtRapidSpeed->value();
}

void SettingsFormController::setRapidSpeed(int rapidSpeed)
{
    mUi.txtRapidSpeed->setValue(rapidSpeed);
}

int SettingsFormController::heightmapProbingFeed()
{
    return mUi.txtHeightMapProbingFeed->value();
}

void SettingsFormController::setHeightmapProbingFeed(int heightmapProbingFeed)
{
    mUi.txtHeightMapProbingFeed->setValue(heightmapProbingFeed);
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

int SettingsFormController::toolType()
{
    return mUi.cboToolType->currentIndex();
}

void SettingsFormController::setToolType(int toolType)
{
    mUi.cboToolType->setCurrentIndex(toolType);
}

double SettingsFormController::toolAngle()
{
    return mUi.txtToolAngle->value();
}

void SettingsFormController::setToolAngle(double toolAngle)
{
    mUi.txtToolAngle->setValue(toolAngle);
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

/*QList<ColorPicker *> SettingsFormController::colors()
{
    return this->findChildren<ColorPicker*>();
}
*/

/*
QColor SettingsFormController::colors(QString name)
{
    ColorPicker *pick = this->findChildren<ColorPicker*>("clp" + name).at(0);
    if (pick) return pick->color(); else return QColor();
}
*/

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

void SettingsFormController::showEvent(QShowEvent *se)
{
    Q_UNUSED(se)
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
    // Ok
    connect(mUi.closeButton, SIGNAL(clicked()),SLOT(onCloseButtonClicked()));
    // Defaults
    connect(mUi.restoreDefaultsButton, SIGNAL(clicked()),SLOT(onRestoreDefaultsButtonClicked()));
    // Serial Port
    // Refresh
    connect(mUi.serialPortRefreshButton, SIGNAL(clicked()), SLOT(onSerialPortRefreshClicked()));
    // Port Name
    connect(mUi.serialPortNameBox, SIGNAL(currentIndexChanged(QString)), SLOT(onSerialPortNameChanged(QString)));
    // Baud Rate
    connect(mUi.serialBaudRateBox, SIGNAL(currentIndexChanged(QString)), SLOT(onSerialBaudRateChanged(QString)));
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

void SettingsFormController::onCloseButtonClicked()
{
    mDialog.accept();
}

void SettingsFormController::onComboBoxToolTypeCurrentIndexChanged(int index)
{
    mUi.lblToolAngle->setEnabled(index == 1);
    mUi.txtToolAngle->setEnabled(index == 1);
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
    setLaserPowerMin(0);
    setLaserPowerMax(100);
    setTouchCommand("G21G91G38.2Z-30F100; G0Z1; G38.2Z-2F10");
    setSafePositionCommand("G21G90; G53G0Z0");
    setMoveOnRestore(false);
    setRestoreMode(0);
    setHeightmapProbingFeed(10);
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
    setToolType(1);
    setToolAngle(15.0);
    setToolDiameter(3.0);
    setToolLength(30.0);
    setShowProgramCommands(false);
    setAutoCompletion(true);
}

void SettingsFormController::onRadioBtnDrawModeVectorsToggled(bool checked)
{
    mUi.chkSimplify->setEnabled(checked);
    mUi.lblSimpilyPrecision->setEnabled(checked && mUi.chkSimplify->isChecked());
    mUi.txtSimplifyPrecision->setEnabled(checked && mUi.chkSimplify->isChecked());

    mUi.radDrawModeRaster->setChecked(!checked);
}

void SettingsFormController::onRadioBtnDrawModeRasterToggled(bool checked)
{
    mUi.radDrawModeVectors->setChecked(!checked);
}

void SettingsFormController::onRadioBtnGrayscaleSToggled(bool checked)
{
    mUi.radGrayscaleZ->setChecked(!checked);
}

void SettingsFormController::onRadioBtnGrayscaleZToggled(bool checked)
{
    mUi.radGrayscaleS->setChecked(!checked);
}

