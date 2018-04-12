/*
 * ToolHolderForm.h
 *
 * Created: 06 2018 by Ashley
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

#include "Controller/AbstractFormController.h"
#include "ui_ToolHolderForm.h"

class SqlSettingsModel;
class ToolHolderGeometry;
class Profile;

class ToolHolderFormController : public AbstractFormController
{
    Q_OBJECT

public:
    explicit ToolHolderFormController(QWidget *parent = nullptr);
    ~ToolHolderFormController() override;

    void setupSignalSlots() override;
    void setFormActive(bool) override;
    void initialise() override;

    void setSettingsModel(SqlSettingsModel* handle);
signals:
    void settingChangedSignal(QString, QString, QVariant);

public slots:

    void onProfileChanged(Profile* model);

private slots:
    void onToolHolderEditingFinished();
    void onToolHolderListSelectionChanged(const QItemSelection& selected, const QItemSelection& delelected);
    void onAddToolHolderButtonClicked();
    void onRemoveToolHolderButtonClicked();


    void onAddToolHolderGeometryButtonClicked();
    void onRemoveToolHolderGeometryButtonClicked();
    void onGeometrySelectionChanged(const QItemSelection&, const QItemSelection&);
    void onGeometryUpdated(ToolHolderGeometry*);

private: // Members
    Ui::ToolHolderForm mUi;
    SqlSettingsModel *mSettingsModelHandle;

private: // Member Functions
    void toolHolderModelSelectionValid(bool isValid);


};
