// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include "Controller/AbstractFormController.h"
#include "Model/Settings/Sql/SqlSettingsModel.h"
#include "Controller/Settings/GrblConfigurationFormController.h"
#include "ui_SettingsForm.h"

using namespace Ui;

class SettingsFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit SettingsFormController(QWidget *parent = nullptr);
    ~SettingsFormController() override;

    void saveSettings();
    int exec();

    void initialise() override;
    void setFormActive(bool active) override;
    void setupSignalSlots() override;

    ToolFormController* getToolFormController();
    ToolHolderFormController* getToolHolderFormController();
    ConnectionFormController* getConnectionFormController();
    MachineFormController* getMachineFormController();
    InterfaceFormController* getInterfaceFormController();
    GrblConfigurationFormController *getGrblConfigurationFormControllerHandle();
public slots:
    void onSettingsModelReady(SqlSettingsModel*);
    void onProfileComboBoxCurrentIndexChanged(int);
    void onProfileRenameButtonClicked();
    void onProfileAddButtonClicked();
    void onProfileRemoveButtonClicked();
    void onGrblConfigButtonClicked();
    void onFirmwareConfigurationRead(int,QString);

signals:
    void settingChangedSignal(QString group, QString param, QVariant value);
    void gcodeCommandManualSendSignal(GcodeCommand*);

private slots:
    void onCloseButtonClicked();
    void onRestoreDefaultsButtonClicked();


private:
    QDialog mDialog;
    SettingsForm mUi;
    SqlSettingsModel* mSettingsModelHandle;
    GrblConfigurationFormController mGrblConfigurationFormController ;

    void setTabIcons();

};

