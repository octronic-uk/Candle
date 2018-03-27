// This file is a part of "Candle" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include "AbstractFormController.h"
#include "ui_AboutForm.h"

using namespace Ui;

class AboutFormController : public AbstractFormController
{
    Q_OBJECT

public:
    explicit AboutFormController(QWidget *parent = 0);
    ~AboutFormController();

private slots:
    void on_cmdOk_clicked();
    void on_lblAbout_linkActivated(const QString &link);
private:
    QDialog mDialog;
    AboutForm mUi;
};
