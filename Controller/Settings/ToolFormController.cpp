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

void ToolFormController::onToolListModelReady
(ToolListModel* model)
{
    qDebug() << "ToolFormController: onToolModel_ListModelReady";
    mListModelHandle = model;
    mUi.toolsListView->setModel(mListModelHandle);
    // Tool Model Selection Changed
    connect
    (
        mUi.toolsListView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this,
        SLOT(onToolListSelectionChanged(const QItemSelection&, const QItemSelection&))
    );
}

void ToolFormController::onToolGeometryCreated(ToolGeometry* item)
{
    qDebug() << "ToolFormController: Forwarding toolGeometryCreated";
    emit toolGeometryCreatedSignal(item);
}

void ToolFormController::onToolGeometryUpdated(ToolGeometry* item)
{
    qDebug() << "ToolFormController: Forwarding toolGeometryUpdated";
    emit toolGeometryUpdatedSignal(item);
}

void ToolFormController::onToolGeometryDeleted(ToolGeometry* item)
{
    qDebug() << "ToolFormController: Forwarding toolGeometryDeleted";
    emit toolGeometryDeletedSignal(item);
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
        mUi.nameLineEdit, SIGNAL(editingFinished()),
        this, SLOT(onToolEditingFinished())
    );
}

void ToolFormController::setFormActive(bool)
{

}

void ToolFormController::initialise()
{

}

void ToolFormController::onToolListSelectionChanged
(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(deselected)

    if (mSelectedToolHandle)
    {
        mSelectedToolHandle->disconnect();
    }

    if (selected.count() == 0)
    {
        mSelectedToolHandle = nullptr;
        return;
    }

    int selectedRow = selected.indexes().first().row();

    qDebug() << "ToolModelFormController: Selection Changed to row"
             << selectedRow;

    mSelectedToolHandle = mListModelHandle->getData(selectedRow);

    qDebug() << "ToolModelFormController: Selected ID "
             << mSelectedToolHandle->getID();

    toolModelSelectionValid(mSelectedToolHandle->isIdValid());
    mUi.toolsRemoveButton->setEnabled(mSelectedToolHandle->isIdValid());
}

void ToolFormController::onToolEditingFinished()
{
    if (mSelectedToolHandle->isIdValid())
    {
        mListModelHandle->setData
        (
            mListModelHandle->indexOf(mSelectedToolHandle),
            mUi.nameLineEdit->text(),
            Qt::EditRole
        );
    }
}

void ToolFormController::onAddToolModelButtonClicked()
{
   qDebug() << "ToolModelFormController::onAddToolModelButtonClicked()";
   mListModelHandle->insert(QSharedPointer<Tool>::create(-1,DEFAULT_NAME));
}

void ToolFormController::onRemoveToolModelButtonClicked()
{
   qDebug() << "ToolModelFormController::onRemoveToolModelButtonClicked()";
   if (!mSelectedToolHandle->isIdValid())
   {
       return;
   }

   int result = QMessageBox::warning
   (
        this,
        "Are you sure?",
        QString("Are you sure?\n\nAny tools using this Tool  will need to be reassigned\n\nDelete: '%1'?")
            .arg(mSelectedToolHandle->getName()),
        QMessageBox::Ok | QMessageBox::Cancel
   );

   switch (result)
   {
       case QMessageBox::Ok:
           mListModelHandle->remove(mSelectedToolHandle);
           mSelectedToolHandle = nullptr;
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
   mSelectedToolHandle->addNewGeometryRow();
}

void ToolFormController::onRemoveToolModelGeometryButtonClicked()
{
   qDebug() << "ToolModelFormController::onRemoveToolModelGeometryButtonClicked()";
   mSelectedToolHandle->removeSelectedGeometryRow();
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
       mUi.nameLineEdit->setText(mSelectedToolHandle->getName());

       if (mUi.geometryTable->model())
       {
           mUi.geometryTable->model()->disconnect();
       }

       if (mUi.geometryTable->selectionModel())
       {
           mUi.geometryTable->selectionModel()->disconnect();
       }

       ToolGeometryTableModel* tableModel = mSelectedToolHandle->getGeometryTableModelHandle();

       if (tableModel != nullptr)
       {
           qDebug() << "ToolModelFormController: tableModel is valid";

           mUi.geometryTable->setModel(tableModel);
           mUi.geometryTable->setEditTriggers(QAbstractItemView::DoubleClicked);

           // Geometry Table CRUD
           connect
           (
                tableModel,
                SIGNAL(toolGeometryCreatedSignal(ToolGeometry*)),
                this,
                SLOT(onToolGeometryCreated(ToolGeometry*))
           );
           connect
           (
                tableModel,
                SIGNAL(toolGeometryUpdatedSignal(ToolGeometry*)),
                this,
                SLOT(onToolGeometryUpdated(ToolGeometry*))
           );
           connect
           (
                tableModel,
                SIGNAL(toolGeometryDeletedSignal(ToolGeometry*)),
                this,
                SLOT(onToolGeometryDeleted(ToolGeometry*))
           );
           // Geometry Table Selection
           connect
           (
               mUi.geometryTable->selectionModel(),
               SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
               this,
               SLOT(onToolGeometrySelectionChanged(const QItemSelection&, const QItemSelection&))
           );
       }
       else
       {
           qDebug() << "ToolModelFormController: tableModel is nullptr";
       }
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
        mSelectedToolHandle->setSelectedGeometryRow(selected.indexes().first().row());
    }
}

const QString ToolFormController::DEFAULT_NAME = "New Tool";
