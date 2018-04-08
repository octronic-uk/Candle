// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "SettingsFormController.h"


#include <QDebug>
#include <QScrollBar>
#include <QColorDialog>
#include "ui_SettingsForm.h"

SettingsFormController::SettingsFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "SettingsFormController: Constructing";
    mUi.setupUi(&mDialog);
    setupSignalSlots();
    mDialog.setLocale(QLocale::C);
    //setTabIcons();
}

SettingsFormController::~SettingsFormController()
{
    qDebug() << "SettingsFormController: Destructing";
}

void SettingsFormController::setTabIcons()
{
   mUi.settingsTabs->setTabIcon(0,QIcon(":/Images/SVG/plug.svg"));
   mUi.settingsTabs->setTabIcon(1,QIcon(":/Images/SVG/desktop.svg"));
   mUi.settingsTabs->setTabIcon(2,QIcon(":/Images/SVG/tachometer-alt.svg"));
   mUi.settingsTabs->setTabIcon(3,QIcon(":/Images/SVG/wrench.svg"));
}

int SettingsFormController::exec()
{
    return mDialog.exec();
}

void SettingsFormController::initialise()
{

}

void SettingsFormController::setFormActive(bool active)
{
    getConnectionFormController()->setFormActive(active);
    getMachineFormController()->setFormActive(active);
    getInterfaceFormController()->setFormActive(active);
    getToolHolderFormController()->setFormActive(active);
}

ToolHolderFormController* SettingsFormController::getToolHolderFormController()
{
    return mUi.toolHolderFormController;
}

ConnectionFormController*SettingsFormController::getConnectionFormController()
{
    return mUi.connectionFormController;
}

MachineFormController*SettingsFormController::getMachineFormController()
{
    return mUi.machineFormController;
}

InterfaceFormController*SettingsFormController::getInterfaceFormController()
{
   return mUi.interfaceFormController;
}

void SettingsFormController::onProfilesListModel_ListModelReady_Signal
(QSharedPointer<ProfilesListModel> model)
{
    mUi.profilesComboBox->setModel(model.data());
}

void SettingsFormController::onSettingChanged(QString group, QString param, QVariant value)
{
    /*
    qDebug() << "SettingsFormController: onSettingChanged"
             << group
             << param
             << value;
    */

   if (group == Settings::GFX)
   {

   }

   else if (group == Settings::HEIGHT_MAP)
   {

   }

   else if (group == Settings::TOOL)
   {

   }
   else if (group == Settings::VISUALISER)
   {

   }
}

void SettingsFormController::setupSignalSlots()
{
    qDebug() << "SettingsFormController: Setup Signals/Slots";

    // Main ----------------------------------------------------------------------
    connect(mUi.closeButton, SIGNAL(clicked()),SLOT(onCloseButtonClicked()));
    connect(mUi.restoreDefaultsButton, SIGNAL(clicked()),SLOT(onRestoreDefaultsButtonClicked()));
}

void SettingsFormController::onCloseButtonClicked()
{
    mDialog.accept();
}

void SettingsFormController::onRestoreDefaultsButtonClicked()
{
    QMessageBox::StandardButton result = QMessageBox::warning
    (
        &mDialog,
        qApp->applicationDisplayName(),
        tr("Restore Default Settings?"),
        QMessageBox::Cancel | QMessageBox::Ok
    );

    if (result != QMessageBox::Ok)
    {
        return;
    }

    getConnectionFormController()->initialise();
    getInterfaceFormController()->initialise();
    getMachineFormController()->initialise();
    getToolHolderFormController()->initialise();
}



