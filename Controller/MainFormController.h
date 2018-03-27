// This file is a part of "Candle" application.
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

#include "Model/Parser/GCodeViewParse.h"
#include "Model/SettingsModel.h"
#include "Model/HeightMapFileModel.h"

#include "View/Drawers/OriginDrawer.h"
#include "View/Drawers/GCodeDrawer.h"
#include "View/Drawers/ToolDrawer.h"
#include "View/Drawers/HeightMapBorderDrawer.h"
#include "View/Drawers/HeightMapGridDrawer.h"
#include "View/Drawers/HeightMapInterpolationDrawer.h"
#include "View/Drawers/ShaderDrawable.h"
#include "View/Drawers/SelectionDrawer.h"
#include "Model/GCodeFileModel.h"

#include "Model/Tables/GCodeTableModel.h"
#include "Model/Tables/HeightMapTableModel.h"
#include "Model/SerialPortModel.h"

#include "Utils/Interpolation.h"

#include "View/Widgets/StyledToolButton.h"

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

    void onScroolBarAction(int action);
    void onPanelsSizeChanged(QSize size);


    // Main Menu Actions

    void onActFileExitTriggered();
    void onActFileOpenTriggered();
    void onActFileNewTriggered();
    void onActFileSaveAsTriggered();
    void onActFileSaveTriggered();
    void onActFileSaveTransformedAsTriggered();

    void onActServiceSettingsTriggered();
    void onActAboutTriggered();
    void onActRecentClearTriggered();
    void onActRecentFileTriggered();
    void onGripFeedToggled(bool checked);
    void onGripSpindleToggled(bool checked);
    void onGripJogToggled(bool checked);
    void onGripUserCommandsToggled(bool checked);

protected:
    void showEvent(QShowEvent *se);
    void hideEvent(QHideEvent *he);
    void resizeEvent(QResizeEvent *re);
    void timerEvent(QTimerEvent *);
    void closeEvent(QCloseEvent *ce);
    void dragEnterEvent(QDragEnterEvent *dee);
    void dropEvent(QDropEvent *de);
    void onSplitterSplitterMoved(int pos, int index);

private:
    const int BUFFERLENGTH = 127;

    Ui::MainForm mUi;

    // Form Controllers
    QSharedPointer<ConsoleFormController> mConsoleFormController;
    QSharedPointer<ControlFormController> mControlFormController;
    QSharedPointer<FeedFormController> mFeedFormController;
    QSharedPointer<HeightMapFormController> mHeightMapFormController;
    QSharedPointer<JogFormController> mJogFormController;
    QSharedPointer<ProgramFormController> mProgramFormController;
    QSharedPointer<SpindleFormController> mSpindleFormController;
    QSharedPointer<StateFormController> mStateFormController;
    QSharedPointer<UserCommandsFormController> mUserCommandsFormController;
    QSharedPointer<VisualisationFormController> mVisualisationFormController;

    // Dialog Controllers
    QSharedPointer<AboutFormController> mAboutFormController;
    QSharedPointer<SettingsFormController> mSettingsFormController;

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

    bool eventFilter(QObject *obj, QEvent *event);
    bool keyIsMovement(int key);
    void resizeCheckBoxes();
    void updateLayouts();
    void updateRecentFilesMenu();

    void storeParserState();
    void restoreParserState();

    void storeOffsets();
    void restoreOffsets();
    bool compareCoordinates(double x, double y, double z);

    bool saveChanges(bool heightMapMode);
    int bufferLength();

private:
    QMainWindow mMainWindow;
    GCodeFileModel mGcodeFileModel;
    HeightMapFileModel mHeightMapFileModel;
    bool mHeightMapMode = false;
    SerialPortModel mSerialPortModel;
};
