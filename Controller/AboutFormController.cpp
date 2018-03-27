// This file is a part of "Candle" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include <QDesktopServices>
#include "AboutFormController.h"

AboutFormController::AboutFormController(QWidget *parent)
    : AbstractFormController(parent)
{

    QMessageLogger().info("Constructing AboutFormController");
    mUi.setupUi(&mDialog);
    mUi.lblAbout->setText(
         mUi.lblAbout->text().arg(qApp->applicationVersion())
    );

    QFile file(qApp->applicationDirPath() + "/LICENSE");

    if (file.open(QIODevice::ReadOnly))
    {
        mUi.txtLicense->setPlainText(file.readAll());
    }
}

AboutFormController::~AboutFormController()
{
    QMessageLogger().info("Destructing AboutFormController");
}

void AboutFormController::on_cmdOk_clicked()
{
    mDialog.hide();
}

void AboutFormController::on_lblAbout_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(link);
}
