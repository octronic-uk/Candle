/*
 * ProgramFormController.cpp
 *
 * Created: 22 2018 by Ashley
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

#include <QMenu>
#include <QShortcut>
#include <QMessageBox>
#include <QScrollBar>
#include "Model/Gcode/Parser/GcodeViewParser.h"
#include "Model/Gcode/GcodeFileModel.h"

#include "ProgramFormController.h"

using namespace Ui;

ProgramFormController::ProgramFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "ProgramFormController: Contructing";
    mUi.setupUi(this);
    setupSignalSlots();
    setFormActive(false);
}

ProgramFormController::~ProgramFormController()
{
    qDebug() << "ProgramFormController: Destructing";
}

void ProgramFormController::setupProgramTable()
{

    mUi.programTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Table & Model
    mUi.programTable->setModel(&mProgramTableModel);
    connect(
        mUi.programTable->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
        this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex))
    );
    mUi.programTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    mUi.programTable->installEventFilter(this);

    connect
    (
        &mProgramTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
        this, SLOT(onTableCellChanged(QModelIndex,QModelIndex))
    );

    connect(
        mUi.programTable->verticalScrollBar(), SIGNAL(actionTriggered(int)),
        this, SLOT(onScrollBarAction(int))
    );

    // Setup Menu
    QShortcut *insertShortcut = new QShortcut(QKeySequence(Qt::Key_Insert), mUi.programTable);
    connect(insertShortcut, SIGNAL(activated()), this, SLOT(onTableInsertLine()));
    mTableMenu.addAction(tr("&Insert line"), this, SLOT(onTableInsertLine()), insertShortcut->key());

    QShortcut *deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), mUi.programTable);
    connect(deleteShortcut, SIGNAL(activated()), this, SLOT(onTableDeleteLines()));
    mTableMenu.addAction(tr("&Delete lines"), this, SLOT(onTableDeleteLines()), deleteShortcut->key());

    connect(
        mUi.programTable,SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(onTableProgramCustomContextMenuRequested(const QPoint&))
    );

    mUi.programTable->setContextMenuPolicy(Qt::CustomContextMenu);


}

void ProgramFormController::setupSendMenu()
{
    // TODO - This was using ConsoleFormController.mSendMenu
    mUi.sendButton->setMenu(&mSendMenu);

    mSendAction.setText("Send");
    mSendFromCurrentLineAction.setText("Send from Current Line");

    mSendMenu.addAction(&mSendAction);
    mSendMenu.addAction(&mSendFromCurrentLineAction);

    connect(
        &mSendAction, SIGNAL(triggered()),
        this, SLOT(onSendActionTriggered())
    );

    connect(
        &mSendFromCurrentLineAction, SIGNAL(triggered()),
        this, SLOT(onSendFromCurrentLineActionTriggered())
    );
}

bool ProgramFormController::isAutoScrollChecked()
{
    return mUi.autoScrollButton->isChecked();
}

void ProgramFormController::setAutoScrollChecked(bool checked)
{
    mUi.autoScrollButton->setChecked(checked);
}

void ProgramFormController::setFormActive(bool active)
{
    mUi.sendButton->setEnabled(active);
    mUi.testModeButton->setEnabled(active);
    mUi.autoScrollButton->setEnabled(active);
}

void ProgramFormController::initialise()
{
    mProgramTableModel.clear();
    mMarkerListModel.clear();
    setFormActive(false);
}

void ProgramFormController::onReserveGcodeRowsSignal(int rows)
{
    //qDebug() << "ProgramFormController: Reserving " << rows << " rows";
    mProgramTableModel.data().reserve(rows);
}

void ProgramFormController::onTableProgramCustomContextMenuRequested(const QPoint &pos)
{
    //qDebug() << "ProgramFormController: onTableProgramCustomContextMenuRequested(const QPoint &pos)";
    if (mProcessingFile)
    {
        return;
    }

    if (mUi.programTable->selectionModel()->selectedRows().count() > 0)
    {
        mTableMenu.actions().at(0)->setEnabled(true);
        mTableMenu.actions().at(1)->setEnabled(
            mUi.programTable->selectionModel()->selectedRows()[0].row() !=
            mProgramTableModel.rowCount() - 1
        );
    }
    else
    {
        mTableMenu.actions().at(0)->setEnabled(false);
        mTableMenu.actions().at(1)->setEnabled(false);
    }
    mTableMenu.popup(mUi.programTable->viewport()->mapToGlobal(pos));
}

void ProgramFormController::onTableCellChanged(QModelIndex i1, QModelIndex i2)
{

}

void ProgramFormController::onTableCurrentChanged(QModelIndex i1, QModelIndex i2)
{

}

void ProgramFormController::onTableInsertLine()
{
    //qDebug() << "ProgramFormController: onTableInsertLine";
    if (mUi.programTable->selectionModel()->selectedRows().count() == 0 )
    {
        return;
    }

    int row = mUi.programTable->selectionModel()->selectedRows()[0].row();

    mProgramTableModel.insertRow(row);
    mProgramTableModel.setData(mProgramTableModel.index(row, 2), InQueue);

    //updateParser();
    mCellChanged = true;
    mUi.programTable->selectRow(row);
}

void ProgramFormController::onTableDeleteLines()
{
    //qDebug() << "ProgramFormController: onTableDeleteLine";
    if (
        mUi.programTable->selectionModel()->selectedRows().count() == 0 ||
        QMessageBox::warning(
            this,
            this->windowTitle(),
            tr("Delete lines?"),
            QMessageBox::Yes | QMessageBox::No
        ) == QMessageBox::No
    ) {
        return;
    }

    QModelIndex firstRow = mUi.programTable->selectionModel()->selectedRows()[0];
    int rowsCount = mUi.programTable->selectionModel()->selectedRows().count();

    if (mUi.programTable->selectionModel()->selectedRows().last().row() == mProgramTableModel.rowCount() - 1)
    {
        rowsCount--;
    }

    //qDebug() << "ProgramFormController: Deleting lines" << firstRow.row() << rowsCount;

    if (firstRow.row() != mProgramTableModel.rowCount() - 1)
    {
        mProgramTableModel.removeRows(firstRow.row(), rowsCount);
    }
    else
    {
        return;
    }

    //updateParser();
    mCellChanged = true;
    mUi.programTable->selectRow(firstRow.row());
}

void ProgramFormController::clearTable()
{
    //qDebug() << "ProgramFormController: clearTable";
    mProgramTableModel.clear();
}

void ProgramFormController::setupSignalSlots()
{
    //qDebug() << "ProgramFormController: Setup Signals/Slots";
    setupSendMenu();
    setupProgramTable();
    mUi.markerListView->setModel(&mMarkerListModel);
    //mUi.markerListView->setMinimumWidth(mUi.markerListView->sizeHintForColumn(0));

    connect(
        mUi.testModeButton, SIGNAL(toggled(bool)),
        this, SLOT(onTestModeButtonClicked(bool))
    );

    connect
    (
        mUi.markerListView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
        this,
        SLOT(onMarkerSelectionChanged(const QItemSelection&, const QItemSelection&))
    );
}

void ProgramFormController::onMarkerSelectionChanged
(const QItemSelection& selected, const QItemSelection& deselected)
{
    /*qDebug() << "ProgramFormController: MarkerSelectionChanged"
             << selected << deselected;
             */
    Q_UNUSED(deselected)
    GcodeCommand *marker = mMarkerListModel.at(selected.indexes().first().row());
    if (marker)
    {
        int index = marker->getTableIndex();

        if (index > mUi.programTable->model()->rowCount()-1)
        {
            index = mUi.programTable->model()->rowCount()-1;
        }

        /*qDebug() << "ProgramFormController: Scrolling to marker"
                 << index;
                 */

        mUi.programTable->selectRow(index);
    }
}


