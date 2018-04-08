/*
 * ConnectionSettingsFormController.h
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

#pragma once

#include "Controller/AbstractFormController.h"
#include "ui_ConnectionSettingsForm.h"

class ConnectionFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit ConnectionFormController(QWidget *parent = nullptr);
    ~ConnectionFormController() override;

    void setupSignalSlots() override;
    void setFormActive(bool) override;
    void initialise() override;

    QString getPortName();
    void setPortName(QString port);

    int getBaudRate();
    void setBaudRate(int baud);

    double arcLength();
    void setArcLength(double arcLength);

    double arcDegree();
    void setArcDegree(double arcDegree);

    double arcPrecision();

    bool arcDegreeMode();
    void setArcDegreeMode(bool arcDegreeMode);

    bool ignoreErrors();
    void setIgnoreErrors(bool value);

    bool autoLine();
    void setAutoLine(bool value);
public slots:

    void onSettingChanged(QString group, QString param, QVariant value);
    void onSerialPortRefreshClicked();
    void onSerialPortNameChanged(QString);
    void onSerialBaudRateChanged(QString);
    void onIgnoreErrorsToggled(bool);
    void onAutoLineToggled(bool);
    void onArcLengthModeToggled(bool);
    void onArcLengthValueChanged(QString);
    void onArcDegreeModeToggled(bool);
    void onArcDegreeValueChanged(QString);

signals:
    void settingChangedSignal(QString, QString, QVariant);

private:
    void searchPorts();

    Ui::ConnectionSettingsFormController mUi;
};
