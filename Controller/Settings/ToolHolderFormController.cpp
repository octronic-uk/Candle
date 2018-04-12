/*
 * ToolHolderForm.cpp
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

#include "Model/Settings/Sql/SqlSettingsModel.h"
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

void ToolHolderFormController::onProfileChanged(Profile* profile)
{
    qDebug() << "ToolHolderFormController: onProfileChanged";
    mUi.toolHoldersListView->setModel(profile->getToolHolderListModelHandle());
    // Tool Model Selection Changed
    connect
    (
        mUi.toolHoldersListView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this,
        SLOT(onToolHolderListSelectionChanged(const QItemSelection&, const QItemSelection&))
    );
}



void ToolHolderFormController::setupSignalSlots()
{
    // Add tool holder
    connect
    (
        mUi.toolHoldersAddButton, SIGNAL(clicked()),
        this, SLOT(onAddToolHolderButtonClicked())
    );
    // Remove tool holder
    connect
    (
        mUi.toolHoldersRemoveButton, SIGNAL(clicked()),
        this, SLOT(onRemoveToolHolderButtonClicked())
    );
    // Add tool holder geometry
    connect
    (
        mUi.geometryAddButton, SIGNAL(clicked()),
        this, SLOT(onAddToolHolderGeometryButtonClicked())
    );
    // Remove tool holder geometry
    connect
    (
        mUi.geometryRemoveButton, SIGNAL(clicked()),
        this, SLOT(onRemoveToolHolderGeometryButtonClicked())
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

void ToolHolderFormController::setSettingsModel(SqlSettingsModel* handle)
{
    mSettingsModelHandle = handle;
}

void ToolHolderFormController::onToolHolderListSelectionChanged
(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(deselected)
    qDebug() << "ToolHolderFormController: onToolHolderListSelectionChanged"
             << selected
             << deselected;

    ToolHolderListModel* model = mSettingsModelHandle
                            ->getCurrentProfileHandle()
                            ->getToolHolderListModelHandle();


    /*if (model)
    {
        ToolHolder* handle = model->getSelected();

        if (handle)
        {
            handle->disconnect();
        }
    }*/

    if (selected.count() == 0)
    {
        return;
    }

    int selectedRow = selected.indexes().first().row();

    qDebug() << "ToolHolderFormController: Selection Changed to row"
             << selectedRow;

    ToolHolder* newlySelected = model->getData(selectedRow);
    mSettingsModelHandle
        ->getCurrentProfileHandle()
        ->getToolHolderListModelHandle()
        ->setSelected(newlySelected);

    qDebug() << "ToolHolderFormController: Selected ID "
             << newlySelected->getID();

    toolHolderModelSelectionValid(newlySelected->isIdValid());
    mUi.toolHoldersRemoveButton->setEnabled(newlySelected->isIdValid());
}

void ToolHolderFormController::onToolHolderEditingFinished()
{
    if (mSettingsModelHandle)
    {
        ToolHolderListModel *model = mSettingsModelHandle
            ->getCurrentProfileHandle()
            ->getToolHolderListModelHandle();
        ToolHolder *toolHolder = model->getSelected();
        toolHolder->setName(mUi.nameLineEdit->text());
        mSettingsModelHandle->onToolHolderUpdated(toolHolder);
    }
}

void ToolHolderFormController::onGeometryUpdated(ToolHolderGeometry* geom)
{
    if (mSettingsModelHandle)
    {
        mSettingsModelHandle->onToolHolderGeometryUpdated(geom);
    }
}

void ToolHolderFormController::onAddToolHolderButtonClicked()
{
    qDebug() << "ToolHolderFormController::onAddToolHolderButtonClicked()";
    Profile* profile = mSettingsModelHandle->getCurrentProfileHandle();
    if (profile)
    {
        mSettingsModelHandle->onToolHolderCreated();
    }
}

