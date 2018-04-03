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
#include "Model/Parser/GcodeViewParse.h"

#include "ProgramFormController.h"
#include "ProgramFormController.h"

using namespace Ui;

ProgramFormController::ProgramFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "ProgramFormController: Contructing";
    mUi.setupUi(this);
    setupSignalSlots();

}

ProgramFormController::~ProgramFormController()
{
    qDebug() << "ProgramFormController: Destructing";
}

void ProgramFormController::setupProgramTable()
{
    // Table & Model
    mUi.programTable->setModel(&mProgramTableModel);
    connect(
        mUi.programTable->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
        this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex))
    );
    mUi.programTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
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

bool ProgramFormController::isPauseChecked()
{
    return mUi.pauseButton->isChecked();
}

void ProgramFormController::setPauseChecked(bool checked)
{
    mUi.pauseButton->setChecked(checked);
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

}

void ProgramFormController::onReserveGcodeRowsSignal(int rows)
{
    qDebug() << "ProgramFormController: Reserving " << rows << " rows";
    mProgramTableModel.data().reserve(rows);
}

void ProgramFormController::onTableProgramCustomContextMenuRequested(const QPoint &pos)
{
    qDebug() << "ProgramFormController: onTableProgramCustomContextMenuRequested(const QPoint &pos)";
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
    qDebug() << "ProgramFormController: Table Cell Changed" << i1 << i2;
    Q_UNUSED(i2)

    GcodeTableModel *model = static_cast<GcodeTableModel*>(sender());

    if (i1.column() != 0)
    {
        qDebug() << "ProgramFormController: Not an editable column??";
        return;
    }

    // Inserting new line at end
    if (i1.row() == (model->rowCount() - 1) && model->data(model->index(i1.row(), 1)).toString() != "")
    {
        model->setData(model->index(model->rowCount() - 1, 2), InQueue);
        model->insertRow(model->rowCount());

        if (!mProgramLoading)
        {
            mUi.programTable->setCurrentIndex(model->index(i1.row() + 1, 1));
        }
    }
    // Remove last line
    else if (i1.row() != (model->rowCount() - 1) && model->data(model->index(i1.row(), 1)).toString() == "")
    {
        mUi.programTable->setCurrentIndex(model->index(i1.row() + 1, 1));
        mProgramTableModel.removeRow(i1.row());
    }

    if (!mProgramLoading)
    {
        // Clear cached args
        model->setData(model->index(i1.row(), 5), QVariant());

        // Update visualizer
        //updateParser();
    }
}

void ProgramFormController::onTableCurrentChanged(QModelIndex i1, QModelIndex i2)
{
    qDebug() << "ProgramFormController: TableCurrentChanged" << i1 << i2;
    // Update toolpath hightlighting
    /*
    if (idx1.row() > mProgramTableModel.rowCount() - 2)
    {
        idx1 = mProgramTableModel.index(mProgramTableModel.rowCount() - 2, 0);
    }
    if (idx2.row() > mProgramTableModel.rowCount() - 2)
    {
        idx2 = mProgramTableModel.index(mProgramTableModel.rowCount() - 2, 0);
    }

    GcodeViewParse *parser = mCurrentDrawer->viewParser();
    QList<LineSegment*> list = parser->getLineSegmentList();
    QVector<QList<int>> lineIndexes = parser->getLinesIndexes();

    // Update linesegments on cell changed
    if (!mCurrentDrawer->geometryUpdated())
    {
        for (int i = 0; i < list.count(); i++)
        {
            list.at(i)->setIsHightlight(list.at(i)->getLineNumber() <= mProgramTableModel.data(mProgramTableModel.index(idx1.row(), 4)).toInt());
        }
    // Update vertices on current cell changed
    }
    else
    {
        int lineFirst = mProgramTableModel.data(mProgramTableModel.index(idx1.row(), 4)).toInt();
        int lineLast = mProgramTableModel.data(mProgramTableModel.index(idx2.row(), 4)).toInt();
        if (lineLast < lineFirst) qSwap(lineLast, lineFirst);
//        qDebug() << "table current changed" << idx1.row() << idx2.row() << lineFirst << lineLast;

        QList<int> indexes;
        for (int i = lineFirst + 1; i <= lineLast; i++)
        {
            foreach (int l, lineIndexes.at(i))
            {
                list.at(l)->setIsHightlight(idx1.row() > idx2.row());
                indexes.append(l);
            }
        }

        mSelectionDrawer.setEndPosition(indexes.isEmpty() ?
            QVector3D(sNan, sNan, sNan) :
            (
                mCodeDrawer->getIgnoreZ() ?
                    QVector3D(
                        list.at(indexes.last())->getEnd().x(),
                        list.at(indexes.last())->getEnd().y(),
                        0
                    )
                :
                list.at(indexes.last())->getEnd()
            )
        );

        mSelectionDrawer.update();

        if (!indexes.isEmpty()) mCurrentDrawer->update(indexes);
    }

    // Update selection marker
    int line = mProgramTableModel.data(mProgramTableModel.index(idx1.row(), 4)).toInt();
    if (line > 0 && !lineIndexes.at(line).isEmpty())
    {
        QVector3D pos = list.at(lineIndexes.at(line).last())->getEnd();
        mSelectionDrawer.setEndPosition(mCodeDrawer->getIgnoreZ() ? QVector3D(pos.x(), pos.y(), 0) : pos);
    }
    else
    {
        mSelectionDrawer.setEndPosition(QVector3D(sNan, sNan, sNan));
    }
    mSelectionDrawer.update();
    */
}

