/*
 * ProgramFormController.h
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

#pragma once

#include <QMenu>
#include <QTimer>

#include "AbstractFormController.h"
#include "Model/Gcode/GcodeTableModel.h"
#include "Model/Gcode/GcodeMarkerListModel.h"

#include "ui_ProgramForm.h"

class GcodeFileModel;

using namespace Ui;

class ProgramFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit ProgramFormController(QWidget *parent = nullptr);
    ~ProgramFormController() override;

    bool isPauseChecked();
    void setPauseChecked(bool);

    bool isAutoScrollChecked();
    void setAutoScrollChecked(bool);
    int getChkTestModeWidth(); // sizehint->width
    int getChkAutoScrollWidth(); // sizehint->width
    void setFormActive(bool active) override;

    void initialise() override;

signals:
    void setKeyboardControlSignal(bool);
    void programAbortSignal();
    void programPauseSignal(QString);
    void programResetSignal();
    void sendProgramSignal();
    void sendProgramFromLineSignal(long);
public slots:
    void onReserveGcodeRowsSignal(int);
    void onTestModeButtonClicked(bool checked);
    void onAbortButtonClicked();
    void onPauseButtonClicked(bool checked);
    void onResetButtonClicked();

    void onTableInsertLine();
    void onTableDeleteLines();
    void onTableCurrentChanged(QModelIndex idx1, QModelIndex idx2);
    void onTableCellChanged(QModelIndex i1, QModelIndex i2);
    void onTableProgramCustomContextMenuRequested(const QPoint &pos);

    void onScrollBarAction(int action);
    void onGcodeFileLoadStarted();
    void onGcodeFileLoadFinished(GcodeFileModel*);
    void onSendActionTriggered();
    void onSendFromCurrentLineActionTriggered();
    void onUpdateProgramTableStatus(GcodeCommand* command);

private slots:

    void onMarkerSelectionChanged(const QItemSelection&, const QItemSelection&);
    void onTestModeButtonChecked(bool checked);

private:
    ProgramForm mUi;
    GcodeTableModel mProgramTableModel;
    GcodeMarkerListModel mMarkerListModel;
    bool mCellChanged;
    QTimer mStartTime;
    bool mTransferCompleted;
    bool mProcessingFile;
    bool mFileEndSent;
    bool mProgramLoading;

    QMenu mTableMenu;

    QMenu mSendMenu;
    QAction mSendAction;
    QAction mSendFromCurrentLineAction;

    void clearTable();
    QByteArray mHeaderState;

    // AbstractFormController interface
    void setupSendMenu();
    void setupProgramTable();
public:
    void setupSignalSlots() override;
};
