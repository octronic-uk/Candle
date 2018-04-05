// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

//#define INITTIME //QTime time; time.start();
//#define PRINTTIME(x) //qDebug() << "time elapse" << QString("%1:").arg(x) << time.elapsed(); time.start();


#define PROGRESS_MIN_LINES 10000
#define PROGRESS_STEP     1000

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

#include "Model/GrblMachineState.h"
#include "AbstractFormController.h"
#include "MainFormController.h"
#include "ui_MainForm.h"
#include "Model/Parser/GcodeParser.h"

MainFormController::MainFormController(QWidget *parent) :
    AbstractFormController(parent),
    mFormMode(MainFormMode::Idle),
    mLastFolder(QDir::homePath()),
    mOldStatusBarLayout(nullptr)
{
    qDebug() << "MainFormController: Constructing";
    mUi.setupUi(&mMainWindow);
    setupToolbarActions();
    setupCompletionAndBufferProgressBars();
    mGcodeFileModel = QSharedPointer<GcodeFileModel>::create(this);
    setupSignalSlots();

    mUi.splitter->setSizes(QList<int>() << 200 << 200);

    // Handle file drop
    if (qApp->arguments().count() > 1 && mGcodeFileModel->isGcodeFile(qApp->arguments().last()))
    {
        mGcodeFileModel->load(qApp->arguments().last());
    }

    mSettingsModel.onLoadSettings();
}

MainFormController::~MainFormController()
{
    qDebug() << "MainFormController: Destructing";
    mSettingsModel.onSaveSettings();
}

void MainFormController::setupToolbarActions()
{
    QAction *console = mUi.consoleDockWidget->toggleViewAction();
    console->setIcon(QIcon(":/Images/SVG/console.svg"));
    mUi.toolBar->addAction(console);

    QAction *control = mUi.controlDockWidget->toggleViewAction();
    control->setIcon(QIcon(":/Images/SVG/origin.svg"));
    mUi.toolBar->addAction(control);

    QAction *feed = mUi.feedDockWidget->toggleViewAction();
    feed->setIcon(QIcon(":/Images/SVG/feed_rate.svg"));
    mUi.toolBar->addAction(feed);

    QAction *jog = mUi.jogDockWidget->toggleViewAction();
    jog->setIcon(QIcon(":/Images/SVG/crosshairs.svg"));
    mUi.toolBar->addAction(jog);

    QAction *spindle = mUi.spindleDockWidget->toggleViewAction();
    spindle->setIcon(QIcon(":/Images/SVG/dot-circle.svg"));
    mUi.toolBar->addAction(spindle);

    QAction *state = mUi.stateDockWidget->toggleViewAction();
    state->setIcon(QIcon(":/Images/SVG/eye.svg"));
    mUi.toolBar->addAction(state);

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
}

void MainFormController::setupSettingsModelSignals()
{
    // Settings Model to Settings Form
    connect(
        &mSettingsModel, SIGNAL(settingChangedSignal(QString, QString, QVariant)),
        &mSettingsFormController, SLOT(onSettingChanged(QString, QString, QVariant))
    );
    // Settings Form to Settings Model
    connect(
        &mSettingsFormController, SIGNAL(settingChangedSignal(QString, QString, QVariant)),
        &mSettingsModel, SLOT(onSettingChanged(QString, QString, QVariant))
    );
    // Settings Model to SerialPort Model
    connect(
        &mSettingsModel, SIGNAL(settingChangedSignal(QString, QString, QVariant)),
        &mGrblMachineModel, SLOT(onSettingChanged(QString, QString, QVariant))
    );
}

void MainFormController::setupMenuBarSignals()
{
    // About
    connect(
        mUi.actAbout, SIGNAL(triggered()),
        this, SLOT(onActAboutTriggered())
    );
    // File->New
    connect(
                mUi.actFileNew, SIGNAL(triggered()),
                this, SLOT(onActFileNewTriggered())
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
}

void MainFormController::setupHeightMapFileModelSignals()
{
    // On File Load Handlers
    connect(
                &mHeightMapFileModel,SIGNAL(heightMapFileLoadStartedSignal()),
                this, SLOT(onHeightMapFileLoadStarted())
                );
    connect(
                &mHeightMapFileModel,SIGNAL(heightMapFileLoadFinishedSignal()),
                this, SLOT(onHeightMapFileLoadFinished())
                );
    // HeightMap File Model
    connect(
                &mHeightMapFileModel, SIGNAL(statusBarUpdateSignal(QString)),
                this, SLOT(onStatusBarUpdate(QString))
                );
}

void MainFormController::setupGcodeFileModelSignals()
{
    qDebug() << "MainFormController: setupGcodeFileModelSignals";
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
                mGcodeFileModel.data(), SIGNAL(gcodeFileLoadFinishedSignal(QList<GcodeCommand*>&)),
                mUi.programFormController, SLOT(onGcodeFileLoadFinished(QList<GcodeCommand*>&))
                );
    connect(
                mGcodeFileModel.data(), SIGNAL(gcodeFileLoadFinishedSignal(QList<GcodeCommand*>&)),
                mUi.visualisationFormController, SLOT(onGcodeFileLoadFinished(QList<GcodeCommand*>&))
                );
    connect(
            mGcodeFileModel.data(), SIGNAL(gcodeFileLoadFinishedSignal(QList<GcodeCommand*>&)),
            this, SLOT(onGcodeFileLoadFinished(QList<GcodeCommand*>&))
    );
    // Gcode Loading Finished
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
        mGcodeFileModel.data(), SIGNAL(gcodeParserUpdatedSignal(QSharedPointer<GcodeParser>)),
        mUi.visualisationFormController, SLOT(onGcodeParserUpdated(QSharedPointer<GcodeParser>))
    );
}

