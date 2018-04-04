// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QMainWindow>
#include <QSettings>
#include <QTimer>
#include <QBasicTimer>
#include <QStringList>
#include <QList>
#include <QTime>
#include <QMenu>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QProgressBar>
#include <exception>
#include <QHBoxLayout>

#include "Model/Parser/GcodeViewParse.h"
#include "Model/Settings/IniFileSettingsModel.h"
#include "Model/HeightMapFileModel.h"

#include "Model/RecentFilesModel.h"
#include "Model/RecentHeightMapFilesModel.h"

#include "View/Drawers/OriginDrawer.h"
#include "View/Drawers/GcodeDrawer.h"
#include "View/Drawers/ToolDrawer.h"
#include "View/Drawers/HeightMapBorderDrawer.h"
#include "View/Drawers/HeightMapGridDrawer.h"
#include "View/Drawers/HeightMapInterpolationDrawer.h"
#include "View/Drawers/ShaderDrawable.h"
#include "View/Drawers/SelectionDrawer.h"
#include "Model/GcodeFileModel.h"

#include "Model/Tables/GcodeTableModel.h"
#include "Model/Tables/HeightMapTableModel.h"
#include "Model/GrblMachineModel.h"

#include "Utils/Interpolation.h"

#include "Controller/AboutFormController.h"
#include "Controller/ConsoleFormController.h"
#include "Controller/ControlFormController.h"
#include "Controller/FeedFormController.h"
#include "Controller/HeightMapFormController.h"
#include "Controller/JogFormController.h"
#include "Controller/ProgramFormController.h"
#include "Controller/SettingsFormController.h"
#include "Controller/SpindleFormController.h"
#include "Controller/StateFormController.h"
#include "Controller/UserCommandsFormController.h"
#include "Controller/VisualisationFormController.h"
#include "ui_MainForm.h"
#include "CancelException.h"

#ifdef WINDOWS
#include <QtWinExtras/QtWinExtras>
#include "shobjidl.h" // What the quadralateral fuck is this Microsoft?
#endif

enum class MainFormMode
{
    Idle,
    Gerber,
    RunningGerber,
    HeightMap
};

class MainFormController : public AbstractFormController
{
    Q_OBJECT

public:
    explicit MainFormController(QWidget *parent = nullptr);
    ~MainFormController() override;

    void showMainWindow();
    void setupSignalSlots() override;
    bool isInHeightMapMode();
    bool isInGerberMode();

signals:
   void sendProgramSignal(GcodeFileModel&);
   void sendProgramFromLineSignal(GcodeFileModel&, long);

public slots:
    void onRecentHeightMapFilesChanged();
    void onRecentGcodeFilesChanged();
    void onStatusBarUpdate(QString status);
    void onStatusTextUpdate(QString status);
    void onGcodeFileLoadStarted();
    void onGcodeFileLoadFinished(QList<GcodeCommand*>& items);
    void onSendProgram();
    void onSendProgramFromLine(long);
    void onSerialPortError(QString error);
    void onSetBufferProgressValue(int);
    void onSetCompletionProgressValue(int);
    void onSetFormMode(MainFormMode mode);

private slots:
    // Main Menu Actions
    void onActFileExitTriggered();
    void onActFileOpenTriggered();
    void onActFileNewTriggered();
    void onActFileSaveAsTriggered();
    void onActFileSaveTriggered();
    void onActFileSaveTransformedAsTriggered();
    void onActSettingsTriggered();
    void onActAboutTriggered();
    void onActRecentFileTriggered();
    void onHeightMapFileLoadStarted();
    void onHeightMapFileLoadFinished();
    void onMachineStateUpdated(const GrblMachineState& state);

protected:
    void showEvent(QShowEvent *se) override;
    void hideEvent(QHideEvent *he) override;
    void resizeEvent(QResizeEvent *re) override;
    void closeEvent(QCloseEvent *ce) override;
    void dragEnterEvent(QDragEnterEvent *dee) override;
    void dropEvent(QDropEvent *de) override;

private: // Members
    Ui::MainForm mUi;
    AboutFormController mAboutFormController;
    SettingsFormController mSettingsFormController;
    RecentFilesModel mRecentFilesModel;
    RecentHeightMapFilesModel mRecentHeightMapFilesModel;
    MainFormMode mFormMode;
    QString mLastFolder;
    QTimer mConnectionTimer;
    QTimer mStateQueryTimer;
    IniFileSettingsModel mSettingsModel;
    QMainWindow mMainWindow;
    GcodeFileModel mGcodeFileModel;
    HeightMapFileModel mHeightMapFileModel;
    bool mHeightMapMode = false;
    GrblMachineModel mGrblMachineModel;
    QProgressBar mCompletionProgressBar;
    QProgressBar mBufferProgressBar;
    QHBoxLayout mStatusProgressBarsHLayout;
    QLayout* mOldStatusBarLayout;


private: // Member Functions
    bool eventFilter(QObject *obj, QEvent *event) override;
    bool keyIsMovement(int key);
    void updateRecentFilesMenu();
    bool saveChanges(bool heightMapMode);
    void setFormActive(bool active) override;
    void populateRecentGcodeFilesMenu();
    void populateRecentHeightMapFilesMenu();
    void clearRecentGcodeFilesMenu();
    void clearRecentHeightMapFilesMenu();
    void setupToolbarActions();
    void setupToolbarSignals();
    void setupSettingsModelSignals();
    void setupMenuBarSignals();
    void setupGcodeFileModelSignals();
    void setupHeightMapFileModelSignals();
    void setupRecentFilesModelsSignals();
    void setupGrblMachineModelSignals();
    void setupConsoleFormSignals();
    void setupControlFormSignals();
    void setupProgramFormSignals();
    void setupJogFormSignals();
    void setupCompletionAndBufferProgressBars();
};