void ToolHolderFormController::onRemoveToolHolderButtonClicked()
{
   qDebug() << "ToolHolderFormController::onRemoveToolHolderButtonClicked()";
   if (mSettingsModelHandle)
   {
       ToolHolderListModel * model = mSettingsModelHandle
            ->getCurrentProfileHandle()
            ->getToolHolderListModelHandle();

       ToolHolder *selected = model->getSelected();
       if (selected)
       {
           int result = QMessageBox::warning
           (
                this,
                "Are you sure?",
                QString("Are you sure?\n\nAny tools using this Tool Holder will need to be reassigned\n\nDelete: '%1'?")
                    .arg(selected->getName()),
                QMessageBox::Ok | QMessageBox::Cancel
           );

           switch (result)
           {
               case QMessageBox::Ok:
                   mSettingsModelHandle->onToolHolderDeleted(selected);
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
   }
}

void ToolHolderFormController::onAddToolHolderGeometryButtonClicked()
{
    qDebug() << "ToolHolderFormController::onAddTooHolderGeometryButtonClicked()";
    if (mSettingsModelHandle)
    {
        mSettingsModelHandle->onToolHolderGeometryCreated();
    }
}

void ToolHolderFormController::onRemoveToolHolderGeometryButtonClicked()
{
    qDebug() << "ToolHolderFormController::onRemoveToolHolderGeometryButtonClicked()";
    if (mSettingsModelHandle)
    {
        ToolHolderGeometry *selected = mSettingsModelHandle
            ->getCurrentProfileHandle()
            ->getToolHolderListModelHandle()
            ->getSelected()
            ->getSelectedGeometryHandle();

        if (selected)
        {
            mSettingsModelHandle->onToolHolderGeometryDeleted(selected);
        }
    }
}

void ToolHolderFormController::toolHolderModelSelectionValid(bool isValid)
{
    mUi.geometryAddButton->setEnabled(isValid);
    mUi.geometryRemoveButton->setEnabled(isValid);
    mUi.geometryTable->setEnabled(isValid);
    mUi.nameLineEdit->setEnabled(isValid);
    mUi.nameLabel->setEnabled(isValid);
    mUi.geometryLabel->setEnabled(isValid);

    if (!mSettingsModelHandle)
    {
        return;
    }

    Profile *profile = mSettingsModelHandle->getCurrentProfileHandle();
    if (profile)
    {
        ToolHolder *selected = profile->getToolHolderListModelHandle()->getSelected();

        if (selected)
        {

            if (isValid)
            {
               mUi.nameLineEdit->setText(selected->getName());

               if (mUi.geometryTable->model())
               {
                   mUi.geometryTable->model()->disconnect();
               }

               if (mUi.geometryTable->selectionModel())
               {
                   mUi.geometryTable->selectionModel()->disconnect();
               }

               ToolHolderGeometryTableModel* tableModel = selected->getGeometryTableModelHandle();

               if (tableModel != nullptr)
               {
                   qDebug() << "ToolHolderFormController: tableModel is valid";

                   mUi.geometryTable->setModel(tableModel);
                   mUi.geometryTable->setEditTriggers(QAbstractItemView::DoubleClicked);

                   // Geometry Table CRUD
                   connect
                   (
                        tableModel,
                        SIGNAL(geometryUpdatedSignal(ToolHolderGeometry*)),
                        this,
                        SLOT(onGeometryUpdated(ToolHolderGeometry*))
                   );
                   // Geometry Table Selection
                   connect
                   (
                       mUi.geometryTable->selectionModel(),
                       SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                       this,
                       SLOT(onGeometrySelectionChanged(const QItemSelection&, const QItemSelection&))
                   );
               }
               else
               {
                   qDebug() << "ToolHolderFormController: tableModel is nullptr";
               }
            }
        }
    }
}

void ToolHolderFormController::onGeometrySelectionChanged
(const QItemSelection &selected, const QItemSelection &deselected)
{
    qDebug() << "ToolHolderFormController::onToolHolderGeometrySelectionChanged";
    Q_UNUSED(deselected)
    if(selected.indexes().count() > 0)
    {
        qDebug() << "ToolHolderFormController: row valid";
        ToolHolder *selectedHandle = mSettingsModelHandle
         ->getCurrentProfileHandle()
         ->getToolHolderListModelHandle()
         ->getSelected();
        selectedHandle->setSelectedGeometryRow(selected.indexes().first().row());
    }
}
