// This file is a part of "Cocoanut" application.
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
#include <QProgressDialog>
#include <exception>

#include "Model/Parser/GcodeViewParse.h"
#include "Model/SettingsModel.h"
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
#include "Model/SerialPortModel.h"

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
#include "CommandAttributes.h"
#include "CommandQueue.h"
#include "CancelException.h"

#ifdef WINDOWS
#include <QtWinExtras/QtWinExtras>
#include "shobjidl.h" // What the quadralateral fuck is this Microsoft?
#endif

enum MainFormMode
{
    MAIN_FORM_MODE_NONE,
    MAIN_FORM_MODE_GERBER,
    MAIN_FORM_MODE_HEIGHTMAP
};

class MainFormController : public AbstractFormController
{
    Q_OBJECT

public:
    explicit MainFormController(QWidget *parent = 0);
    ~MainFormController();

    double toolZPosition();
    void showMainWindow();

    bool getHoming() const;
    void setHoming(bool homing);

private slots:

    void onTimerConnection();
    void onTimerStateQuery();
    void onPanelsSizeChanged(QSize size);

    // Main Menu Actions
    void onActFileExitTriggered();
    void onActFileOpenTriggered();
    void onActFileNewTriggered();
    void onActFileSaveAsTriggered();
    void onActFileSaveTriggered();
    void onActFileSaveTransformedAsTriggered();

    void onActSettingsTriggered();
    void onActAboutTriggered();
    void onActRecentClearTriggered();
    void onActRecentFileTriggered();

    void onGripFeedToggled(bool checked);
    void onGripSpindleToggled(bool checked);
    void onGripJogToggled(bool checked);
    void onGripUserCommandsToggled(bool checked);

    void onHeightMapFileLoadStarted();
    void onHeightMapFileLoadFinished();

    void onSerialPortError(QString error);

protected:
    void showEvent(QShowEvent *se) override;
    void hideEvent(QHideEvent *he) override;
    void resizeEvent(QResizeEvent *re) override;
    void timerEvent(QTimerEvent *) override;
    void closeEvent(QCloseEvent *ce) override;
    void dragEnterEvent(QDragEnterEvent *dee) override;
    void dropEvent(QDropEvent *de) override;
    void onSplitterSplitterMoved(int pos, int index);

private:
    const int BUFFERLENGTH = 127;

    Ui::MainForm mUi;

    // Dialog Controllers
    AboutFormController mAboutFormController;
    SettingsFormController mSettingsFormController;
    RecentFilesModel mRecentFilesModel;
    RecentHeightMapFilesModel mRecentHeightMapFilesModel;
    MainFormMode mFormMode;

    QString mLastFolder;
    QTimer mConnectionTimer;
    QTimer mStateQueryTimer;

    SettingsModel mSettingsModel;

#ifdef WINDOWS
    QWinTaskbarButton *m_taskBarButton;
    QWinTaskbarProgress *m_taskBarProgress;
#endif

    QMenu *mTableMenu;
    QList<CommandAttributes> mCommandsList;
    QList<CommandQueue> mCommandsQueue;
    QTime mStartTime;

    QSharedPointer<QMessageBox> mSenderErrorBox;

    // Stored origin
    double mStoredOriginX = 0;
    double mStoredOriginY = 0;
    double mStoredOriginZ = 0;

    QString mStoredParserStatus;
    bool mIsUpdatingParserStatus = false;
    bool mStatusReceived = false;
    bool mIsProcessingFile = false;
    bool mIsTransferCompleted = false;
    bool mIsFileEndSent = false;
    bool mCellChanged;

    // Indices
    int mFileCommandIndex;
    int mFileProcessedCommandIndex;
    int mProbeIndex;

    // Current values
    int mLastGrblStatus;

    void updateParser();

    bool dataIsFloating(QString data);
    bool dataIsEnd(QString data);
    bool dataIsReset(QString data);

    QTime updateProgramEstimatedTime(QList<LineSegment *> lines);

    bool eventFilter(QObject *obj, QEvent *event) override;
    bool keyIsMovement(int key);
    void resizeCheckBoxes();
    void updateLayouts();
    void updateRecentFilesMenu();

    void storeParserState();
    void restoreParserState();

    void storeOffsets();
    void restoreOffsets();

    bool saveChanges(bool heightMapMode);
    int bufferLength();

private:
    QMainWindow mMainWindow;
    GcodeFileModel mGcodeFileModel;
    HeightMapFileModel mHeightMapFileModel;
    bool mHeightMapMode = false;
    SerialPortModel mSerialPortModel;
    void setFormMode(MainFormMode mode);

    void populateRecentGcodeFilesMenu();
    void populateRecentHeightMapFilesMenu();
    void clearRecentGcodeFilesMenu();
    void clearRecentHeightMapFilesMenu();
    void setupToolbarActions();
public:
    void setupSignalSlots() override;
    bool isInHeightMapMode();
    bool isInGerberMode();
public slots:
    void onRecentHeightMapFilesChanged();
    void onRecentGcodeFilesChanged();
    void onStatusUpdate(QString status);
    void onGcodeFileLoadStarted();
    void onGcodeFileLoadFinished();
    void onCommandSent(QString command, int len);
};


