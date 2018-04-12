/*
 * ToolFormController.h
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
#include "Model/Settings/Profile/Profile.h"
#include "Model/Settings/Tool/ToolListModel.h"
#include "Model/Settings/Tool/ToolGeometryTableModel.h"
#include "Model/Settings/ToolHolder/ToolHolderListModel.h"
#include "ui_ToolForm.h"

class SqlSettingsModel;

class ToolFormController : public AbstractFormController
{
    Q_OBJECT

public:
    explicit ToolFormController(QWidget *parent = nullptr);
    ~ToolFormController() override;

    void setupSignalSlots() override;
    void setFormActive(bool) override;
    void initialise() override;

    void setSettingsModel(SqlSettingsModel* handle);

public slots:
    void onProfileChanged(Profile* model);

private slots:
    void onToolListSelectionChanged(const QItemSelection& selected, const QItemSelection& delelected);
    void onToolHolderComboBoxIndexChanged(int currentIndex);
    void onToolNumberSpinBoxValueChanged(int value);
    void onToolNameEditingFinished();
    void onAddToolButtonClicked();
    void onRemoveToolButtonClicked();

    void onAddToolGeometryButtonClicked();
    void onRemoveToolGeometryButtonClicked();
    void onGeometrySelectionChanged(const QItemSelection&, const QItemSelection&);
    void onGeometryUpdated(ToolGeometry*);


private: // Members
    Ui::ToolForm mUi;
    SqlSettingsModel* mSettingsModelHandle;

private: // Member Functions
    void toolSelectionValid(bool isValid);
};
