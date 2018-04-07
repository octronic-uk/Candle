/*
 * ToolHolderModelForm.cpp
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
#include "ToolHolderModelFormController.h"
#include "ui_ToolHolderModelForm.h"

#include <QtDebug>
#include <QMessageBox>

ToolHolderModelFormController::ToolHolderModelFormController(QWidget *parent)
    : QWidget(parent)
{
    mUi.setupUi(this);
    mUi.splitter->setStretchFactor(0,1);
    mUi.splitter->setStretchFactor(1,2);
    mUi.toolHoldersListView->setModel(&mListModel);
    toolHolderModelSelectionValid(false);
    mUi.geometryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setupSignalsSlots();
}

ToolHolderModelFormController::~ToolHolderModelFormController() {}

void ToolHolderModelFormController::setupSignalsSlots()
{
    // Add tool holder
    connect
    (
        mUi.toolHoldersAddButton, SIGNAL(clicked()),
        this, SLOT(onAddToolHolderModelButtonClicked())
    );
    // Remove tool holder
    connect
    (
        mUi.toolHoldersRemoveButton, SIGNAL(clicked()),
        this, SLOT(onRemoveToolHolderModelButtonClicked())
    );
    // Add tool holder geometry
    connect
    (
        mUi.geometryAddButton, SIGNAL(clicked()),
        this, SLOT(onAddToolHolderModelGeometryButtonClicked())
    );
    // Remove tool holder geometry
    connect
    (
        mUi.geometryRemoveButton, SIGNAL(clicked()),
        this, SLOT(onRemoveToolHolderModelGeometryButtonClicked())
    );
    // Tool Model Selection Changed
    connect
    (
        mUi.toolHoldersListView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this,
        SLOT(onToolHolderModelListSelectionChanged(const QItemSelection&, const QItemSelection&))

    );
    // Tool Holder Name
    connect
    (
        mUi.nameLineEdit, SIGNAL(textChanged(QString)),
        this, SLOT(onToolHolderModelNameChanged(QString))
    );
    // Geometry Table Selection
    connect
    (
        mUi.geometryTable->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this,
        SLOT(onToolHolderGeometrySelectionChanged(const QItemSelection&, const QItemSelection&))
    );
}

void ToolHolderModelFormController::onToolHolderModelListSelectionChanged
(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(deselected)
    if (selected.count() == 0)
    {
        mSelected.clear();
        return;
    }

    int selectedRow = selected.indexes().first().row();

    qDebug() << "ToolHolderModelFormController: Selection Changed" << selectedRow;
    mSelected = mListModel.getData(selectedRow);
    toolHolderModelSelectionValid(mSelected != nullptr);
    mUi.toolHoldersRemoveButton->setEnabled(mSelected != nullptr);
}

void ToolHolderModelFormController::onToolHolderModelNameChanged(QString txt)
{
    if (mSelected != nullptr)
        mSelected->setName(txt);

   emit mListModel.dataChanged(QModelIndex(),QModelIndex());
}

void ToolHolderModelFormController::onAddToolHolderModelButtonClicked()
{
   qDebug() << "ToolHolderModelFormController::onAddToolHolderModelButtonClicked()";
   mListModel.insert(QSharedPointer<ToolHolderModelListItem>::create(DEFAULT_NAME));
}

void ToolHolderModelFormController::onRemoveToolHolderModelButtonClicked()
{
   qDebug() << "ToolHolderModelFormController::onRemoveToolHolderModelButtonClicked()";
   if (mSelected == nullptr)
   {
       return;
   }

   int result = QMessageBox::warning
   (
        this,
        "Are you sure?",
        QString("Are you sure?\n\nAny tools using this Tool Holder will need to be reassigned\n\nDelete: '%1'?").arg(mSelected->getName()),
        QMessageBox::Ok | QMessageBox::Cancel
   );

   switch (result)
   {
       case QMessageBox::Ok:
           mListModel.remove(mSelected);
           mSelected.clear();
           mUi.nameLineEdit->setText("");
           mUi.geometryTable->setModel(nullptr);
           mUi.geometryTable->setEnabled(false);
           mUi.nameLineEdit->setEnabled(false);
           mUi.nameLabel->setEnabled(false);
           mUi.geometryAddButton->setEnabled(false);
           mUi.geometryRemoveButton->setEnabled(false);
           mUi.geometryLabel->setEnabled(false);
           mUi.toolHoldersListView->clearSelection();
           break;
       case QMessageBox::Cancel:
           break;
   }
}

void ToolHolderModelFormController::onAddToolHolderModelGeometryButtonClicked()
{
   qDebug() << "ToolHolderModelFormController::onAddTooHolderlModelGeometryButtonClicked()";
   mSelected->addNewRow();
}

void ToolHolderModelFormController::onRemoveToolHolderModelGeometryButtonClicked()
{
   qDebug() << "ToolHolderModelFormController::onRemoveToolHolderModelGeometryButtonClicked()";
}

void ToolHolderModelFormController::toolHolderModelSelectionValid(bool isValid)
{
    mUi.geometryAddButton->setEnabled(isValid);
    mUi.geometryRemoveButton->setEnabled(isValid);
    mUi.geometryTable->setEnabled(isValid);
    mUi.nameLineEdit->setEnabled(isValid);
    mUi.nameLabel->setEnabled(isValid);
    mUi.geometryLabel->setEnabled(isValid);

    if (isValid)
    {
       mUi.nameLineEdit->setText(mSelected->getName());
       mUi.geometryTable->setModel(mSelected->getTableModel().data());
       mUi.geometryTable->setEditTriggers(QAbstractItemView::DoubleClicked);
    }
}

void ToolHolderModelFormController::onToolHolderGeometrySelectionChanged
(const QItemSelection &selected, const QItemSelection &deselected)
{
    qDebug() << "ToolHolderModelFormController::onToolHolderGeometrySelectionChanged";
    Q_UNUSED(deselected)
    if(selected.indexes().count() > 0)
    {
        qDebug() << "ToolHolderModelFormController: row valid";
        mSelected->setSelectedRow(selected.indexes().first().row());
    }
}

const QString ToolHolderModelFormController::DEFAULT_NAME = "New Tool Holder";
