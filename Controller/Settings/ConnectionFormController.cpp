/*
 * ConnectionConnectionFormController.cpp
 *
 * Created: 08 2018 by Ashley
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

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "ConnectionFormController.h"
#include "Model/SerialBaudRate.h"
#include "Model/Settings/Settings.h"

ConnectionFormController::ConnectionFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    mUi.setupUi(this);
    searchPorts();
}

ConnectionFormController::~ConnectionFormController()
{
}

void ConnectionFormController::setupSignalSlots()
{
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
}

void ConnectionFormController::setFormActive(bool)
{

}

void ConnectionFormController::initialise()
{
    setPortName("");
    setBaudRate(SerialBaudRate::BAUD_115200);
    setIgnoreErrors(false);
    setArcLength(0.0);
    setArcDegreeMode(true);
    setArcDegree(5.0);

}

void ConnectionFormController::onSerialPortRefreshClicked()
{
    searchPorts();
}

void ConnectionFormController::onSerialPortNameChanged(QString port)
{
   emit settingChangedSignal(Settings::SERIAL, Settings::SERIAL_PORT_NAME, port);
}

void ConnectionFormController::onSerialBaudRateChanged(QString baud)
{
    emit settingChangedSignal(Settings::SERIAL, Settings::SERIAL_BAUD_RATE, baud.toInt());
}

void ConnectionFormController::onIgnoreErrorsToggled(bool value)
{
}

void ConnectionFormController::onAutoLineToggled(bool value)
{
}

void ConnectionFormController::onArcLengthModeToggled(bool value)
{
}

void ConnectionFormController::onArcLengthValueChanged(QString)
{

}

void ConnectionFormController::onArcDegreeModeToggled(bool)
{

}

void ConnectionFormController::onArcDegreeValueChanged(QString)
{

}

QString ConnectionFormController::getPortName()
{
    return mUi.serialPortNameBox->currentText();
}

void ConnectionFormController::setPortName(QString port)
{
    mUi.serialPortNameBox->setCurrentText(port);
}

int ConnectionFormController::getBaudRate()
{
    return mUi.serialBaudRateBox->currentText().toInt();
}

void ConnectionFormController::setBaudRate(int baud)
{
    mUi.serialBaudRateBox->setCurrentText(QString::number(baud));
}

double ConnectionFormController::arcLength()
{
    return mUi.txtArcLength->value();
}

void ConnectionFormController::setArcLength(double arcPrecision)
{
    mUi.txtArcLength->setValue(arcPrecision);
}

double ConnectionFormController::arcDegree()
{
    return mUi.txtArcDegree->value();
}

void ConnectionFormController::setArcDegree(double arcDegree)
{
    mUi.txtArcDegree->setValue(arcDegree);
}

double ConnectionFormController::arcPrecision()
{
    return mUi.radArcDegreeMode->isChecked() ? mUi.txtArcDegree->value() : mUi.txtArcLength->value();
}

bool ConnectionFormController::arcDegreeMode()
{
    return mUi.radArcDegreeMode->isChecked();
}

void ConnectionFormController::setArcDegreeMode(bool arcDegreeMode)
{
    mUi.radArcDegreeMode->setChecked(arcDegreeMode);
}

void ConnectionFormController::onSettingChanged(QString group, QString param, QVariant value)
{
   if (group == Settings::SERIAL)
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
}

bool ConnectionFormController::ignoreErrors()
{
    return mUi.chkIgnoreErrors->isChecked();
}

void ConnectionFormController::setIgnoreErrors(bool value)
{
    mUi.chkIgnoreErrors->setChecked(value);
}

bool ConnectionFormController::autoLine()
{
    return mUi.chkAutoLine->isChecked();
}

void ConnectionFormController::setAutoLine(bool value)
{
    mUi.chkAutoLine->setChecked(value);
}

void ConnectionFormController::searchPorts()
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