void ProgramFormController::onTableInsertLine()
{
    qDebug() << "ProgramFormController: onTableInsertLine";
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
    qDebug() << "ProgramFormController: onTableDeleteLine";
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

    qDebug() << "ProgramFormController: Deleting lines" << firstRow.row() << rowsCount;

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
    qDebug() << "ProgramFormController: clearTable";
    mProgramTableModel.clear();
}

void ProgramFormController::setupSignalSlots()
{
    qDebug() << "ProgramFormController: Setup Signals/Slots";
    setupSendMenu();
    setupProgramTable();

    connect(
        mUi.testModeButton, SIGNAL(toggled(bool)),
        this, SLOT(onTestModeButtonClicked(bool))
    );
}

void ProgramFormController::onGcodeFileLoadFinished(QList<GcodeCommand> items)
{
    qDebug() << "ProgramFormController: onGcodeFileLoadFinished";
    mProgramTableModel.insertRows(0,items.count(),QModelIndex());
    mProgramTableModel.append(items);
}

void ProgramFormController::onSendActionTriggered()
{
    qDebug() << "ProgramFormController: onSendActionTriggered";

    if (mProgramTableModel.rowCount() == 1)
    {
        return;
    }

    onResetButtonClicked();

    mStartTime.start();
    mTransferCompleted = false;
    mProcessingFile = true;
    mFileEndSent = false;

    emit setKeyboardControlSignal(false);

    if (!mUi.testModeButton->isChecked())
    {
        //storeOffsets(); // Allready stored on check
    }
    //storeParserState();

    //updateControlsState();
    mUi.pauseButton->setFocus();

    emit sendNextFileCommandsSignal();
}

void ProgramFormController::onSendFromCurrentLineActionTriggered()
{
    qDebug() << "ProgramFormController: onActSendFormLineTriggered";

    if (mProgramTableModel.rowCount() == 0)
    {
        return;
    }

    //Line to start from
    int commandIndex = mUi.programTable->currentIndex().row();

    /*
    // Set parser state
    if (mSettingsForm->autoLine())
    {
        GcodeViewParse *parser = mCurrentDrawer->viewParser();
        QList<LineSegment*> list = parser->getLineSegmentList();
        QVector<QList<int>> lineIndexes = parser->getLinesIndexes();

        int lineNumber = mProgramTableModel.data(mProgramTableModel.index(commandIndex, 4)).toInt();
        LineSegment* firstSegment = list.at(lineIndexes.at(lineNumber).first());
        LineSegment* lastSegment = list.at(lineIndexes.at(lineNumber).last());
        LineSegment* feedSegment = lastSegment;
        int segmentIndex = list.indexOf(feedSegment);
        while (feedSegment->isFastTraverse() && segmentIndex > 0) feedSegment = list.at(--segmentIndex);

        QStringList commands;

        commands.append(QString("M3 S%1").arg(qMax<double>(lastSegment->getSpindleSpeed(), mUi->txtSpindleSpeed->value())));

        commands.append(QString("G21 G90 G0 X%1 Y%2")
                        .arg(firstSegment->getStart().x())
                        .arg(firstSegment->getStart().y()));
        commands.append(QString("G1 Z%1 F%2")
                        .arg(firstSegment->getStart().z())
                        .arg(feedSegment->getSpeed()));

        commands.append(QString("%1 %2 %3 F%4")
                        .arg(lastSegment->isMetric() ? "G21" : "G20")
                        .arg(lastSegment->isAbsolute() ? "G90" : "G91")
                        .arg(lastSegment->isFastTraverse() ? "G0" : "G1")
                        .arg(lastSegment->isMetric() ? feedSegment->getSpeed() : feedSegment->getSpeed() / 25.4));

        if (lastSegment->isArc())
        {
            commands.append(lastSegment->plane() == PointSegment::XY ? "G17"
            : lastSegment->plane() == PointSegment::ZX ? "G18" : "G19");
        }

        QMessageBox box(this);
        box.setIcon(QMessageBox::Information);
        box.setText(tr("Following commands will be sent before selected line:\n") + commands.join('\n'));
        box.setWindowTitle(qApp->applicationDisplayName());
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.addButton(tr("Skip"), QMessageBox::DestructiveRole);

        int res = box.exec();

        if (res == QMessageBox::Cancel)
        {
            return;
        }
        else if (res == QMessageBox::Ok)
        {
            foreach (QString command, commands)
            {
                sendCommand(command, -1, mSettingsForm->showUICommands());
            }
        }
    }

    mFileCommandIndex = commandIndex;
    mFileProcessedCommandIndex = commandIndex;
    mLastDrawnLineIndex = 0;
    mProbeIndex = -1;

    QList<LineSegment*> list = mViewParser.getLineSegmentList();

    QList<int> indexes;
    for (int i = 0; i < list.count(); i++)
    {
        list[i]->setDrawn(list.at(i)->getLineNumber() < mProgramTableModel.data().at(commandIndex).line);
        indexes.append(i);
    }
    mCodeDrawer->update(indexes);

    mUi->programTable->setUpdatesEnabled(false);

    for (int i = 0; i < mProgramTableModel.data().count() - 1; i++)
    {
        mProgramTableModel.data()[i].state = i < commandIndex ? GcodeCommand::Skipped : GcodeCommand::InQueue;
        mProgramTableModel.data()[i].response = QString();
    }
    mUi->programTable->setUpdatesEnabled(true);
    mUi->glwVisualizer->setSpendTime(QTime(0, 0, 0));

    mStartTime.start();

    mTransferCompleted = false;
    mProcessingFile = true;
    mFileEndSent = false;
    mStoredKeyboardControl = mUi->chkKeyboardControl->isChecked();
    mUi->chkKeyboardControl->setChecked(false);

    if (!mUi->chkTestMode->isChecked())
    {
        storeOffsets(); // Allready stored on check
    }
    storeParserState();

    updateControlsState();
    mUi->pauseButton->setFocus();

    mFileCommandIndex = commandIndex;
    mFileProcessedCommandIndex = commandIndex;
    sendNextFileCommands();
    */
}

