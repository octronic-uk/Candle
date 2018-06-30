// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich


#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <QTextBlock>
#include <QTextCursor>
#include <QMessageBox>
#include <QComboBox>
#include <QScrollBar>
#include <QShortcut>
#include <QAction>
#include <QLayout>
#include <QMimeData>
#include <QMessageBox>
#include <QtDebug>
#include <QErrorMessage>
#include <QDesktopWidget>

#include "Controller/ConsoleFormController.h"
#include "Controller/ControlFormController.h"
#include "Controller/JogFormController.h"
#include "Controller/ProgramFormController.h"
#include "Controller/OverrideFormController.h"
#include "Controller/StateFormController.h"
#include "Controller/VisualisationFormController.h"
#include "Model/RecentFile.h"

#include "Model/Gcode/Parser/GcodeParser.h"
#include "Model/Grbl/GrblMachineState.h"

#include "MainFormController.h"
#include "ui_MainForm.h"

MainFormController::MainFormController(QWidget *parent) :
    AbstractFormController(parent),
    mLastFolder(QDir::homePath()),
    mRecentGcodeFilesModelHandle(nullptr),
    mOldStatusBarLayout(nullptr)
{
    qDebug() << "MainFormController: Constructing";
    mUi.setupUi(&mMainWindow);
    setupToolbarActions();
    setupCompletionAndBufferProgressBars();
    mSqlSettingsModel = QSharedPointer<SqlSettingsModel>::create();
    setupSignalSlots();
    onMachineStateUpdated(GrblMachineState::Locked);

    mUi.splitter->setSizes(QList<int>() << 200 << 200);
    mSqlSettingsModel->onLoadSettings();
    mUi.stateFormController->onStatusTextUpdate("Not Connected",QColor("Gray"),QColor("LightBlue"));
}

MainFormController::~MainFormController()
{
    qDebug() << "MainFormController: Destructing";
}

void MainFormController::setupToolbarActions()
{
    QAction *console = mUi.consoleDockWidget->toggleViewAction();
    console->setIcon(QIcon(":/Images/SVG/console.svg"));
    mUi.toolBar->addAction(console);

    QAction *control = mUi.controlDockWidget->toggleViewAction();
    control->setIcon(QIcon(":/Images/SVG/origin.svg"));
    mUi.toolBar->addAction(control);

    QAction *jog = mUi.jogDockWidget->toggleViewAction();
    jog->setIcon(QIcon(":/Images/SVG/crosshairs.svg"));
    mUi.toolBar->addAction(jog);

    QAction *spindle = mUi.overridesDockWidget->toggleViewAction();
    spindle->setIcon(QIcon(":/Images/SVG/feed_rate.svg"));
    mUi.toolBar->addAction(spindle);

    QAction *state = mUi.stateDockWidget->toggleViewAction();
    state->setIcon(QIcon(":/Images/SVG/eye.svg"));
    mUi.toolBar->addAction(state);

    QWidget* toolbarSpacer = new QWidget();
    toolbarSpacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    mUi.toolBar->addWidget(toolbarSpacer);

    QAction *eStop = mUi.actionStop;
    mUi.toolBar->addAction(eStop);
}

void MainFormController::setupToolbarSignals()
{
    // Preferences
    connect(
        mUi.actFileSettings, SIGNAL(triggered()),
        this, SLOT(onActSettingsTriggered())
    );
    // Clear All
    connect
    (
        mUi.actionClearAll, SIGNAL(triggered()),
        this, SLOT(onActionClearAllTriggered())
    );
    // Connect
    connect
    (
        mUi.actionConnect, SIGNAL(triggered()),
        &mGrblMachineModel, SLOT(onConnect())
    );
    // Stop
    connect
    (
        mUi.actionStop, SIGNAL(toggled(bool)),
        this, SLOT(onStopTriggered(bool))
    );

}

