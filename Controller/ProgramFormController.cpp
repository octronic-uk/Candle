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
#include "Model/Parser/GCodeViewParse.h"

#include "ProgramFormController.h"

using namespace Ui;

ProgramFormController::ProgramFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    QMessageLogger().info("Contructing ProgramFormController");
    mUi.setupUi(this);
    /*
    // TODO - This was using ConsoleFormController.mSendMenu
    // mUi.cmdFileSend->setMenu(menuSend);

    QShortcut *insertShortcut = new QShortcut(QKeySequence(Qt::Key_Insert), mUi.tblProgram);
    QShortcut *deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), mUi.tblProgram);

    connect(insertShortcut, SIGNAL(activated()), this, SLOT(onTableInsertLine()));
    connect(deleteShortcut, SIGNAL(activated()), this, SLOT(onTableDeleteLines()));

    // Setup Menu
    mTableMenu.addAction(tr("&Insert line"), this, SLOT(onTableInsertLine()), insertShortcut->key());
    mTableMenu.addAction(tr("&Delete lines"), this, SLOT(onTableDeleteLines()), deleteShortcut->key());

    // Model
    mUi.tblProgram->setModel(&mProgramTableModel);

    // A E S T H E T I C
    mUi.tblProgram->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    mUi.tblProgram->hideColumn(4);
    mUi.tblProgram->hideColumn(5);
    // mUi.tblProgram->showColumn(4);

    mCurrentGCodeTableModel = &mProgramTableModel;

    connect
    (
        &mProgramTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
        this, SLOT(onTableCellChanged(QModelIndex,QModelIndex))
    );

    /*
    connect(
        mUi.tblProgram->verticalScrollBar(), SIGNAL(actionTriggered(int)),
        this, SLOT(onScroolBarAction(int))
    );

    connect(
        mUi.tblProgram->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
        this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex))
    );

    clearTable();

    //mUi->tblProgram->installEventFilter(this);

    */
}

ProgramFormController::~ProgramFormController()
{
    QMessageLogger().info("Destructing ProgramFormController");
}

bool ProgramFormController::isPauseChecked()
{

}

void ProgramFormController::setPauseChecked(bool)
{

}

bool ProgramFormController::isAutoScrollChecked()
{

}

void ProgramFormController::setAutoScrollChecked(bool)
{

}

void ProgramFormController::onTableProgramCustomContextMenuRequested(const QPoint &pos)
{
    /*
    if (mProcessingFile) {
        return;
    }

    if (mUi->tblProgram->selectionModel()->selectedRows().count() > 0)
    {
        mTableMenu->actions().at(0)->setEnabled(true);
        mTableMenu->actions().at(1)->setEnabled(mUi->tblProgram->selectionModel()->selectedRows()[0].row() != mCurrentModel->rowCount() - 1);
    }
    else
    {
        mTableMenu->actions().at(0)->setEnabled(false);
        mTableMenu->actions().at(1)->setEnabled(false);
    }
    mTableMenu->popup(mUi->tblProgram->viewport()->mapToGlobal(pos));
}

void ProgramFormController::onTableCellChanged(QModelIndex i1, QModelIndex i2)
{
    Q_UNUSED(i2)

    GCodeTableModel *model = (GCodeTableModel*)sender();

    if (i1.column() != 1)
    {
        return;
    }
    // Inserting new line at end
    if (i1.row() == (model->rowCount() - 1) && model->data(model->index(i1.row(), 1)).toString() != "")
    {
        model->setData(model->index(model->rowCount() - 1, 2), GCodeItem::InQueue);
        model->insertRow(model->rowCount());
        if (!mProgramLoading)
        {
            mUi->tblProgram->setCurrentIndex(model->index(i1.row() + 1, 1));
        }

    // Remove last line
    } /*else if (i1.row() != (model->rowCount() - 1) && model->data(model->index(i1.row(), 1)).toString() == "") {
        mUi->tblProgram->setCurrentIndex(model->index(i1.row() + 1, 1));
        mTableModel.removeRow(i1.row());
    }*/

    /*

    if (!mProgramLoading)
    {

        // Clear cached args
        model->setData(model->index(i1.row(), 5), QVariant());

        // Drop heightmap cache
        if (mCurrentModel == &mProgramModel) mProgramHeightmapModel.clear();

        // Update visualizer
        updateParser();

        // Hightlight w/o current cell changed event (double hightlight on current cell changed)
        QList<LineSegment*> list = mViewParser.getLineSegmentList();
        for (int i = 0; i < list.count() && list[i]->getLineNumber() <= mCurrentModel->data(mCurrentModel->index(i1.row(), 4)).toInt(); i++) {
            list[i]->setIsHightlight(true);
        }
    }
    */
}

