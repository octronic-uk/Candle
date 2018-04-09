// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include "Controller/AbstractFormController.h"
#include "Model/Settings/Settings.h"
#include "Model/Settings/Profile/ProfilesListModel.h"
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

public slots:
    void onProfileListModelReady(ProfilesListModel*);
    void onSettingChanged(QString group, QString param, QVariant value);

signals:
    void settingChangedSignal(QString group, QString param, QVariant value);

private slots:
    void onCloseButtonClicked();
    void onRestoreDefaultsButtonClicked();


private:
    QDialog mDialog;
    SettingsForm mUi;


    void setTabIcons();

};