void MainFormController::setupSettingsModelSignals()
{
    // Settings Model
    connect
    (
        mSqlSettingsModel.data(),
        SIGNAL(settingsModelReadySignal(SqlSettingsModel*)),
        &mSettingsFormController,
        SLOT(onSettingsModelReady(SqlSettingsModel*))
    );
    connect
    (
        mSqlSettingsModel.data(),
        SIGNAL(settingsModelReadySignal(SqlSettingsModel*)),
        mUi.controlFormController,
        SLOT(onSettingsModelReady(SqlSettingsModel*))
    );
    connect
    (
        mSqlSettingsModel.data(),
        SIGNAL(settingsModelReadySignal(SqlSettingsModel*)),
        mUi.visualisationFormController,
        SLOT(onSettingsModelReady(SqlSettingsModel*))
    );
    // Serial Port
    connect(
        mSettingsFormController.getConnectionFormController(),SIGNAL(serialPortNameChangedSignal(QString)),
        &mGrblMachineModel, SLOT(onSerialPortNameChanged(QString))
    );
    connect
    (
        mSettingsFormController.getConnectionFormController(), SIGNAL(serialPortBaudRateChangedSignal(int)),
        &mGrblMachineModel, SLOT(onSerialPortBaudRateChanged(int))
    );
    // Firmware Config
    connect
    (
        &mGrblMachineModel, SIGNAL(firmwareConfigurationReadSignal(int,QString)),
        &mSettingsFormController, SLOT(onFirmwareConfigurationRead(int,QString))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(firmwareConfigurationReadSignal(int,QString)),
        mUi.visualisationFormController, SLOT(onFirmwareConfigurationRead(int,QString))
    );
    connect(
        mSettingsFormController.getGrblConfigurationFormControllerHandle(),
        SIGNAL(gcodeCommandManualSendSignal(GcodeCommand*)),
        &mGrblMachineModel,
        SLOT(onGcodeCommandManualSend(GcodeCommand*))

    );
}

void MainFormController::setupMenuBarSignals()
{
    // About
    connect(
        mUi.actAbout, SIGNAL(triggered()),
        this, SLOT(onActAboutTriggered())
    );
    // File->Open
    connect(
        mUi.actFileOpen, SIGNAL(triggered()),
        this, SLOT(onActFileOpenTriggered())
    );
    // File->Save
    connect(
        mUi.actFileSave, SIGNAL(triggered()),
        this, SLOT(onActFileSaveTriggered())
    );
    // File->Save As...
    connect(
        mUi.actFileSaveAs, SIGNAL(triggered()),
        this, SLOT(onActFileSaveAsTriggered())
    );
    // File->Save Transformed As...
    connect(
        mUi.actFileSaveTransformedAs, SIGNAL(triggered()),
        this, SLOT(onActFileSaveTransformedAsTriggered())
    );
    // File Exit
    connect
    (
        mUi.actFileExit, SIGNAL(triggered()),
        this, SLOT(onActFileExitTriggered())
    );
}

void MainFormController::setupGcodeFileModelSignals()
{
    //qDebug() << "MainFormController: setupGcodeFileModelSignals";
    // Gcode File Model
    connect(
        mGcodeFileModel.data(), SIGNAL(statusBarUpdateSignal(QString)),
        this, SLOT(onStatusBarUpdate(QString))
    );
    connect(
        mGcodeFileModel.data(), SIGNAL(gcodeFileLoadStartedSignal()),
        this, SLOT(onGcodeFileLoadStarted())
    );
    connect(
        mGcodeFileModel.data(), SIGNAL(reserveGcodeRowsSignal(int)),
        mUi.programFormController, SLOT(onReserveGcodeRowsSignal(int))
    );
    connect(
        mGcodeFileModel.data(), SIGNAL(gcodeFileLoadStartedSignal()),
        mUi.programFormController, SLOT(onGcodeFileLoadStarted())
    );
    // Gcode Loading Finished
    connect(
        mGcodeFileModel.data(), SIGNAL(gcodeFileLoadFinishedSignal(GcodeFileModel*)),
        mUi.programFormController, SLOT(onGcodeFileLoadFinished(GcodeFileModel*))
    );
    connect(
        mGcodeFileModel.data(), SIGNAL(gcodeFileLoadFinishedSignal(GcodeFileModel*)),
        mUi.visualisationFormController, SLOT(onGcodeFileLoadFinished(GcodeFileModel*))
    );
    connect(
        mGcodeFileModel.data(), SIGNAL(gcodeFileLoadFinishedSignal(GcodeFileModel*)),
        this, SLOT(onGcodeFileLoadFinished(GcodeFileModel*))
    );
    // Gcode Loading Started
    connect(
        mGcodeFileModel.data(), SIGNAL(gcodeFileLoadStartedSignal()),
        mUi.visualisationFormController, SLOT(onGcodeFileLoadStarted())
    );
    connect(
        mGcodeFileModel.data(), SIGNAL(gcodeFileLoadStartedSignal()),
        this, SLOT(onGcodeFileLoadStarted())
    );
    // Parser State
    connect(
        mGcodeFileModel.data(), SIGNAL(gcodeParserUpdatedSignal(GcodeParser*)),
        mUi.visualisationFormController, SLOT(onGcodeParserUpdated(GcodeParser*))
    );
}

