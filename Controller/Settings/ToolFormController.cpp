/*
 * ToolModelForm.cpp
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
#include "ToolFormController.h"
#include <QtDebug>
#include <QMessageBox>

ToolFormController::ToolFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    mUi.setupUi(this);
    mUi.splitter->setStretchFactor(0,1);
    mUi.splitter->setStretchFactor(1,2);
    toolModelSelectionValid(false);
    mUi.geometryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setupSignalSlots();
}

ToolFormController::~ToolFormController() {}

void ToolFormController::onToolModel_ListModelReady
(QSharedPointer<ToolModelListModel> model)
{
    mListModel = model;
    mUi.toolsListView->setModel(mListModel.data());
    // Tool Model Selection Changed
    connect
    (
        mUi.toolsListView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this,
        SLOT(onToolModelListSelectionChanged(const QItemSelection&, const QItemSelection&))

    );
}

void ToolFormController::setupSignalSlots()
{
    // Add tool holder
    connect
    (
        mUi.toolsAddButton, SIGNAL(clicked()),
        this, SLOT(onAddToolModelButtonClicked())
    );
    // Remove tool holder
    connect
    (
        mUi.toolsRemoveButton, SIGNAL(clicked()),
        this, SLOT(onRemoveToolModelButtonClicked())
    );
    // Add tool holder geometry
    connect
    (
        mUi.geometryAddButton, SIGNAL(clicked()),
        this, SLOT(onAddToolModelGeometryButtonClicked())
    );
    // Remove tool holder geometry
    connect
    (
        mUi.geometryRemoveButton, SIGNAL(clicked()),
        this, SLOT(onRemoveToolModelGeometryButtonClicked())
    );

    // Tool  Name
    connect
    (
        mUi.nameLineEdit, SIGNAL(textChanged(QString)),
        this, SLOT(onToolModelNameChanged(QString))
    );

}

void ToolFormController::setFormActive(bool)
{

}

void ToolFormController::initialise()
{

}

void ToolFormController::onToolModelListSelectionChanged
(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(deselected)
    if (selected.count() == 0)
    {
        mSelected.clear();
        return;
    }

    int selectedRow = selected.indexes().first().row();

    qDebug() << "ToolModelFormController: Selection Changed" << selectedRow;
    mSelected = mListModel->getData(selectedRow);
    toolModelSelectionValid(mSelected != nullptr);
    mUi.toolsRemoveButton->setEnabled(mSelected != nullptr);
}

void ToolFormController::onToolModelNameChanged(QString txt)
{
    if (mSelected != nullptr)
        mSelected->setName(txt);

   emit mListModel->dataChanged(QModelIndex(),QModelIndex());
}

void ToolFormController::onAddToolModelButtonClicked()
{
   qDebug() << "ToolModelFormController::onAddToolModelButtonClicked()";
   mListModel->insert(QSharedPointer<Tool>::create(-1,DEFAULT_NAME));
}

void ToolFormController::onRemoveToolModelButtonClicked()
{
   qDebug() << "ToolModelFormController::onRemoveToolModelButtonClicked()";
   if (mSelected == nullptr)
   {
       return;
   }

   int result = QMessageBox::warning
   (
        this,
        "Are you sure?",
        QString("Are you sure?\n\nDelete: '%1'?").arg(mSelected->getName()),
        QMessageBox::Ok | QMessageBox::Cancel
   );

   switch (result)
   {
       case QMessageBox::Ok:
           mListModel->remove(mSelected);
           mSelected.clear();
           mUi.nameLineEdit->setText("");
           mUi.geometryTable->setModel(nullptr);
           mUi.geometryTable->setEnabled(false);
           mUi.nameLineEdit->setEnabled(false);
           mUi.nameLabel->setEnabled(false);
           mUi.geometryAddButton->setEnabled(false);
           mUi.geometryRemoveButton->setEnabled(false);
           mUi.geometryLabel->setEnabled(false);
           mUi.toolsListView->clearSelection();
           break;
       case QMessageBox::Cancel:
           break;
   }
}

void ToolFormController::onAddToolModelGeometryButtonClicked()
{
   qDebug() << "ToolModelFormController::onAddToolModelGeometryButtonClicked()";
   mSelected->addNewRow();
}

void ToolFormController::onRemoveToolModelGeometryButtonClicked()
{
   qDebug() << "ToolModelFormController::onRemoveToolModelGeometryButtonClicked()";
}

void ToolFormController::toolModelSelectionValid(bool isValid)
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
       // Geometry Table Selection
       connect
       (
           mUi.geometryTable->selectionModel(),
           SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
           this,
           SLOT(onToolGeometrySelectionChanged(const QItemSelection&, const QItemSelection&))
       );
    }
}

void ToolFormController::onToolGeometrySelectionChanged
(const QItemSelection &selected, const QItemSelection &deselected)
{
    qDebug() << "ToolModelFormController::onToolGeometrySelectionChanged";
    Q_UNUSED(deselected)
    if(selected.indexes().count() > 0)
    {
        qDebug() << "ToolModelFormController: row valid";
        mSelected->setSelectedRow(selected.indexes().first().row());
    }
}

const QString ToolFormController::DEFAULT_NAME = "New Tool";
