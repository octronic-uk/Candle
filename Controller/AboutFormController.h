// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include "AbstractFormController.h"
#include "ui_AboutForm.h"

using namespace Ui;

class AboutFormController : public AbstractFormController
{
    Q_OBJECT

public:
    explicit AboutFormController(QWidget *parent = nullptr);
    ~AboutFormController() override;
    void setFormActive(bool active) override;

    void initialise() override;

private slots:
    void onCmdOkClicked();
    void onLblAboutLinkActivated(const QString &link);
private:
    QDialog mDialog;
    AboutForm mUi;

    // AbstractFormController interface
public:
    void setupSignalSlots() override;
    void exec();
};
