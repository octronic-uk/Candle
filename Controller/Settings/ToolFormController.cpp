/*
 * ToolFormController.cpp
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
#include "Model/Settings/Sql/SqlSettingsModel.h"
#include <QtDebug>
#include <QMessageBox>

ToolFormController::ToolFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    mUi.setupUi(this);
    mUi.splitter->setStretchFactor(0,1);
    mUi.splitter->setStretchFactor(1,2);
    toolSelectionValid(false);
    mUi.geometryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setupSignalSlots();
}

ToolFormController::~ToolFormController() {}

void ToolFormController::onProfileChanged
(Profile* profile)
{
    qDebug() << "ToolFormController: onProfileChanged";
    mUi.toolsListView->setModel(profile->getToolListModelHandle());
    mUi.toolHolderComboBox->setModel(profile->getToolHolderListModelHandle());
    // Tool Model Selection Changed
    connect
    (
        mUi.toolsListView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this,
        SLOT(onToolListSelectionChanged(const QItemSelection&, const QItemSelection&))
    );
}

void ToolFormController::onToolHolderComboBoxIndexChanged(int currentIndex)
{
    qDebug() << "ToolFormController: tool holder combobox changed"
             << currentIndex;
    if (!mSettingsModelHandle)
        return;

    Profile* profile = mSettingsModelHandle->getCurrentProfileHandle();
    if (!profile)
        return;

    ToolListModel* toolModel = profile->getToolListModelHandle();
    if (!toolModel)
        return;

    ToolHolderListModel* toolHolderModel = profile->getToolHolderListModelHandle();
    Tool* selected = toolModel->getSelected();

    if (toolHolderModel && selected)
    {
        ToolHolder* holder = toolHolderModel->getData(currentIndex);
        if (holder)
        {
            selected->setToolHolderID(holder->getID());
            mSettingsModelHandle->onToolUpdated(selected);
        }
    }
}

void ToolFormController::onToolNumberSpinBoxValueChanged(int value)
{
    Profile* profile = mSettingsModelHandle->getCurrentProfileHandle();
    if (profile)
    {
        ToolListModel* toolModel = profile->getToolListModelHandle();
        if (toolModel)
        {
            Tool* selected = toolModel->getSelected();
            if (selected)
            {
                selected->setToolNumber(value);
                mSettingsModelHandle->onToolUpdated(selected);
            }
        }
    }
}

void ToolFormController::setupSignalSlots()
{
    // Add tool holder
    connect
    (
        mUi.toolsAddButton, SIGNAL(clicked()),
        this, SLOT(onAddToolButtonClicked())
    );
    // Remove tool holder
    connect
    (
        mUi.toolsRemoveButton, SIGNAL(clicked()),
        this, SLOT(onRemoveToolButtonClicked())
    );
    // Add tool holder geometry
    connect
    (
        mUi.geometryAddButton, SIGNAL(clicked()),
        this, SLOT(onAddToolGeometryButtonClicked())
    );
    // Remove tool holder geometry
    connect
    (
        mUi.geometryRemoveButton, SIGNAL(clicked()),
        this, SLOT(onRemoveToolGeometryButtonClicked())
    );
    // Tool  Name
    connect
    (
        mUi.nameLineEdit, SIGNAL(editingFinished()),
        this, SLOT(onToolNameEditingFinished())
    );
    // Tool Holder Combo Changed
    connect
    (
        mUi.toolHolderComboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(onToolHolderComboBoxIndexChanged(int))
    );
    // Tool number spinbox
    connect
    (
        mUi.toolNumberSpinBox, SIGNAL(valueChanged(int)),
        this, SLOT(onToolNumberSpinBoxValueChanged(int))
    );
}

void ToolFormController::setFormActive(bool)
{

}

void ToolFormController::initialise()
{

}

void ToolFormController::setSettingsModel(SqlSettingsModel* handle)
{
    mSettingsModelHandle = handle;
}

void ToolFormController::onToolListSelectionChanged
(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(deselected)

    Profile* profile = mSettingsModelHandle->getCurrentProfileHandle();
    ToolListModel* toolModel = profile->getToolListModelHandle();

    if (toolModel->getSelected())
    {
        toolModel->getSelected()->disconnect();
    }

    if (selected.count() == 0)
    {
        toolModel->setSelectedToolHandle(nullptr);
        return;
    }

    int selectedRow = selected.indexes().first().row();

    qDebug() << "ToolFormController: Selection Changed to row"
             << selectedRow;

    toolModel->setSelectedToolHandle(toolModel->getData(selectedRow));

    qDebug() << "ToolFormController: Selected ID "
             << toolModel->getSelected()->getID();

    toolSelectionValid(toolModel->getSelected()->isIdValid());
    mUi.toolsRemoveButton->setEnabled(toolModel->getSelected()->isIdValid());
}

void ToolFormController::onToolNameEditingFinished()
{
    Profile* profile = mSettingsModelHandle->getCurrentProfileHandle();
    ToolListModel* toolModel = profile->getToolListModelHandle();
    Tool* selected = toolModel->getSelected();

    if (selected)
    {
        selected->setName(mUi.nameLineEdit->text());
        mSettingsModelHandle->onToolUpdated(selected);
    }
}

void ToolFormController::onAddToolButtonClicked()
{
   qDebug() << "ToolModelFormController::onAddToolButtonClicked()";
   Profile* profile = mSettingsModelHandle->getCurrentProfileHandle();
   if (profile)
   {
       mSettingsModelHandle->onToolCreated();
   }
}

void ToolFormController::onRemoveToolButtonClicked()
{
   qDebug() << "ToolHolderFormController::onRemoveToolButtonClicked()";
   if (mSettingsModelHandle)
   {
       ToolListModel * model = mSettingsModelHandle
            ->getCurrentProfileHandle()
            ->getToolListModelHandle();

       Tool *selected = model->getSelected();
       if (selected)
       {
           int result = QMessageBox::warning
           (
                this,
                "Are you sure?",
                QString("Are you sure?\n\nAny tools using this Tool?\n\nDelete: '%1'?")
                    .arg(selected->getName()),
                QMessageBox::Ok | QMessageBox::Cancel
           );

           switch (result)
           {
               case QMessageBox::Ok:
                   mSettingsModelHandle->onToolDeleted(selected);
                   model->setSelectedToolHandle(nullptr);
                   mUi.nameLineEdit->setText("");
                   mUi.geometryTable->setModel(nullptr);
                   mUi.geometryTable->setEnabled(false);
                   mUi.nameLineEdit->setEnabled(false);
                   mUi.nameLabel->setEnabled(false);
                   mUi.geometryAddButton->setEnabled(false);
                   mUi.geometryRemoveButton->setEnabled(false);
                   mUi.geometryLabel->setEnabled(false);
                   mUi.toolNumberLabel->setEnabled(false);
                   mUi.toolNumberSpinBox->setEnabled(false);
                   mUi.toolHolderComboBox->setEnabled(false);
                   mUi.toolHolderLabel->setEnabled(false);
                   mUi.toolsListView->clearSelection();
                   break;
               case QMessageBox::Cancel:
                   break;
           }
       }
   }
}

void ToolFormController::onAddToolGeometryButtonClicked()
{
   qDebug() << "ToolFormController::onAddToolGeometryButtonClicked()";
   if (mSettingsModelHandle)
   {
       mSettingsModelHandle->onToolGeometryCreated();
   }
}

void ToolFormController::onRemoveToolGeometryButtonClicked()
{
    qDebug() << "ToolFormController::onRemoveToolGeometryButtonClicked()";
     if (mSettingsModelHandle)
    {
        ToolGeometry *selected = mSettingsModelHandle
            ->getCurrentProfileHandle()
            ->getToolListModelHandle()
            ->getSelected()
            ->getSelectedGeometryHandle();

        if (selected)
        {
            mSettingsModelHandle->onToolGeometryDeleted(selected);
        }
    }
}

void ToolFormController::toolSelectionValid(bool isValid)
{
    mUi.geometryAddButton->setEnabled(isValid);
    mUi.geometryRemoveButton->setEnabled(isValid);
    mUi.geometryTable->setEnabled(isValid);
    mUi.nameLineEdit->setEnabled(isValid);
    mUi.nameLabel->setEnabled(isValid);
    mUi.geometryLabel->setEnabled(isValid);
    mUi.toolNumberLabel->setEnabled(isValid);
    mUi.toolNumberSpinBox->setEnabled(isValid);
    mUi.toolHolderLabel->setEnabled(isValid);
    mUi.toolHolderComboBox->setEnabled(isValid);

    if (!mSettingsModelHandle)
    {
        return;
    }

    Profile* profile = mSettingsModelHandle->getCurrentProfileHandle();

    if (!profile)
    {
        return;
    }

    ToolListModel* toolModel = profile->getToolListModelHandle();
    ToolHolderListModel* toolHolderModel = profile->getToolHolderListModelHandle();

    if (isValid)
    {
        mUi.nameLineEdit->setText(toolModel->getSelected()->getName());
        mUi.toolNumberSpinBox->setValue(toolModel->getSelected()->getToolNumber());

        if (toolHolderModel)
        {
            QModelIndex index = toolHolderModel->getIndexByID
                (toolModel->getSelected()->getToolHolderID());
            mUi.toolHolderComboBox->setCurrentIndex(index.row());
        }

        if (mUi.geometryTable->model())
        {
            mUi.geometryTable->model()->disconnect();
        }

        if (mUi.geometryTable->selectionModel())
        {
            mUi.geometryTable->selectionModel()->disconnect();
        }

        ToolGeometryTableModel* tableModel = toolModel
            ->getSelected()->getGeometryTableModelHandle();

        if (tableModel != nullptr)
        {
           qDebug() << "ToolFormController: tableModel is valid";

           mUi.geometryTable->setModel(tableModel);
           mUi.geometryTable->setEditTriggers(QAbstractItemView::DoubleClicked);

           // Geometry Table CRUD
           connect
           (
                tableModel,
                SIGNAL(geometryUpdatedSignal(ToolGeometry*)),
                this,
                SLOT(onGeometryUpdated(ToolGeometry*))
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
           qDebug() << "ToolFormController: tableModel is nullptr";
       }
    }
}

void ToolFormController::onGeometrySelectionChanged
(const QItemSelection &selected, const QItemSelection &deselected)
{
    qDebug() << "ToolFormController::onToolGeometrySelectionChanged";
    Q_UNUSED(deselected)
    if(selected.indexes().count() > 0)
    {
        qDebug() << "ToolFormController: row valid";
        Tool *selectedHandle = mSettingsModelHandle
         ->getCurrentProfileHandle()
         ->getToolListModelHandle()
         ->getSelected();
        selectedHandle->setSelectedGeometryRow(selected.indexes().first().row());
    }
}

void ToolFormController::onGeometryUpdated(ToolGeometry* toolGeometry)
{
    if (mSettingsModelHandle)
    {
        mSettingsModelHandle->onToolGeometryUpdated(toolGeometry);
    }
}