void MainFormController::setupRecentFilesModelsSignals()
{
    // Recent files changed handlers
    connect(
        mSqlSettingsModel.data(),
        SIGNAL(profileChangedSignal(Profile*)),
        this,
        SLOT(onProfileChanged(Profile*))
    );
}

void MainFormController::onProfileChanged(Profile* profile)
{
    mRecentGcodeFilesModelHandle = profile->getRecentGcodeFilesModelHandle();
}

void MainFormController::onAlarm(QString alarmMsg)
{
    // Wassat now?
    QMessageBox::critical
    (
        &mMainWindow,
        tr("Grbl Alarm!"),
        QString("%1").arg(alarmMsg),
        QMessageBox::Ok
    );
    // Lock the UI
    onMachineStateUpdated(GrblMachineState::Locked);
}

void MainFormController::setupGrblMachineModelSignals()
{
    // Grbl Machine Model
    connect
    (
        &mGrblMachineModel, SIGNAL(serialPortErrorSignal(QString)),
        this, SLOT(onSerialPortError(QString))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(statusBarUpdateSignal(QString)),
        this, SLOT(onStatusBarUpdate(QString))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(setCompletionProgressSignal(int)),
        this, SLOT(onSetCompletionProgressValue(int))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(setBufferProgressSignal(int)),
        this, SLOT(onSetBufferProgressValue(int))
    );
    connect
    (
        this, SIGNAL(sendProgramSignal(GcodeFileModel*)),
        &mGrblMachineModel, SLOT(onSendProgram(GcodeFileModel*))
    );
    connect
    (
        this, SIGNAL(sendProgramFromLineSignal(GcodeFileModel*,long)),
        &mGrblMachineModel, SLOT(onSendProgramFromLine(GcodeFileModel*,long))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(jobCompletedSignal()),
        this, SLOT(onJobCompleted())
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(updateMachinePositionSignal(const QVector3D)),
        mUi.stateFormController, SLOT(onUpdateMachinePosition(const QVector3D))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(updateWorkPositionSignal(const QVector3D)),
        mUi.stateFormController, SLOT(onUpdateWorkPosition(const QVector3D))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(updateWorkPositionSignal(const QVector3D)),
        mUi.visualisationFormController, SLOT(onUpdateWorkPosition(const QVector3D))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(updateWCOSignal(const QVector3D)),
        mUi.visualisationFormController, SLOT(onUpdateWCO(const QVector3D))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(updateWorkPositionSignal(const QVector3D)),
        mUi.visualisationFormController, SLOT(onUpdateWorkPosition(const QVector3D))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(updateMachinePositionSignal(const QVector3D)),
        mUi.visualisationFormController, SLOT(onUpdateMachinePosition(const QVector3D))
    );
    // Machine State
    connect
    (
        &mGrblMachineModel, SIGNAL(errorSignal(QString)),
        this, SLOT(onGrblMachineError(QString))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(machineConnectedSigal(bool)),
        this, SLOT(onGrblMachineConnected(bool))
    );
    // Tool Change
    connect
    (
        &mGrblMachineModel, SIGNAL(toolChangeSignal(int)),
        this, SLOT(onToolChange(int))
    );
    connect
    (
        this, SIGNAL(toolChangeCompletedSignal()),
        &mGrblMachineModel, SLOT(onToolChangeCompleted())
    );

}

void MainFormController::setupJogFormSignals()
{
    // Jog Form
    connect(
        mUi.jogFormController, SIGNAL(statusBarUpdateSignal(QString)),
        this, SLOT(onStatusBarUpdate(QString))
    );
    connect(
        mUi.jogFormController,
        SIGNAL(gcodeCommandManualSendSignal(GcodeCommand*)),
        &mGrblMachineModel,
        SLOT(onGcodeCommandManualSend(GcodeCommand*))
    );
}

