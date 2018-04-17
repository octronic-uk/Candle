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
#include "Model/Settings/Sql/SqlSettingsModel.h"
#include "Model/SerialBaudRate.h"
#include "Model/Settings/Settings.h"

ConnectionFormController::ConnectionFormController(QWidget *parent)
    : AbstractFormController(parent),
      mSettingsModelHandle(nullptr)
{
    mUi.setupUi(this);
    setupSignalSlots();
    searchPorts();
}

ConnectionFormController::~ConnectionFormController()
{
}

void ConnectionFormController::setupSignalSlots()
{
    // Serial
    connect(mUi.serialPortRefreshButton, SIGNAL(clicked()), SLOT(onSerialPortRefreshClicked()));
    connect(mUi.serialPortNameBox, SIGNAL(activated(QString)), SLOT(onSerialPortNameChanged(QString)));
    connect(mUi.serialBaudRateBox, SIGNAL(activated(QString)), SLOT(onSerialBaudRateChanged(QString)));
    // Sender
    connect(mUi.chkIgnoreErrors, SIGNAL(toggled(bool)), this, SLOT(onIgnoreErrorsToggled(bool)));
    connect(mUi.chkAutoParserState, SIGNAL(toggled(bool)), this, SLOT(onAutoLineToggled(bool)));
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
    if (isModelValid())
    {
        auto settings = getSettings();
        setPortName(settings->getSerialPort());
        setBaudRate(settings->getSerialBaudRate());
        setIgnoreErrors(settings->getIgnoreErrorMessages());
        setAutoLine(settings->getSetParserState());
        setArcDegreeMode(settings->getArcApproximation());
        setArcLength(settings->getArcApproximationLength());
        setArcDegree(settings->getArcApproximationDegrees());

        emit serialPortNameChangedSignal(settings->getSerialPort());
        emit serialPortBaudRateChangedSignal(settings->getSerialBaudRate());
    }
}

void ConnectionFormController::onSerialPortRefreshClicked()
{
    searchPorts();
}

bool ConnectionFormController::isModelValid()
{
    if (!mSettingsModelHandle)
    {
        qDebug() << "ConnectionFormController: mSettingsModel = nullptr";
        return false;
    }

    Profile* profile = mSettingsModelHandle->getCurrentProfileHandle();

    if(!profile)
    {
        qDebug() << "ConnectionFormController: profile = nullptr";
        return false;
    }

    ConnectionSettings* settings = profile->getConnectionSettingsHandle();

    if (!settings)
    {
        qDebug() << "ConnectionFormController: settings = nullptr";
        return false;
    }
    return true;
}

ConnectionSettings* ConnectionFormController::getSettings()
{
    return mSettingsModelHandle
            ->getCurrentProfileHandle()
            ->getConnectionSettingsHandle();
}

void ConnectionFormController::commit()
{
    auto settings = getSettings();
    mSettingsModelHandle->onConnecitonSettingsUpdated(settings);
}

void ConnectionFormController::onSerialPortNameChanged(QString port)
{
    qDebug() << "ConnectionFormController: onSerialPortNameChanged" << port;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setSerialPort(port);
        commit();
        emit serialPortNameChangedSignal(port);
    }
}

void ConnectionFormController::onSerialBaudRateChanged(QString baud)
{
    qDebug() << "ConnectionFormController: onSerialBaudRateChanged" << baud;
    if (isModelValid())
    {
        auto settings = getSettings();
        int baudInt = baud.toInt();
        settings->setSerialBaudRate(baudInt);
        commit();
        emit serialPortBaudRateChangedSignal(baudInt);
    }
}

void ConnectionFormController::onIgnoreErrorsToggled(bool value)
{
    qDebug() << "ConnectionFormController: onIgnoreErrors" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setIgnoreErrorMessages(value);
        commit();
    }
}

void ConnectionFormController::onAutoLineToggled(bool autoLine)
{
    qDebug() << "ConnectionFormController: onAutoLineToggled" << autoLine;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setSetParserState(autoLine);
        commit();
    }
}

void ConnectionFormController::onArcLengthModeToggled(bool value)
{
    qDebug() << "ConnectionFormController: onArcLengthMode" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        mUi.radArcDegreeMode->setChecked(!value);
        mUi.txtArcDegree->setEnabled(!value);
        mUi.txtArcLength->setEnabled(value);
        settings->setArcApproximation(value);
        commit();
    }
}

void ConnectionFormController::onArcLengthValueChanged(QString value)
{
    qDebug() << "ConnectionFormController: onArcLengthValueChanged" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setArcApproximationLength(value.toFloat());
        commit();
    }
}

void ConnectionFormController::onArcDegreeModeToggled(bool value)
{
    qDebug() << "ConnectionFormController: onArcDegreeChanged" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        mUi.radArcLengthMode->setChecked(!value);
        mUi.txtArcDegree->setEnabled(value);
        mUi.txtArcLength->setEnabled(!value);
        settings->setArcApproximation(!value);
        commit();
    }
}

void ConnectionFormController::onArcDegreeValueChanged(QString value)
{
    qDebug() << "ConnectionFormController: onArcDegreeValueChanged" << value;
    if (isModelValid())
    {
        auto settings = getSettings();
        settings->setArcApproximationDegrees(value.toFloat());
        commit();
    }
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
    return mUi.chkAutoParserState->isChecked();
}

void ConnectionFormController::setAutoLine(bool value)
{
    mUi.chkAutoParserState->setChecked(value);
}

void ConnectionFormController::setSettingsModel(SqlSettingsModel* handle)
{
    mSettingsModelHandle = handle;
    initialise();
}

void ConnectionFormController::onProfileChanged(Profile*)
{
    initialise();
}

void ConnectionFormController::searchPorts()
{
    mUi.serialPortNameBox->clear();

    foreach (QSerialPortInfo info, QSerialPortInfo::availablePorts())
    {
        mUi.serialPortNameBox->addItem(info.portName());
    }

    mUi.serialBaudRateBox->clear();

    for (int baud : SERIAL_BAUDS)
    {
        mUi.serialBaudRateBox->addItem(QString::number(baud),baud);
    }
}
