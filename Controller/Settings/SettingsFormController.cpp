// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "SettingsFormController.h"


#include <QDebug>
#include <QScrollBar>
#include <QColorDialog>
#include "Controller/Settings/NameDialogController.h"
#include "ui_SettingsForm.h"

SettingsFormController::SettingsFormController(QWidget *parent)
    : AbstractFormController(parent),
      mSettingsModelHandle(nullptr)
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

ConnectionFormController* SettingsFormController::getConnectionFormController()
{
    return mUi.connectionFormController;
}

MachineFormController* SettingsFormController::getMachineFormController()
{
    return mUi.machineFormController;
}

InterfaceFormController* SettingsFormController::getInterfaceFormController()
{
   return mUi.interfaceFormController;
}

void SettingsFormController::onSettingsModelReady
(SqlSettingsModel *model)
{
    qDebug() << "SettingsFormController: Model Ready";
    mSettingsModelHandle = model;
    mUi.profilesComboBox->setModel(model->getProfilesListModelHandle());
    getConnectionFormController()->setSettingsModel(mSettingsModelHandle);
    getInterfaceFormController()->setSettingsModel(mSettingsModelHandle);
    getMachineFormController()->setSettingsModel(mSettingsModelHandle);
    getToolFormController()->setSettingsModel(mSettingsModelHandle);
    getToolHolderFormController()->setSettingsModel(mSettingsModelHandle);
}

void SettingsFormController::onProfileComboBoxCurrentIndexChanged(int index)
{
    qDebug() << "SettinngsFormController: Profile combo box index changed" << index;
    Profile* profile = mSettingsModelHandle->getProfileFromModelAtIndex(index);
    mSettingsModelHandle->setCurrentProfileHandle(profile);
    getToolFormController()->onProfileChanged(profile);
    getToolHolderFormController()->onProfileChanged(profile);
    getConnectionFormController()->onProfileChanged(profile);
    getMachineFormController()->onProfileChanged(profile);
    getInterfaceFormController()->onProfileChanged(profile);

}

void SettingsFormController::onProfileRenameButtonClicked()
{
    qDebug() << "SettinngsFormController: Profile Rename clicked";
    NameDialogController dialog(&mDialog);
    Profile *profile = mSettingsModelHandle->getCurrentProfileHandle();
    dialog.setWindowTitle("Rename Profile");
    dialog.setNameText(profile->getName());
    int response = dialog.exec();

    switch (response)
    {
        case QDialog::Accepted:
            qDebug() << "SettingsFormController: Rename Accepted" << dialog.getNameText();
            mSettingsModelHandle->updateProfileName(profile, dialog.getNameText());
            break;
        case QDialog::Rejected:
            qDebug() << "SettingsFormController: Rename Cancelled";
            break;
    }
}

void SettingsFormController::onProfileAddButtonClicked()
{
    qDebug() << "SettinngsFormController: Profile add clicked";
    NameDialogController dialog(&mDialog);
    dialog.setWindowTitle("Create Profile");
    dialog.setNameText("");
    int response = dialog.exec();

    switch (response)
    {
        case QDialog::Accepted:
            qDebug() << "SettingsFormController: Create Accepted" << dialog.getNameText();
            mSettingsModelHandle->createNewProfile(dialog.getNameText());
            break;
        case QDialog::Rejected:
            qDebug() << "SettingsFormController: Create Cancelled";
            break;
    }
}

void SettingsFormController::onProfileRemoveButtonClicked()
{
    qDebug() << "SettinngsFormController: Profile Remove Clicked";
    if (!mSettingsModelHandle)
    {
        qDebug() << "SettinngsFormController: Settings model is null";
        return;
    }

   int result = QMessageBox::warning
   (
        this,
        "Delete Profile",
        QString("Are you sure you want to delete this profile?\n\n%1")
            .arg(mSettingsModelHandle->getCurrentProfileHandle()->getName()),
        QMessageBox::Ok | QMessageBox::Cancel
   );

   switch (result)
   {
       case QMessageBox::Ok:
           break;
       case QMessageBox::Cancel:
           break;
   }
}

void SettingsFormController::setupSignalSlots()
{
    qDebug() << "SettingsFormController: Setup Signals/Slots";

    // Lower Buttons
    connect(mUi.closeButton, SIGNAL(clicked()),SLOT(onCloseButtonClicked()));
    connect(mUi.restoreDefaultsButton, SIGNAL(clicked()),SLOT(onRestoreDefaultsButtonClicked()));

    // Profile Controls
    connect(
        mUi.profilesComboBox,SIGNAL(currentIndexChanged(int)),
        this, SLOT(onProfileComboBoxCurrentIndexChanged(int))
    );
    connect
    (
        mUi.profileRenameButton, SIGNAL(clicked()),
        this, SLOT(onProfileRenameButtonClicked())
    );
    connect(
        mUi.profileAddButton,SIGNAL(clicked()),
        this, SLOT(onProfileAddButtonClicked())
    );
    connect(
        mUi.profileRemoveButton,SIGNAL(clicked()),
        this, SLOT(onProfileRemoveButtonClicked())
    );

}

ToolFormController* SettingsFormController::getToolFormController()
{
   return mUi.toolsFormController;
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
