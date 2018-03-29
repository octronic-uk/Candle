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
#include "Model/Tables/GcodeTableModel.h"

#include "ui_ProgramForm.h"

using namespace Ui;

class ProgramFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit ProgramFormController(QWidget *parent = nullptr);
    ~ProgramFormController();

    bool isPauseChecked();
    void setPauseChecked(bool);

    bool isAutoScrollChecked();
    void setAutoScrollChecked(bool);
    int getChkTestModeWidth(); // sizehint->width
    int getChkAutoScrollWidth(); // sizehint->width

signals:
    void setKeyboardControlSignal(bool);
public slots:
    void onReserveGcodeRowsSignal(int);
    void onActSendFromLineTriggered();
    void onChkBoxTestModeClicked(bool checked);
    void onCmdFileSendClicked();
    void onCmdFileAbortClicked();
    void onCmdCommandSendClicked();
    void onCmdFilePauseClicked(bool checked);
    void onCmdFileResetClicked();

    void onTableInsertLine();
    void onTableDeleteLines();
    void onTableCurrentChanged(QModelIndex idx1, QModelIndex idx2);
    void onTableCellChanged(QModelIndex i1, QModelIndex i2);
    void onTableProgramCustomContextMenuRequested(const QPoint &pos);

    void onScrollBarAction(int action);
    void onGcodeFileLoadStarted();
    void onGcodeFileLoadFinished(QList<GcodeItem>);
private:
    ProgramForm mUi;
    GcodeTableModel mProgramTableModel;
    bool mCellChanged;
    QTimer mStartTime;
    bool mTransferCompleted;
    bool mProcessingFile;
    bool mFileEndSent;
    bool mProgramLoading;

    QMenu mTableMenu;
    QMenu mSendMenu;

    void clearTable();
    QByteArray mHeaderState;

    // AbstractFormController interface
public:
    void setupSignalSlots() override;
};
