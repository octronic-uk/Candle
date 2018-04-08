/*
 * ToolModelForm.h
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
#include "Model/Settings/Tool/ToolListModel.h"
#include "Model/Settings/Tool/ToolGeometryTableModel.h"
#include "ui_ToolForm.h"

class ToolFormController : public AbstractFormController
{
    Q_OBJECT

public:
    explicit ToolFormController(QWidget *parent = nullptr);
    ~ToolFormController() override;

    void setupSignalSlots() override;
    void setFormActive(bool) override;
    void initialise() override;

signals:
    void settingChangedSignal(QString, QString, QVariant);

public slots:
    void onToolModel_ListModelReady(QSharedPointer<ToolModelListModel> model);

private slots:
    // Buttons
    void onAddToolModelButtonClicked();
    void onRemoveToolModelButtonClicked();
    void onAddToolModelGeometryButtonClicked();
    void onRemoveToolModelGeometryButtonClicked();
    void onToolModelListSelectionChanged(const QItemSelection& selected, const QItemSelection& delelected);
    void onToolModelNameChanged(QString);
    void onToolGeometrySelectionChanged(const QItemSelection&, const QItemSelection&);

private: // Members
    Ui::ToolModelForm mUi;
    QSharedPointer<ToolModelListModel> mListModel;
    const static QString DEFAULT_NAME;
    QSharedPointer<Tool> mSelected;

private: // Member Functions
    void toolModelSelectionValid(bool isValid);


};