void MainFormController::setupStateFormSignals()
{
   connect(
        &mGrblMachineModel, SIGNAL(feedRateChangedSignal(int)),
        mUi.stateFormController, SLOT(onFeedRateChanged(int))
    );
    connect(
        &mGrblMachineModel, SIGNAL(spindleSpeedChangedSignal(int)),
        mUi.stateFormController, SLOT(onSpindleSpeedChanged(int))
                );
}

void MainFormController::setupVisualisationFormSignals()
{
    connect(
        &mGrblMachineModel, SIGNAL(spindleSpeedChangedSignal(int)),
        mUi.visualisationFormController, SLOT(onSpindleSpeedChanged(int))
    );
}

void MainFormController::setupConsoleFormSignals()
{
    // Console Form
    connect(
        &mGrblMachineModel, SIGNAL(appendCommandToConsoleSignal(GcodeCommand*)),
        mUi.consoleFormController, SLOT(onAppendCommandToConsole(GcodeCommand*))
    );
    connect(
        &mGrblMachineModel, SIGNAL(appendResponseToConsoleSignal(const GrblResponse&)),
        mUi.consoleFormController, SLOT(onAppendResponseToConsole(const GrblResponse&))
    );
    connect(
        mUi.consoleFormController, SIGNAL(gcodeCommandSendSignal(GcodeCommand*)),
        &mGrblMachineModel, SLOT(onGcodeCommandManualSend(GcodeCommand*)));
}

void MainFormController::setupProgramFormSignals()
{
    // Program Form
    connect
    (
        mUi.programFormController, SIGNAL(sendProgramSignal()),
        this, SLOT(onSendProgram())
    );
    connect
    (
        mUi.programFormController, SIGNAL(sendProgramFromLineSignal(long)),
        this, SLOT(onSendProgramFromLine(long))
    );
    connect
    (
        &mGrblMachineModel,SIGNAL(machineStateUpdatedSignal(const GrblMachineState&)),
        this, SLOT(onMachineStateUpdated(const GrblMachineState&))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(updateProgramTableStatusSignal(GcodeCommand*)),
        mUi.programFormController, SLOT(onUpdateProgramTableStatus(GcodeCommand*))
    );
    connect(
        mUi.programFormController, SIGNAL(gcodeCommandManualSendSignal(GcodeCommand*)),
        &mGrblMachineModel, SLOT(onGcodeCommandManualSend(GcodeCommand*))
                );
}

void MainFormController::setupOverrideFormSignals()
{
    connect
    (
        mUi.overrideFormController, SIGNAL(gcodeCommandManualSendSignal(GcodeCommand*)),
        &mGrblMachineModel, SLOT(onGcodeCommandManualSend(GcodeCommand*))
    );
}

void MainFormController::onMachineStateUpdated(const GrblMachineState& state)
{
    onStatusTextUpdate(stateToString(state));
    switch (state)
    {
       case GrblMachineState::Unlocked:
            mUi.programFormController->setFormActive(true);
            mUi.consoleFormController->setFormActive(true);
            mUi.controlFormController->setFormActive(true);
            mUi.controlFormController->highlightClearAlarmReset(false);
            mUi.jogFormController->setFormActive(true);
            mUi.overrideFormController->setFormActive(true);
            mUi.stateFormController->setClass(StateClass::Warning);
            break;
        case GrblMachineState::Alarm:
        case GrblMachineState::Error:
        case GrblMachineState::Locked:
            mUi.consoleFormController->setFormActive(false);
            mUi.programFormController->setFormActive(false);
            mUi.controlFormController->setFormActive(false);
            mUi.controlFormController->highlightClearAlarmReset(true);
            mUi.jogFormController->setFormActive(false);
            mUi.overrideFormController->setFormActive(false);
            mUi.stateFormController->setClass(StateClass::Danger);
            mUi.controlFormController->setClearAlarmResetActive(true);
            break;
        case GrblMachineState::Unknown:
            mUi.stateFormController->setClass(StateClass::Primary);
            break;
        case GrblMachineState::Idle:
            if (!mGrblMachineModel.getProgramRunning())
            {
                mUi.programFormController->setFormActive(true);
                mUi.consoleFormController->setFormActive(true);
                mUi.controlFormController->setFormActive(true);
                mUi.controlFormController->highlightClearAlarmReset(false);
                mUi.jogFormController->setFormActive(true);
                mUi.overrideFormController->setFormActive(true);
            }
            mUi.stateFormController->setClass(StateClass::Info);
            break;
        case GrblMachineState::Run:
            if (mGrblMachineModel.getProgramRunning())
            {
                mUi.jogFormController->setFormActive(false);
                mUi.consoleFormController->setFormActive(false);
                mUi.controlFormController->setFormActive(false);
            }
            mUi.stateFormController->setClass(StateClass::Success);
            break;
        case GrblMachineState::Home:
            mUi.stateFormController->setClass(StateClass::Info);
            break;
        case GrblMachineState::Hold:
            mUi.stateFormController->setClass(StateClass::Warning);
            break;
        case GrblMachineState::Queue:
            mUi.stateFormController->setClass(StateClass::Warning);
            break;
        case GrblMachineState::Check:
            mUi.stateFormController->setClass(StateClass::Warning);
            break;
        case GrblMachineState::Door:
            mUi.stateFormController->setClass(StateClass::Danger);
            break;
        case GrblMachineState::Jog:
            mUi.consoleFormController->setFormActive(false);
            mUi.overrideFormController->setFormActive(false);
            mUi.stateFormController->setClass(StateClass::Warning);
            break;

    }
}

