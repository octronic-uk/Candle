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
#include "ToolHolderFormController.h"
#include <QtDebug>
#include <QMessageBox>

ToolHolderFormController::ToolHolderFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    mUi.setupUi(this);
    mUi.splitter->setStretchFactor(0,1);
    mUi.splitter->setStretchFactor(1,2);
    toolHolderModelSelectionValid(false);
    mUi.geometryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setupSignalSlots();
}

ToolHolderFormController::~ToolHolderFormController() {}

void ToolHolderFormController::onToolHolderListModelReady
(ToolHolderListModel* model)
{
    qDebug() << "ToolHolderFormController: onToolHolderModel_ListModelReady";
    mListModelHandle = model;
    mUi.toolHoldersListView->setModel(mListModelHandle);
    // Tool Model Selection Changed
    connect
    (
        mUi.toolHoldersListView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this,
        SLOT(onToolHolderListSelectionChanged(const QItemSelection&, const QItemSelection&))
    );
}

void ToolHolderFormController::onToolHolderGeometryCreated(ToolHolderGeometry* item)
{
    qDebug() << "ToolHolderFormController: Forwarding toolHolderGeometryCreated";
    emit toolHolderGeometryCreatedSignal(item);
}

void ToolHolderFormController::onToolHolderGeometryUpdated(ToolHolderGeometry* item)
{
    qDebug() << "ToolHolderFormController: Forwarding toolHolderGeometryUpdated";
    emit toolHolderGeometryUpdatedSignal(item);
}

void ToolHolderFormController::onToolHolderGeometryDeleted(ToolHolderGeometry* item)
{
    qDebug() << "ToolHolderFormController: Forwarding toolHolderGeometryDeleted";
    emit toolHolderGeometryDeletedSignal(item);
}

void ToolHolderFormController::setupSignalSlots()
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
    // Tool Holder Name
    connect
    (
        mUi.nameLineEdit, SIGNAL(editingFinished()),
        this, SLOT(onToolHolderEditingFinished())
    );
}

void ToolHolderFormController::setFormActive(bool)
{

}

void ToolHolderFormController::initialise()
{

}

void ToolHolderFormController::onToolHolderListSelectionChanged
(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(deselected)

    if (mSelectedToolHolderHandle)
    {
        mSelectedToolHolderHandle->disconnect();
    }

    if (selected.count() == 0)
    {
        mSelectedToolHolderHandle = nullptr;
        return;
    }

    int selectedRow = selected.indexes().first().row();

    qDebug() << "ToolHolderModelFormController: Selection Changed to row"
             << selectedRow;

    mSelectedToolHolderHandle = mListModelHandle->getData(selectedRow);

    qDebug() << "ToolHolderModelFormController: Selected ID "
             << mSelectedToolHolderHandle->getID();

    toolHolderModelSelectionValid(mSelectedToolHolderHandle->isIdValid());
    mUi.toolHoldersRemoveButton->setEnabled(mSelectedToolHolderHandle->isIdValid());
}

void ToolHolderFormController::onToolHolderEditingFinished()
{
    if (mSelectedToolHolderHandle->isIdValid())
    {
        mListModelHandle->setData
        (
            mListModelHandle->indexOf(mSelectedToolHolderHandle),
            mUi.nameLineEdit->text(),
            Qt::EditRole
        );
    }
}

void ToolHolderFormController::onAddToolHolderModelButtonClicked()
{
   qDebug() << "ToolHolderModelFormController::onAddToolHolderModelButtonClicked()";
   mListModelHandle->insert(QSharedPointer<ToolHolder>::create(-1,DEFAULT_NAME));
}

void ToolHolderFormController::onRemoveToolHolderModelButtonClicked()
{
   qDebug() << "ToolHolderModelFormController::onRemoveToolHolderModelButtonClicked()";
   if (!mSelectedToolHolderHandle->isIdValid())
   {
       return;
   }

   int result = QMessageBox::warning
   (
        this,
        "Are you sure?",
        QString("Are you sure?\n\nAny tools using this Tool Holder will need to be reassigned\n\nDelete: '%1'?")
            .arg(mSelectedToolHolderHandle->getName()),
        QMessageBox::Ok | QMessageBox::Cancel
   );

   switch (result)
   {
       case QMessageBox::Ok:
           mListModelHandle->remove(mSelectedToolHolderHandle);
           mSelectedToolHolderHandle = nullptr;
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

void ToolHolderFormController::onAddToolHolderModelGeometryButtonClicked()
{
   qDebug() << "ToolHolderModelFormController::onAddTooHolderlModelGeometryButtonClicked()";
   mSelectedToolHolderHandle->addNewGeometryRow();
}

void ToolHolderFormController::onRemoveToolHolderModelGeometryButtonClicked()
{
   qDebug() << "ToolHolderModelFormController::onRemoveToolHolderModelGeometryButtonClicked()";
   mSelectedToolHolderHandle->removeSelectedGeometryRow();
}

void ToolHolderFormController::toolHolderModelSelectionValid(bool isValid)
{
    mUi.geometryAddButton->setEnabled(isValid);
    mUi.geometryRemoveButton->setEnabled(isValid);
    mUi.geometryTable->setEnabled(isValid);
    mUi.nameLineEdit->setEnabled(isValid);
    mUi.nameLabel->setEnabled(isValid);
    mUi.geometryLabel->setEnabled(isValid);

    if (isValid)
    {
       mUi.nameLineEdit->setText(mSelectedToolHolderHandle->getName());

       if (mUi.geometryTable->model())
       {
           mUi.geometryTable->model()->disconnect();
       }

       if (mUi.geometryTable->selectionModel())
       {
           mUi.geometryTable->selectionModel()->disconnect();
       }

       ToolHolderGeometryTableModel* tableModel = mSelectedToolHolderHandle->getGeometryTableModelHandle();

       if (tableModel != nullptr)
       {
           qDebug() << "ToolHolderModelFormController: tableModel is valid";

           mUi.geometryTable->setModel(tableModel);
           mUi.geometryTable->setEditTriggers(QAbstractItemView::DoubleClicked);

           // Geometry Table CRUD
           connect
           (
                tableModel,
                SIGNAL(toolHolderGeometryCreatedSignal(ToolHolderGeometry*)),
                this,
                SLOT(onToolHolderGeometryCreated(ToolHolderGeometry*))
           );
           connect
           (
                tableModel,
                SIGNAL(toolHolderGeometryUpdatedSignal(ToolHolderGeometry*)),
                this,
                SLOT(onToolHolderGeometryUpdated(ToolHolderGeometry*))
           );
           connect
           (
                tableModel,
                SIGNAL(toolHolderGeometryDeletedSignal(ToolHolderGeometry*)),
                this,
                SLOT(onToolHolderGeometryDeleted(ToolHolderGeometry*))
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
       else
       {
           qDebug() << "ToolHolderModelFormController: tableModel is nullptr";
       }
    }
}

void ToolHolderFormController::onToolHolderGeometrySelectionChanged
(const QItemSelection &selected, const QItemSelection &deselected)
{
    qDebug() << "ToolHolderModelFormController::onToolHolderGeometrySelectionChanged";
    Q_UNUSED(deselected)
    if(selected.indexes().count() > 0)
    {
        qDebug() << "ToolHolderModelFormController: row valid";
        mSelectedToolHolderHandle->setSelectedGeometryRow(selected.indexes().first().row());
    }
}

const QString ToolHolderFormController::DEFAULT_NAME = "New Tool Holder";
