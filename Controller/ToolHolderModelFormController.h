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

#include <QWidget>
#include "Model/ToolHolderModelListModel.h"
#include "Model/ToolHolderModelTableModel.h"
#include "ui_ToolHolderModelForm.h"

class ToolHolderModelFormController : public QWidget
{
    Q_OBJECT

public:
    explicit ToolHolderModelFormController(QWidget *parent = nullptr);
    ~ToolHolderModelFormController();

protected:
    void setupSignalsSlots();

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
    ToolHolderModelListModel mListModel;
    const static QString DEFAULT_NAME;
    QSharedPointer<ToolHolderModelListItem> mSelected;

private: // Member Functions
    void toolHolderModelSelectionValid(bool isValid);
};
