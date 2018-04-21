// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QMainWindow>
#include <QSettings>
#include <QStringList>
#include <QList>
#include <QMenu>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QProgressBar>
#include <exception>
#include <QHBoxLayout>

#include "Model/Gcode/Parser/GcodeViewParser.h"
#include "Model/Settings/Sql/SqlSettingsModel.h"
#include "Model/RecentFilesModel.h"
#include "Model/Gcode/GcodeFileModel.h"
#include "Model/Gcode/GcodeTableModel.h"
#include "Model/Grbl/GrblMachineModel.h"

#include "View/Drawers/OriginDrawer.h"
#include "View/Drawers/GcodeDrawer.h"
#include "View/Drawers/ToolDrawer.h"
#include "View/Drawers/ShaderDrawable.h"
#include "View/Drawers/SelectionDrawer.h"

#include "Controller/AboutFormController.h"
#include "Controller/Settings/SettingsFormController.h"


#include "Utils/Interpolation.h"

#include "CancelException.h"

#include "ui_MainForm.h"

#ifdef WINDOWS
#include <QtWinExtras/QtWinExtras>
#include "shobjidl.h" // What the quadralateral fuck is this Microsoft?
#endif

class MainFormController : public AbstractFormController
{
    Q_OBJECT

public:
    explicit MainFormController(QWidget *parent = nullptr);
    ~MainFormController() override;

    void showMainWindow();
    void setupSignalSlots() override;

    void initialise() override;

signals:
   void sendProgramSignal(const GcodeFileModel&);
   void sendProgramFromLineSignal(const GcodeFileModel&, long);
   void toolChangeCompletedSignal();

public slots:
    void onGrblMachineConnected(bool);
    void onRecentGcodeFilesChanged();
    void onStatusBarUpdate(QString status);
    void onStatusTextUpdate(QString status);
    void onGcodeFileLoadStarted();
    void onGcodeFileLoadFinished(GcodeFileModel* items);
    void onSendProgram();
    void onSendProgramFromLine(long);
    void onSerialPortError(QString error);
    void onSetBufferProgressValue(int);
    void onSetCompletionProgressValue(int);
    void onGrblMachineError(QString error);
    void onProfileChanged(Profile* model);
    void onAlarm(QString alarmMsg);

private slots:
    // Main Menu Actions
    void onJobCompleted();
    void onActFileExitTriggered();
    void onActFileOpenTriggered();
    void onActFileSaveAsTriggered();
    void onActFileSaveTriggered();
    void onActFileSaveTransformedAsTriggered();
    void onActSettingsTriggered();
    void onActAboutTriggered();
    void onActRecentFileTriggered();
    void onMachineStateUpdated(const GrblMachineState& state);
    void onActionClearAllTriggered();
    void onStopTriggered(bool ean);
    void onToolChange(int);

private: // Members
    Ui::MainForm mUi;
    AboutFormController mAboutFormController;
    SettingsFormController mSettingsFormController;

    QString mLastFolder;
    QSharedPointer<SqlSettingsModel> mSqlSettingsModel;
    RecentFilesModel* mRecentGcodeFilesModelHandle;
    QMainWindow mMainWindow;
    GcodeFileModel mGcodeFileModel;
    GrblMachineModel mGrblMachineModel;
    QProgressBar mCompletionProgressBar;
    QProgressBar mBufferProgressBar;
    QHBoxLayout mStatusProgressBarsHLayout;
    QLayout* mOldStatusBarLayout;


private: // Member Functions
    bool keyIsMovement(int key);
    void updateRecentFilesMenu();
    bool saveChanges();
    void setFormActive(bool active) override;
    void populateRecentGcodeFilesMenu();
    void clearRecentGcodeFilesMenu();
    void setupToolbarActions();
    void setupToolbarSignals();
    void setupSettingsModelSignals();
    void setupMenuBarSignals();
    void setupGcodeFileModelSignals();
    void setupRecentFilesModelsSignals();
    void setupGrblMachineModelSignals();
    void setupConsoleFormSignals();
    void setupControlFormSignals();
    void setupProgramFormSignals();
    void setupJogFormSignals();
    void setupCompletionAndBufferProgressBars();
};


