/*
 * ToolHolderModelForm.h
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
#include "Model/Settings/ToolHolder/ToolHolderListModel.h"
#include "Model/Settings/ToolHolder/ToolHolderGeometryTableModel.h"
#include "ui_ToolHolderForm.h"

class ToolHolderFormController : public AbstractFormController
{
    Q_OBJECT

public:
    explicit ToolHolderFormController(QWidget *parent = nullptr);
    ~ToolHolderFormController() override;

    void setupSignalSlots() override;
    void setFormActive(bool) override;
    void initialise() override;

signals:
    void settingChangedSignal(QString, QString, QVariant);

public slots:
    void onToolHolderModel_ListModelReady(QSharedPointer<ToolHolderModelListModel> model);

private slots:
    // Buttons
    void onAddToolHolderModelButtonClicked();
    void onRemoveToolHolderModelButtonClicked();
    void onAddToolHolderModelGeometryButtonClicked();
    void onRemoveToolHolderModelGeometryButtonClicked();
    void onToolHolderModelListSelectionChanged(const QItemSelection& selected, const QItemSelection& delelected);
    void onToolHolderModelNameChanged(QString);
    void onToolHolderGeometrySelectionChanged(const QItemSelection&, const QItemSelection&);

private: // Members
    Ui::ToolHolderModelForm mUi;
    QSharedPointer<ToolHolderModelListModel> mListModel;
    const static QString DEFAULT_NAME;
    QSharedPointer<ToolHolder> mSelected;

private: // Member Functions
    void toolHolderModelSelectionValid(bool isValid);


};