void ProgramFormController::onTestModeButtonClicked(bool checked)
{
    Q_UNUSED(checked)
    qDebug() << "ProgramFormController: onChkBoxTestClicked";
}

void ProgramFormController::onAbortButtonClicked()
{
    qDebug() << "ProgramFormController: onCmdFileAbortClicked";
    emit programAbortSignal();
    /*
    mAborting = true;
    if (!mUi->chkTestMode->isChecked())
    {
        mSerialPort.write("!");
    }
    else
    {
        grblReset();
    }
    */
}

void ProgramFormController::onSendButtonClicked()
{
    qDebug() << "ProgramFormController: onCmdCommandSendClicked";
}

void ProgramFormController::onPauseButtonClicked(bool checked)
{
    qDebug() << "ProgramFormController: onCmdFilePauseClicked";
    emit programPauseSignal(checked ? "!" : "~");
}

void ProgramFormController::onResetButtonClicked()
{
    qDebug() << "ProgramFormController: onCmdFileResetClicked";
    emit programResetSignal();
    /*
    mFileCommandIndex = 0;
    mFileProcessedCommandIndex = 0;
    mLastDrawnLineIndex = 0;
    mProbeIndex = -1;

    if (!mHeightMapMode) {
        QTime time;

        time.start();

        QList<LineSegment*> list = mViewParser.getLineSegmentList();

        QList<int> indexes;
        for (int i = 0; i < list.count(); i++) {
            list[i]->setDrawn(false);
            indexes.append(i);
        }
        mCodeDrawer->update(indexes);

        qDebug() << "drawn false:" << time.elapsed();

        time.start();

        mUi->programTable->setUpdatesEnabled(false);

        for (int i = 0; i < mCurrentGCodeTableModel->data().count() - 1; i++) {
            mCurrentGCodeTableModel->data()[i].state = GcodeCommand::InQueue;
            mCurrentGCodeTableModel->data()[i].response = QString();
        }
        mUi->programTable->setUpdatesEnabled(true);

        qDebug() << "table updated:" << time.elapsed();

        mUi->programTable->scrollTo(mCurrentGCodeTableModel->index(0, 0));
        mUi->programTable->clearSelection();
        mUi->programTable->selectRow(0);

        mUi->glwVisualizer->setSpendTime(QTime(0, 0, 0));
    } else {
        mUi->txtHeightMapGridX->setEnabled(true);
        mUi->txtHeightMapGridY->setEnabled(true);
        mUi->txtHeightMapGridZBottom->setEnabled(true);
        mUi->txtHeightMapGridZTop->setEnabled(true);

        delete mHeightMapInterpolationDrawer.data();
        mHeightMapInterpolationDrawer.setData(NULL);

        mHeightMapModel.clear();
        updateHeightMapGrid();
    }
    */
}

void ProgramFormController::onScrollBarAction(int action)
{
    qDebug() << "ProgramFormController: onScrollBarAction";
    Q_UNUSED(action)
    if (mProcessingFile)
    {
        mUi.autoScrollButton->setChecked(false);
    }
}

void ProgramFormController::onGcodeFileLoadStarted()
{
    qDebug() << "ProgramFormController: onGcodeFileLoadStarted";
    clearTable();
}
