// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include <QDesktopServices>
#include <QApplication>
#include <QFile>
#include <QUrl>
#include "AboutFormController.h"

AboutFormController::AboutFormController(QWidget *parent)
    : AbstractFormController(parent)
{

    qDebug() << "AboutFormController: Constructing";
    mUi.setupUi(&mDialog);
    mUi.lblAbout->setText(
         mUi.lblAbout->text().arg(qApp->applicationVersion())
    );

    QFile file(":/Misc/LICENSE");

    if (file.open(QIODevice::ReadOnly))
    {
        mUi.txtLicense->setPlainText(file.readAll());
    }

    setupSignalSlots();
}

AboutFormController::~AboutFormController()
{
    qDebug() << "AboutFormController: Destructing";
}

void AboutFormController::setFormActive(bool active)
{
   if (active)
   {

   }
   else
   {

   }
}

void AboutFormController::initialise()
{

}

void AboutFormController::exec()
{
    mDialog.exec();
}

void AboutFormController::onCmdOkClicked()
{
    mDialog.hide();
}

void AboutFormController::onLblAboutLinkActivated(const QString &link)
{
    QDesktopServices::openUrl(link);
}

void AboutFormController::setupSignalSlots()
{
    qDebug() << "AboutFormController: Setup Signals/Slots";
    connect(
        mUi.cmdOk, SIGNAL(clicked()),
        this, SLOT(onCmdOkClicked())
    );
    connect(
        mUi.lblAbout, SIGNAL(linkActivated(const QString&)),
        this, SLOT(onLblAboutLinkActivated(const QString&))
    );

}