void MainFormController::setupRecentFilesModelsSignals()
{
    // Recent files changed handlers
    connect(
                &mRecentFilesModel, SIGNAL(recentFilesChangedSignal()),
                this, SLOT(onRecentGcodeFilesChanged())
                );
    connect(
                &mRecentHeightMapFilesModel, SIGNAL(recentFilesChangedSignal()),
                this, SLOT(onRecentHeightMapFilesChanged())
                );
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
        &mGrblMachineModel, SIGNAL(statusTextUpdateSignal(QString,QColor,QColor)),
        mUi.stateFormController, SLOT(onStatusTextUpdate(QString,QColor,QColor))
    );
    connect
    (
        this, SIGNAL(sendProgramSignal(QSharedPointer<GcodeFileModel>)),
        &mGrblMachineModel, SLOT(onSendProgram(QSharedPointer<GcodeFileModel>))
    );
    connect
    (
        this, SIGNAL(sendProgramFromLineSignal(QSharedPointer<GcodeFileModel>,long)),
        &mGrblMachineModel, SLOT(onSendProgramFromLine(QSharedPointer<GcodeFileModel>,long))
    );
    connect
    (
        &mGrblMachineModel, SIGNAL(jobCompletedSignal()),
        this, SLOT(onJobCompleted())
    );
}

