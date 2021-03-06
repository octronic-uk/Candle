/*
 * UserCommandsFormController.h
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
#include "ui_UserCommandsForm.h"

using namespace Ui;

class UserCommandsFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit UserCommandsFormController(QWidget *parent = nullptr);
    ~UserCommandsFormController() override;

    void setFormActive(bool active) override;
    void initialise() override;
signals:
public slots:

    // User Commands
    void onCmdUserClicked(bool checked);
private:
    UserCommandsForm mUi;

    // AbstractFormController interface
public:
    void setupSignalSlots() override;
};