void MainFormController::onActionClearAllTriggered()
{
    int result = QMessageBox::warning
    (
        &mMainWindow,
        tr("Are you sure?"),
        tr("Any unsaved changes will be lost. Are you sure you want to continue?"),
        QMessageBox::Ok, QMessageBox::Cancel
    );

    switch (result)
    {
        case QMessageBox::Ok:
            //qDebug() << "MainFormController: Clearing all data";
            mUi.consoleFormController->initialise();
            mUi.controlFormController->initialise();
            mUi.jogFormController->initialise();
            mUi.programFormController->initialise();
            mUi.overrideFormController->initialise();
            mUi.stateFormController->initialise();
            mUi.visualisationFormController->initialise();
            mSettingsFormController.initialise();
            mGcodeFileModel.clear();
            mGcodeFileModel = QSharedPointer<GcodeFileModel>::create();
            mGrblMachineModel.initialise();
            break;
        case QMessageBox::Cancel:
            return;
    }
}

void MainFormController::setupControlFormSignals()
{
    connect(
        mUi.controlFormController, SIGNAL(gcodeCommandManualSendSignal(GcodeCommand*)),
        &mGrblMachineModel, SLOT(onGcodeCommandManualSend(GcodeCommand*))
    );
    // Safe Position Changed
    connect
    (
        mUi.controlFormController,SIGNAL(safePositionSetSignal()),
        mUi.visualisationFormController,SLOT(onSafePositionSetSignal())
    );

    connect
    (
        mUi.controlFormController,SIGNAL(uiLockOverrideSignal()),
        this,SLOT(onUiLockOverride())
    );
}

void MainFormController::onUiLockOverride()
{
    onMachineStateUpdated(GrblMachineState::Unlocked);
}

void MainFormController::onSendProgram()
{
    emit sendProgramSignal(mGcodeFileModel.data());
}

void MainFormController::onSendProgramFromLine(long line)
{
    emit sendProgramFromLineSignal(mGcodeFileModel.data(), line);
}

void MainFormController::onSerialPortError(QString error)
{
    /*
    QMessageBox::critical
    (
        &mMainWindow,
        tr("Serial Port Error!"),
        QString("Error: %1").arg(error),
        QMessageBox::Ok
    );
    */
}

void MainFormController::setupSignalSlots()
{
    //qDebug() << "MainFormController: Setup Signals/Slots";
    setupMenuBarSignals();
    setupToolbarSignals();
    setupSettingsModelSignals();
    setupRecentFilesModelsSignals();
    setupGrblMachineModelSignals();
    setupJogFormSignals();
    setupConsoleFormSignals();
    setupControlFormSignals();
    setupProgramFormSignals();
    setupStateFormSignals();
    setupVisualisationFormSignals();
    setupOverrideFormSignals();
}

void MainFormController::showMainWindow()
{
    //qDebug() << "MainFormController: showMainWindow ";
    auto geom = qApp->desktop()->availableGeometry();
    mMainWindow.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            geom.size(),
            geom
        )
    );
    mMainWindow.showMaximized();
    mMainWindow.raise();  // for MacOS
    mMainWindow.activateWindow(); // for Windows
}