void ProgramFormController::onTableCurrentChanged(QModelIndex idx1, QModelIndex idx2)
{
    /*
    // Update toolpath hightlighting
    if (idx1.row() > mCurrentModel->rowCount() - 2)
    {
        idx1 = mCurrentModel->index(mCurrentModel->rowCount() - 2, 0);
    }
    if (idx2.row() > mCurrentModel->rowCount() - 2)
    {
        idx2 = mCurrentModel->index(mCurrentModel->rowCount() - 2, 0);
    }

    GcodeViewParse *parser = mCurrentDrawer->viewParser();
    QList<LineSegment*> list = parser->getLineSegmentList();
    QVector<QList<int>> lineIndexes = parser->getLinesIndexes();

    // Update linesegments on cell changed
    if (!mCurrentDrawer->geometryUpdated())
    {
        for (int i = 0; i < list.count(); i++)
        {
            list.at(i)->setIsHightlight(list.at(i)->getLineNumber() <= mCurrentModel->data(mCurrentModel->index(idx1.row(), 4)).toInt());
        }
    // Update vertices on current cell changed
    }
    else
    {
        int lineFirst = mCurrentModel->data(mCurrentModel->index(idx1.row(), 4)).toInt();
        int lineLast = mCurrentModel->data(mCurrentModel->index(idx2.row(), 4)).toInt();
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
    int line = mCurrentModel->data(mCurrentModel->index(idx1.row(), 4)).toInt();
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

void ProgramFormController::onTableCellChanged(QModelIndex i1, QModelIndex i2)
{

}

void ProgramFormController::onTableInsertLine()
{
    /*
    if (mUi->tblProgram->selectionModel()->selectedRows().count() == 0 || mProcessingFile)
    {
        return;
    }

    int row = mUi->tblProgram->selectionModel()->selectedRows()[0].row();

    mCurrentModel->insertRow(row);
    mCurrentModel->setData(mCurrentModel->index(row, 2), GCodeItem::InQueue);

    updateParser();
    mCellChanged = true;
    mUi->tblProgram->selectRow(row);
    */
}

void ProgramFormController::onTableDeleteLines()
{
    /*
    if (
        mUi->tblProgram->selectionModel()->selectedRows().count() == 0 ||
        mProcessingFile ||
        QMessageBox::warning(
            this,
            this->windowTitle(),
            tr("Delete lines?"),
            QMessageBox::Yes | QMessageBox::No
        ) == QMessageBox::No
    ) {
        return;
    }

    QModelIndex firstRow = mUi->tblProgram->selectionModel()->selectedRows()[0];
    int rowsCount = mUi->tblProgram->selectionModel()->selectedRows().count();
    if (mUi->tblProgram->selectionModel()->selectedRows().last().row() == mCurrentModel->rowCount() - 1) rowsCount--;

    qDebug() << "deleting lines" << firstRow.row() << rowsCount;

    if (firstRow.row() != mCurrentModel->rowCount() - 1)
    {
        mCurrentModel->removeRows(firstRow.row(), rowsCount);
    } else return;

    // Drop heightmap cache
    if (mCurrentModel == &mProgramModel) {
        mProgramHeightmapModel.clear();
    }

    updateParser();
    mCellChanged = true;
    mUi->tblProgram->selectRow(firstRow.row());
    */
}

void ProgramFormController::clearTable()
{
    /*
    mProgramModel.clear();
    mProgramModel.insertRow(0);
    */
}



