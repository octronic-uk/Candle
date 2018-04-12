/*
 * StateFormController.h
 *
 * Created: 22 2018 by Ashley
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

#include "AbstractFormController.h"
#include "ui_StateForm.h"
#include <QMetaType>
#include <QColor>
#include <QVector3D>

using namespace Ui;

class StateFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit StateFormController(QWidget *parent = nullptr);
    ~StateFormController() override;

    void setFormActive(bool active) override;
    void initialise() override;
signals:
public slots:
    void onStatusTextUpdate(QString text, QColor textColor = QColor("Black"), QColor bgColor = QColor("White"));
    void onToolTextUpdate(QString text, QColor textColor = QColor("Black"), QColor bgColor = QColor("White"));
    void onUpdateMachinePosition(const QVector3D);
    void onUpdateWorkPosition(const QVector3D);
private:
    StateForm mUi;

    // AbstractFormController interface
public:
    void setupSignalSlots() override;
};