bool MainFormController::saveChanges()
{
    //qDebug() << "MainFormController: saveChanges ";
    return true;
}

void MainFormController::setFormActive(bool active)
{

}

void MainFormController::onGrblMachineError(QString error)
{
    //qDebug() << "MainFormController: Got Error" << error;
    QMessageBox::critical
    (
        &mMainWindow,
        tr("Grbl Machine Error!"),
        QString("Grbl Error: %1").arg(error),
        QMessageBox::Ok
    );
}

void MainFormController::onJobCompleted()
{
    QMessageBox dialog(&mMainWindow);

    dialog.setWindowTitle(tr("Program Finished"));
    dialog.setText(QString("All operations have completed."));
    dialog.setStandardButtons(QMessageBox::Ok);
    auto icon = QPixmap(":/Images/SVG/thumbs-up.svg");
    dialog.setIconPixmap(icon.scaled(48,48));
    dialog.exec();

    onMachineStateUpdated(GrblMachineState::Unlocked);
}

void MainFormController::onActFileExitTriggered()
{
    //qDebug() << "MainFormController: onActFileExitTriggered";
    mMainWindow.close();
}

void MainFormController::onActSettingsTriggered()
{
    //qDebug() << "MainFormController: onActSettingsTriggered";
    mSettingsFormController.exec();
}

void MainFormController::onActFileOpenTriggered()
{
    //qDebug() << "MainFormController: onActFileOpenTriggered";
    if (!saveChanges())
    {
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open"),
                mLastFolder,
                tr("G-Code files (*.nc *.ncc *.ngc *.tap *.txt);;All files (*.*)")
                );

    //qDebug() << "MainFormController: Opening File " << fileName;

    mGcodeFileModel.clear();
    mGcodeFileModel = QSharedPointer<GcodeFileModel>::create();
    setupGcodeFileModelSignals();

    if (!fileName.isEmpty())
    {
        if (mGcodeFileModel->isGcodeFile(fileName))
        {
            //qDebug() << "MainFormController: Gcode file format";
            mGcodeFileModel->load(fileName);
            if (mRecentGcodeFilesModelHandle)
            {
                mRecentGcodeFilesModelHandle->add(RecentFile(fileName));
            }
        }
    }
}

void MainFormController::initialise()
{

}

void MainFormController::onGrblMachineConnected(bool connected)
{
    if (mGcodeFileModel)
    {
        mUi.programFormController->setFormActive(mGcodeFileModel->isOpen() && connected);
    }
    mUi.actionConnect->setEnabled(!connected);
}

void MainFormController::onActFileSaveTransformedAsTriggered()
{
    //qDebug() << "MainFormController: onActFileSaveTransformAsTriggered";
    QString fileName = (
                QFileDialog::getSaveFileName(
                    &mMainWindow,
                    tr("Save file as"),
                    mLastFolder,
                    tr("G-Code files (*.nc *.ncc *.ngc *.tap *.txt)")
                    )
                );
}

void MainFormController::onActFileSaveAsTriggered()
{
    //qDebug() << "MainFormController: onActFileSaveAsTriggered";
}

void MainFormController::onActFileSaveTriggered()
{
    //qDebug() << "MainFormController: onActFIleSaveTriggered";
}

void MainFormController::onActAboutTriggered()
{
    //qDebug() << "MainFormController: onActAboutTriggered";
    mAboutFormController.exec();
}

void MainFormController::onActRecentFileTriggered()
{
    //qDebug() << "MainFormController: onActRecentFileTriggered";
}

void MainFormController::updateRecentFilesMenu()
{
    //qDebug() << "MainFormController: updateRecentFilesMenu";
}

void MainFormController::onRecentGcodeFilesChanged()
{
    //qDebug() << "MainFormController: onRecentGerberFilesChanged";
    clearRecentGcodeFilesMenu();
    populateRecentGcodeFilesMenu();
}

void MainFormController::onStatusTextUpdate(QString status)
{
    mUi.stateFormController->onStatusTextUpdate(status);
}

void MainFormController::onStatusBarUpdate(QString status)
{
    mUi.statusBar->showMessage(status);
}

void MainFormController::onGcodeFileLoadStarted()
{
    //qDebug() << "MainFormController: onGcodeFileLoadStarted";
}

