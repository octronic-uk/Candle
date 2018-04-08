/*
 * AbstractSettingsModel.h
 *
 * Created: 30 2018 by Ashley
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

#include <QSharedPointer>
#include <QSettings>
#include "Model/SerialBaudRate.h"
#include "Settings.h"
#include <QDir>

class AbstractSettingsModel : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSettingsModel(QObject *parent = nullptr);
    virtual ~AbstractSettingsModel() = 0;

signals:
    void settingChangedSignal(QString groupName, QString settingName, QVariant value);

public slots:
    virtual void onSaveSettings() = 0;
    virtual void onLoadSettings() = 0;
    virtual void onSettingChanged(QString groupName, QString settingName, QVariant value) = 0;

protected:
    bool mSettingsLoading;
};
