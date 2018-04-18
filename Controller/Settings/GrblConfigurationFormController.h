/*
 * GrblConfigurationFormController.h
 *
 * Created: 18 2018 by Ashley
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

#include <QDialog>
#include "Model/Grbl/GrblConfigurationModel.h"
#include <ui_GrblConfigurationForm.h>

class GcodeCommand;

using namespace Ui;

class GrblConfigurationFormController : public QDialog
{
    Q_OBJECT
public:
    explicit GrblConfigurationFormController(QWidget *parent = nullptr);
    ~GrblConfigurationFormController() override;


signals:
    void gcodeCommandManualSendSignal(GcodeCommand*);

public slots:
    void onFirmwareConfigurationRead(int param, QString value);
private slots:
    void onReadButtonClicked();
    void onWriteButtonClicked();
private:
    GrblConfigurationForm mUi;
    GrblConfigurationModel mTableModel;
    void setupSignalSlots();
};