void MainFormController::onGcodeFileLoadFinished(GcodeFileModel* items)
{
    Q_UNUSED(items)
    //qDebug() << "MainFormController: onGcodeFileLoadFinished";
    QString fileName =  mGcodeFileModel->getCurrentFileName();

    onStatusBarUpdate
    (
        QString("Opened Gcode File " +fileName)
    );

    mMainWindow.setWindowTitle("CoconutCNC - " + fileName);
    mMainWindow.setWindowFilePath(fileName); // For macOS

    if (mGrblMachineModel.isPortOpen())
    {
        mUi.programFormController->setFormActive(true);
    }
}

void MainFormController::populateRecentGcodeFilesMenu()
{
    //qDebug() << "MainFormController: populateRecentGcodeFilesMenu";
    foreach (RecentFile file, mRecentGcodeFilesModelHandle->getRecentFiles())
    {
        QAction *action = new QAction(file.getPath(), this);
        connect(action, SIGNAL(triggered()), this, SLOT(onActRecentFileTriggered()));
        mUi.recentGcodeFilesMenu->insertAction
        (
            mUi.recentGcodeFilesMenu->actions()[0], action
        );
    }
}

void MainFormController::clearRecentGcodeFilesMenu()
{
    //qDebug() << "MainFormController: clearRecentGcodeFilesMenu";
    foreach (QAction * action, mUi.recentGcodeFilesMenu->actions())
    {
        if (action->text() == "")
        {
            break;
        }
        else
        {
            mUi.recentGcodeFilesMenu->removeAction(action);
            delete action;
        }
    }
}

void MainFormController::onSetBufferProgressValue(int value)
{
    mBufferProgressBar.setValue(value);
}

void MainFormController::onSetCompletionProgressValue(int value)
{
    mCompletionProgressBar.setValue(value);
}

void MainFormController::setupCompletionAndBufferProgressBars()
{
    mUi.statusBar->addPermanentWidget(new QLabel("Progress"));
    mCompletionProgressBar.setMinimum(0);
    mCompletionProgressBar.setMaximum(100);
    mCompletionProgressBar.setTextVisible(false);
    mCompletionProgressBar.setMinimumWidth(200);
    mCompletionProgressBar.setMaximumWidth(200);
    mUi.statusBar->addPermanentWidget(&mCompletionProgressBar);
    mUi.statusBar->addPermanentWidget(new QLabel("Buffer Usage"));
    mBufferProgressBar.setMinimum(0);
    mBufferProgressBar.setMaximum(100);
    mBufferProgressBar.setTextVisible(false);
    mBufferProgressBar.setMinimumWidth(200);
    mBufferProgressBar.setMaximumWidth(200);
    mUi.statusBar->addPermanentWidget(&mBufferProgressBar);
}

void MainFormController::onStopTriggered(bool checked)
{
    // in resume state
    if (checked)
    {
        mGrblMachineModel.onGcodeCommandManualSend(GcodeCommand::FeedHoldCommand());
        mUi.actionStop->setText("Resume");
        mUi.actionStop->setIcon(QIcon(":/Images/SVG/thumbs-up.svg"));
    }
    // in feed hold mode
    else
    {
        mGrblMachineModel.onGcodeCommandManualSend(GcodeCommand::CyclePauseResumeCommand());
        mUi.actionStop->setText("Feed Hold");
        mUi.actionStop->setIcon(QIcon(":/Images/SVG/hand-paper.svg"));
    }
}

void MainFormController::onToolChange(int tool)
{
    // TODO - get tool info from settings model
    Tool* requestedTool = mSqlSettingsModel->getToolByNumber(tool);
    QString toolStr;
    if (requestedTool)
    {
       toolStr = "("+QString::number(tool)+") "+requestedTool->getName();
    }
    else
    {
        toolStr = QString::number(tool);
    }
    QMessageBox dialog(&mMainWindow);

    dialog.setWindowTitle(tr("Tool Change"));
    dialog.setText(QString("Tool Change Detected.\n\nPlease change to tool %1 and click 'Ok' to proceed.").arg(toolStr));
    dialog.setStandardButtons(QMessageBox::Ok);
    auto icon = QPixmap(":/Images/SVG/drilling-machine.svg");
    dialog.setIconPixmap(icon.scaled(48,48));

    auto result = dialog.exec();

    if (result == QMessageBox::Ok)
    {
        emit toolChangeCompletedSignal();
        mUi.stateFormController->setCurrentTool(requestedTool);
    }
}