void ProgramFormController::onCmdFileSendClicked()
{
    /*
    if (mCurrentModel->rowCount() == 1)
    {
        return;
    }

    onCmdFileResetClicked();

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

#ifdef WINDOWS
    if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
        if (mTaskBarProgress) {
            mTaskBarProgress->setMaximum(mCurrentModel->rowCount() - 2);
            mTaskBarProgress->setValue(0);
            mTaskBarProgress->show();
        }
    }
#endif

    updateControlsState();
    mUi->cmdFilePause->setFocus();

    sendNextFileCommands();
    */
}

void ProgramFormController::onActSendFromLineTriggered()
{
    /*
    if (mCurrentModel->rowCount() == 1) {
        return;
    }

    //Line to start from
    int commandIndex = mUi->tblProgram->currentIndex().row();

    // Set parser state
    if (mSettingsForm->autoLine())
    {
        GcodeViewParse *parser = mCurrentDrawer->viewParser();
        QList<LineSegment*> list = parser->getLineSegmentList();
        QVector<QList<int>> lineIndexes = parser->getLinesIndexes();

        int lineNumber = mCurrentModel->data(mCurrentModel->index(commandIndex, 4)).toInt();
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
        list[i]->setDrawn(list.at(i)->getLineNumber() < mCurrentModel->data().at(commandIndex).line);
        indexes.append(i);
    }
    mCodeDrawer->update(indexes);

    mUi->tblProgram->setUpdatesEnabled(false);

    for (int i = 0; i < mCurrentModel->data().count() - 1; i++)
    {
        mCurrentModel->data()[i].state = i < commandIndex ? GCodeItem::Skipped : GCodeItem::InQueue;
        mCurrentModel->data()[i].response = QString();
    }
    mUi->tblProgram->setUpdatesEnabled(true);
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

#ifdef WINDOWS
    if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7)
    {
        if (mTaskBarProgress)
        {
            mTaskBarProgress->setMaximum(mCurrentModel->rowCount() - 2);
            mTaskBarProgress->setValue(commandIndex);
            mTaskBarProgress->show();
        }
    }
#endif

    updateControlsState();
    mUi->cmdFilePause->setFocus();

    mFileCommandIndex = commandIndex;
    mFileProcessedCommandIndex = commandIndex;
    sendNextFileCommands();
    */
}

void ProgramFormController::onChkBoxTestModeClicked(bool checked)
{

}

void ProgramFormController::onCmdFileAbortClicked()
{
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

void ProgramFormController::onCmdCommandSendClicked()
{

}


// Program

void ProgramFormController::onCmdFileOpenClicked()
{
    /*
    if (!mHeightMapMode)
    {
        if (!saveChanges(false))
        {
            return;
        }

        QString fileName  = QFileDialog::getOpenFileName(
            this,
            tr("Open"),
            mLastFolder,
            tr("G-Code files (*.nc *.ncc *.ngc *.tap *.txt);;All files (*.*)")
        );

        if (!fileName.isEmpty())
        {
            mLastFolder = fileName.left(fileName.lastIndexOf(QRegExp("[/\\\\]+")));
        }

        if (fileName != "")
        {
            addRecentFile(fileName);
            updateRecentFilesMenu();
            loadFile(fileName);
        }
    }
    else
    {
        if (!saveChanges(true))
        {
            return;
        }

        QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), mLastFolder, tr("Heightmap files (*.map)"));

        if (fileName != "")
        {
            addRecentHeightmap(fileName);
            updateRecentFilesMenu();
            loadHeightMap(fileName);
        }
    }
    */
}

void ProgramFormController::onCmdFilePauseClicked(bool checked)
{
    //mSerialPort.write(checked ? "!" : "~");
}

void ProgramFormController::onCmdFileResetClicked()
{
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

        mUi->tblProgram->setUpdatesEnabled(false);

        for (int i = 0; i < mCurrentGCodeTableModel->data().count() - 1; i++) {
            mCurrentGCodeTableModel->data()[i].state = GCodeItem::InQueue;
            mCurrentGCodeTableModel->data()[i].response = QString();
        }
        mUi->tblProgram->setUpdatesEnabled(true);

        qDebug() << "table updated:" << time.elapsed();

        mUi->tblProgram->scrollTo(mCurrentGCodeTableModel->index(0, 0));
        mUi->tblProgram->clearSelection();
        mUi->tblProgram->selectRow(0);

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