void ProgramFormController::onGcodeFileLoadFinished(GcodeFileModel* fileModel)
{
    //qDebug() << "ProgramFormController: onGcodeFileLoadFinished";
    mProgramTableModel.insertRows(0,fileModel->getData().count(),QModelIndex());
    mProgramTableModel.setCommandData(fileModel->getData());
    mMarkerListModel.setMarkers(fileModel->getMarkers());
}

void ProgramFormController::onSendActionTriggered()
{
    //qDebug() << "ProgramFormController: onSendActionTriggered";

    if (mProgramTableModel.rowCount() == 0)
    {
        return;
    }
    emit setKeyboardControlSignal(false);
    emit sendProgramSignal();
}

void ProgramFormController::onSendFromCurrentLineActionTriggered()
{
    //qDebug() << "ProgramFormController: onActSendFormLineTriggered";

    if (mProgramTableModel.rowCount() == 0)
    {
        return;
    }

    //Line to start from
    int row = mUi.programTable->currentIndex().row();
    if (row >= 0 && row <= mProgramTableModel.data().count())
    {
        GcodeCommand* startingAt = mProgramTableModel.data().at(row);
        emit sendProgramFromLineSignal(startingAt->getID());
        setAutoScrollChecked(true);
    }
}

void ProgramFormController::onUpdateProgramTableStatus(GcodeCommand* command)
{
    /*qDebug() << "ProgramFormController: onUpdateProgramTableStatus"
             << "C" << command->getCommand()
             << "L" << command->getLine()
             << "I" << command->getTableIndex()
             << "RData" << command->getResponse().getData();
    */

    QModelIndex stateIndex = mProgramTableModel.index(command->getTableIndex(),1);
    QModelIndex responseIndex = mProgramTableModel.index(command->getTableIndex(),2);

    int fewMoreIndex =
        mUi.programTable->indexAt(mUi.programTable->rect().bottomRight()).row()
        -
        mUi.programTable->indexAt(mUi.programTable->rect().topLeft()).row();

    fewMoreIndex /= 2;

    QModelIndex fewMore = mProgramTableModel.index(command->getTableIndex() + fewMoreIndex,1);

    mProgramTableModel.setData(stateIndex, QVariant::fromValue(command->getState()));
    mProgramTableModel.setData(responseIndex, QVariant::fromValue(command->getResponse()));

    if (isAutoScrollChecked())
    {
        if (command->getTableIndex() >= 0)
        {
            mUi.programTable->scrollTo(fewMore);
        }
        mUi.programTable->selectRow(stateIndex.row());
    }
}

void ProgramFormController::onTestModeButtonClicked(bool checked)
{
    Q_UNUSED(checked)
    //qDebug() << "ProgramFormController: onChkBoxTestClicked";
}

void ProgramFormController::onScrollBarAction(int action)
{
    Q_UNUSED(action)
    mUi.autoScrollButton->setChecked(false);
}

void ProgramFormController::onGcodeFileLoadStarted()
{
    //qDebug() << "ProgramFormController: onGcodeFileLoadStarted";
    clearTable();
}

void ProgramFormController::onTestModeButtonChecked(bool checked)
{
    // TODO
}