void MainFormController::setupJogFormSignals()
{
    // Jog Form
    connect(
        mUi.jogFormController, SIGNAL(statusBarUpdateSignal(QString)),
        this, SLOT(onStatusBarUpdate(QString))
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
}

void MainFormController::onMachineStateUpdated(const GrblMachineState& state)
{
   switch (state)
   {
       case GrblMachineState::Unlocked:
            mUi.programFormController->setFormActive(true);
            mUi.controlFormController->highlightUnlock(false);
            break;
        case GrblMachineState::Locked:
            mUi.programFormController->setFormActive(false);
            mUi.controlFormController->setFormActive(false);
            mUi.controlFormController->highlightUnlock(true);
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
            mUi.consoleFormController->initialise();
            mUi.controlFormController->initialise();
            mUi.feedFormController->initialise();
            mUi.jogFormController->initialise();
            mUi.programFormController->initialise();
            mUi.spindleFormController->initialise();
            mUi.stateFormController->initialise();
            mUi.visualisationFormController->initialise();
            mSettingsFormController.initialise();
            mGcodeFileModel->initialise();
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
}

void MainFormController::onSendProgram()
{
    onSetFormMode(MainFormMode::RunningGerber);
    emit sendProgramSignal(mGcodeFileModel);
}

void MainFormController::onSendProgramFromLine(long line)
{
    onSetFormMode(MainFormMode::RunningGerber);
    emit sendProgramFromLineSignal(mGcodeFileModel, line);
}

void MainFormController::onSerialPortError(QString error)
{

}

void MainFormController::setupSignalSlots()
{
    qDebug() << "MainFormController: Setup Signals/Slots";
    setupMenuBarSignals();
    setupToolbarSignals();
    setupSettingsModelSignals();
    setupGcodeFileModelSignals();
    setupHeightMapFileModelSignals();
    setupRecentFilesModelsSignals();
    setupGrblMachineModelSignals();
    setupJogFormSignals();
    setupConsoleFormSignals();
    setupControlFormSignals();
    setupProgramFormSignals();
}

void MainFormController::showMainWindow()
{
    qDebug() << "MainFormController: showMainWindow ";
    mMainWindow.show();
}

bool MainFormController::saveChanges(bool heightMapMode)
{
    qDebug() << "MainFormController: saveChanges ";
    return true;
}

void MainFormController::setFormActive(bool active)
{

}

void MainFormController::onSetFormMode(MainFormMode mode)
{
    qDebug() << "MainFormController: setFormMode";
    mFormMode = mode;
    switch (mFormMode)
    {
        case MainFormMode::Idle:
            mUi.jogFormController->setFormActive(true);
            mUi.consoleFormController->setFormActive(true);
            mUi.controlFormController->setFormActive(true);
            break;
        case MainFormMode::Gerber:
            break;
        case MainFormMode::HeightMap:
            break;
        case MainFormMode::RunningGerber:
            mUi.jogFormController->setFormActive(false);
            mUi.consoleFormController->setFormActive(false);
            mUi.controlFormController->setFormActive(false);
            break;
    }
}

void MainFormController::onJobCompleted()
{
   mUi.controlFormController->setFormActive(true);
   mUi.jogFormController->setFormActive(true);
}

void MainFormController::resizeEvent(QResizeEvent *re)
{
    Q_UNUSED(re)
    qDebug() << "MainFormController: resizeEvent";

    mUi.visualisationFormController->placeVisualizerButtons();
}

void MainFormController::closeEvent(QCloseEvent *ce)
{
    Q_UNUSED(ce)
    qDebug() << "MainFormController: closeEvent";
    bool mode = mHeightMapMode;
    mHeightMapMode = false;
}

void MainFormController::dragEnterEvent(QDragEnterEvent *dee)
{
    Q_UNUSED(dee)
    qDebug() << "MainFormController: dragEnterEvent";
}

void MainFormController::dropEvent(QDropEvent *de)
{
    Q_UNUSED(de)
    qDebug() << "MainFormController: dropEvent";
}

void MainFormController::onActFileExitTriggered()
{
    qDebug() << "MainFormController: onActFileExitTriggered";
    mMainWindow.close();
}

void MainFormController::onActSettingsTriggered()
{
    qDebug() << "MainFormController: onActSettingsTriggered";
    if(mSettingsFormController.exec())
    {
        qDebug() << "MainFormController: Settings should be applied here";
    }
}

bool buttonLessThan(QPushButton *b1, QPushButton *b2)
{
    qDebug() << "MainFormController: buttonLessThan";
    return b1->text().toDouble() < b2->text().toDouble();
}

void MainFormController::onActFileOpenTriggered()
{
    qDebug() << "MainFormController: onActFileOpenTriggered";
    if (!saveChanges(false))
    {
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open"),
                mLastFolder,
                tr("G-Code files (*.nc *.ncc *.ngc *.tap *.txt);;Heightmap files (*.map);;All files (*.*)")
                );

    qDebug() << "MainFormController: Opening File " << fileName;


    if (!fileName.isEmpty())
    {
        if (mHeightMapFileModel.isHeightMapFile(fileName))
        {
            qDebug() << "MainFormController: HeightMap file format";
            mHeightMapFileModel.load(fileName);
            mRecentHeightMapFilesModel.add(fileName);
        }
        else if (mGcodeFileModel->isGcodeFile(fileName))
        {
            qDebug() << "MainFormController: Gcode file format";
            mGcodeFileModel->load(fileName);
            mRecentFilesModel.add(fileName);
        }
        else
        {
            qDebug() << "MainFormController: Unrecognised file format";
        }
    }
}

bool MainFormController::isInHeightMapMode()
{
    qDebug() << "MainFormController: isInHeightMapMode";
    return mFormMode == MainFormMode::HeightMap;
}

bool MainFormController::isInGerberMode()
{
    qDebug() << "MainFormController: isInGerberMode";
    return mFormMode == MainFormMode::Gerber;
}

void MainFormController::initialise()
{

}

void MainFormController::onActFileNewTriggered()
{
    qDebug() << "MainFormController: onActFileNewTriggered";
    qDebug() << "MainFormController: changes:" << mGcodeFileModel->hasFileChanged()
             << mHeightMapFileModel.hasFileChanged();
}

void MainFormController::onActFileSaveTransformedAsTriggered()
{
    qDebug() << "MainFormController: onActFileSaveTransformAsTriggered";
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
    qDebug() << "MainFormController: onActFileSaveAsTriggered";
}

void MainFormController::onActFileSaveTriggered()
{
    qDebug() << "MainFormController: onActFIleSaveTriggered";
}

void MainFormController::onActAboutTriggered()
{
    qDebug() << "MainFormController: onActAboutTriggered";
    mAboutFormController.exec();
}

void MainFormController::onHeightMapFileLoadStarted()
{
    qDebug() << "MainFormController: onHeightMapFileLoadStarted";

}

void MainFormController::onHeightMapFileLoadFinished()
{
    qDebug() << "MainFormController: onHeightMapFileLoadFinished";
    onSetFormMode(MainFormMode::HeightMap);
    onStatusBarUpdate
    (
        QString("Opened HeightMap File %s")
            .arg(mGcodeFileModel->getCurrentFileName())
    );
}

void MainFormController::showEvent(QShowEvent* se)
{

    Q_UNUSED(se)
    qDebug() << "MainFormController: showEvent";

}

void MainFormController::hideEvent(QHideEvent* he)
{

    Q_UNUSED(he)
    qDebug() << "MainFormController: hideEvent";

}

bool MainFormController::eventFilter(QObject *obj, QEvent *event)
{
    qDebug() << "MainFormController: eventFilter";
    return mMainWindow.eventFilter(obj, event);
}

void MainFormController::onActRecentFileTriggered()
{
    qDebug() << "MainFormController: onActRecentFileTriggered";
}

void MainFormController::updateRecentFilesMenu()
{
    qDebug() << "MainFormController: updateRecentFilesMenu";
}

void MainFormController::onRecentHeightMapFilesChanged()
{
    qDebug() << "MainFormController: onRecentHeightMapFilesChanged";
    clearRecentHeightMapFilesMenu();
    populateRecentHeightMapFilesMenu();
}

void MainFormController::onRecentGcodeFilesChanged()
{
    qDebug() << "MainFormController: onRecentGerberFilesChanged";
    clearRecentGcodeFilesMenu();
    populateRecentGcodeFilesMenu();
}

void MainFormController::onStatusTextUpdate(QString status)
{
    mUi.stateFormController->onStatusTextUpdate(status);
}

void MainFormController::onStatusBarUpdate(QString status)
{
    qDebug() << "MainFormController: onStatusUpdate";
    mUi.statusBar->showMessage(status);
}

void MainFormController::onGcodeFileLoadStarted()
{
    qDebug() << "MainFormController: onGcodeFileLoadStarted";
}

void MainFormController::onGcodeFileLoadFinished(QList<GcodeCommand*>& items)
{
    Q_UNUSED(items)
    qDebug() << "MainFormController: onGcodeFileLoadFinished";
    onSetFormMode(MainFormMode::Gerber);
    onStatusBarUpdate
    (
        QString("Opened Gcode File " + mGcodeFileModel->getCurrentFileName())
    );
}

void MainFormController::populateRecentGcodeFilesMenu()
{
    qDebug() << "MainFormController: populateRecentGcodeFilesMenu";
    foreach (QString file, mRecentFilesModel.getRecentFiles())
    {
        QAction *action = new QAction(file, this);
        connect(action, SIGNAL(triggered()), this, SLOT(onActRecentFileTriggered()));
        mUi.recentGcodeFilesMenu->insertAction(
                    mUi.recentGcodeFilesMenu->actions()[0], action
                );
    }
}

void MainFormController::populateRecentHeightMapFilesMenu()
{
    qDebug() << "MainFormController: populateRecentHeightMapFilesMenu";
    foreach (QString file, mRecentHeightMapFilesModel.getRecentFiles())
    {
        QAction *action = new QAction(file, this);
        connect(action, SIGNAL(triggered()), this, SLOT(onActRecentFileTriggered()));
        mUi.recentHeightMapFilesMenu->insertAction(
                    mUi.recentHeightMapFilesMenu->actions()[0], action
                );
    }
}

void MainFormController::clearRecentGcodeFilesMenu()
{
    qDebug() << "MainFormController: clearRecentGcodeFilesMenu";
    foreach (QAction * action, mUi.recentHeightMapFilesMenu->actions())
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

void MainFormController::clearRecentHeightMapFilesMenu()
{
    qDebug() << "MainFormController: clearRecentHeightMapFilesMenu";
    foreach (QAction * action, mUi.recentGcodeFilesMenu->actions())
    {
        if (action->text() == "")
        {
            break;
        }
        else
        {
            mUi.recentHeightMapFilesMenu->removeAction(action);
            delete action;
        }
    }
}

void MainFormController::onSetBufferProgressValue(int value)
{
    qDebug() << "MainFormController: setBufferProgressValue" << value;
    mBufferProgressBar.setValue(value);
}

void MainFormController::onSetCompletionProgressValue(int value)
{
    qDebug() << "MainFormController: setCompletionProgressValue" << value;
    mCompletionProgressBar.setValue(value);
}

void MainFormController::setupCompletionAndBufferProgressBars()
{
    mUi.statusBar->addPermanentWidget(new QLabel("Progress"));
    mCompletionProgressBar.setMinimum(0);
    mCompletionProgressBar.setMaximum(100);
    mCompletionProgressBar.setTextVisible(false);
    mUi.statusBar->addPermanentWidget(&mCompletionProgressBar);
    mUi.statusBar->addPermanentWidget(new QLabel("Buffer"));
    mBufferProgressBar.setMinimum(0);
    mBufferProgressBar.setMaximum(100);
    mBufferProgressBar.setTextVisible(false);
    mUi.statusBar->addPermanentWidget(&mBufferProgressBar);
}
