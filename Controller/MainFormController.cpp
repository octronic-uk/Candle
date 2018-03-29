// This file is a part of "Cocoanut" application.
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

#include "Utils/MachineState.h"
#include "AbstractFormController.h"
#include "MainFormController.h"
#include "ui_MainForm.h"

MainFormController::MainFormController(QWidget *parent) :
    AbstractFormController(parent),
    mFormMode(MAIN_FORM_MODE_NONE),
    mLastFolder(QDir::homePath())
{
    qDebug() << "MainFormController: Constructing";
    mUi.setupUi(&mMainWindow);
    setupToolbarActions();
    setupSignalSlots();

    mFileProcessedCommandIndex = 0;
    mCellChanged = false;
    mIsTransferCompleted = true;

    /*
    foreach (QPushButton* button, mUi.findChildren<StyledToolButton*>(QRegExp("cmdUser\\d")))
    {
        connect(button, SIGNAL(clicked(bool)), this, SLOT(onCmdUserClicked(bool)));
    }
    */

   /* mSenderErrorBox.create(
        QMessageBox::Warning,
        qApp->applicationDisplayName(),
        QString(),
        QMessageBox::Ignore | QMessageBox::Abort,
        &mMainWindow
    );
    */

    //mSenderErrorBox->setCheckBox(new QCheckBox(tr("Don't show again")));

    // Loading settings
    mSettingsModel.load();

    //mControlFormController->updateControlsState();

    mConnectionTimer.start(1000);
    mStateQueryTimer.start();

    // Handle file drop
    if (qApp->arguments().count() > 1 && mGcodeFileModel.isGcodeFile(qApp->arguments().last()))
    {
        mGcodeFileModel.load(qApp->arguments().last());
    }
}

MainFormController::~MainFormController()
{
    qDebug() << "MainFormController: Destructing";
    mSettingsModel.save();
}

void MainFormController::setupToolbarActions()
{
    QAction *console = mUi.consoleDockWidget->toggleViewAction();
    console->setIcon(QIcon(":/Images/info-circle-outline/32x32.png"));
    mUi.toolBar->addAction(console);

    QAction *control = mUi.controlDockWidget->toggleViewAction();
    control->setIcon(QIcon(":/Images/origin.png"));
    mUi.toolBar->addAction(control);

    QAction *feed = mUi.feedDockWidget->toggleViewAction();
    feed->setIcon(QIcon(":/Images/percent-circle-outline/256x256.png"));
    mUi.toolBar->addAction(feed);

    QAction *jog = mUi.jogDockWidget->toggleViewAction();
    jog->setIcon(QIcon(":/Images/chevron-circle-outline/32x32 right.png"));
    mUi.toolBar->addAction(jog);

    QAction *spindle = mUi.spindleDockWidget->toggleViewAction();
    spindle->setIcon(QIcon(":/Images/cutter.png"));
    mUi.toolBar->addAction(spindle);

    QAction *state = mUi.stateDockWidget->toggleViewAction();
    state->setIcon(QIcon(":/Images/eye-outline/256x256.png"));
    mUi.toolBar->addAction(state);

    QAction *userCommands = mUi.userCommandsDockWidget->toggleViewAction();
    userCommands->setIcon(QIcon(":/Images/num1.png"));
    mUi.toolBar->addAction(userCommands);

}

void MainFormController::setupSignalSlots()
{
    qDebug() << "MainFormController: Setup Signals/Slots";
    // Preferences
    connect(
        mUi.actFileSettings, SIGNAL(triggered()),
        this, SLOT(onActSettingsTriggered())
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
    // On File Load Handlers
    connect(
        &mHeightMapFileModel,SIGNAL(heightMapFileLoadStartedSignal()),
        this, SLOT(onHeightMapFileLoadStarted())
    );
    connect(
        &mHeightMapFileModel,SIGNAL(heightMapFileLoadFinishedSignal()),
        this, SLOT(onHeightMapFileLoadFinished())
    );
    // Recent files changed handlers
    connect(
        &mRecentFilesModel, SIGNAL(recentFilesChangedSignal()),
        this, SLOT(onRecentGcodeFilesChanged())
    );
    connect(
        &mRecentHeightMapFilesModel, SIGNAL(recentFilesChangedSignal()),
        this, SLOT(onRecentHeightMapFilesChanged())
    );
    // Serial Port
    connect(
        &mSerialPortModel, SIGNAL(serialPortErrorSignal(QString)),
        this, SLOT(onSerialPortError(QString))
    );
    connect(
        &mSerialPortModel, SIGNAL(statusUpdateSignal(QString)),
        this, SLOT(onStatusUpdate(QString))
    );
    // Gcode File Model
    connect(
        &mGcodeFileModel, SIGNAL(statusUpdateSignal(QString)),
        this, SLOT(onStatusUpdate(QString))
    );
//    connect(
//        &mGcodeFileModel, SIGNAL(gcodeFileLoadStartedSignal()),
//        this, SLOT(onGcodeFileLoadStarted())
//    );
    connect(
        &mGcodeFileModel, SIGNAL(reserveGcodeRowsSignal(int)),
        mUi.programForm, SLOT(onReserveGcodeRowsSignal(int))
    );
    connect(
        &mGcodeFileModel, SIGNAL(gcodeFileLoadStartedSignal()),
        mUi.programForm, SLOT(onGcodeFileLoadStarted())
    );
    connect(
        &mGcodeFileModel, SIGNAL(gcodeFileLoadFinishedSignal(QList<GcodeItem>)),
        mUi.programForm, SLOT(onGcodeFileLoadFinished(QList<GcodeItem>))
    );
    // HeightMap File Model
    connect(
        &mHeightMapFileModel, SIGNAL(statusUpdateSignal(QString)),
        this, SLOT(onStatusUpdate(QString))
    );
    // Timers
    /*
    connect(
        &mConnectionTimer, SIGNAL(timeout()),
        this, SLOT(onTimerConnection())
    );
    connect(
        &mStateQueryTimer, SIGNAL(timeout()),
        this, SLOT(onTimerStateQuery())
    );
    */
    // Console Form
    connect(
        mUi.consoleFormController, SIGNAL(commandSentSignal(QString,int)),
        this, SLOT(onCommandSent(QString,int))
    );
}

void MainFormController::showMainWindow()
{
    qDebug() << "MainFormController: showMainWindow ";
    mMainWindow.show();
}

bool MainFormController::saveChanges(bool heightMapMode)
{
    qDebug() << "MainFormController: saveChanges ";
    /*if ((!heightMapMode && mGcodeFileModel.hasChanged()))
    {
        int res = QMessageBox::warning(
            &mMainWindow,
            mMainWindow.windowTitle(),
            tr("G-code program file was changed. Save?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
        );

        if (res == QMessageBox::Cancel)
        {
            return false;
        }
        else if (res == QMessageBox::Yes)
        {
            onActFileSaveTriggered();
        }
        mGcodeFileModel.setFileChanged(false);
    }

    if (mHeightMapFileModel.hasFileChanged())
    {
        int res = QMessageBox::warning(
            &mMainWindow,
            mMainWindow.windowTitle(),
            tr("Heightmap file was changed. Save?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (res == QMessageBox::Cancel)
        {
            return false;
        }
        else if (res == QMessageBox::Yes)
        {
            mHeightMapMode = true;
            onActFileSaveTriggered();
            mHeightMapMode = heightMapMode;
            updateRecentFilesMenu(); // Restore g-code files recent menu
        }

        mHeightMapFileModel.setFileChanged(false);
    }

    */
    return true;
}

int MainFormController::bufferLength()
{
    qDebug() << "MainFormController: bufferLength";
    int length = 0;

    foreach (CommandAttributes ca, mCommandsList)
    {
        length += ca.length;
    }

    return length;
}

void MainFormController::setFormMode(MainFormMode mode)
{
    qDebug() << "MainFormController: setFormMode";
   mFormMode = mode;
}

void MainFormController::onTimerConnection()
{
    qDebug() << "MainFormController: onTimerConnection";
    /*
    if  (
            !mControlFormController->isHoming() // && !m_reseting
            &&
            !mProgramFormController->isPauseChecked() &&
            mCommandsQueue.length() == 0
        ) {
        if (mSpindleFormController->isUpdatingSpindleSpeed())
        {
            mSpindleFormController->setUpdatingSpindleSpeed(false);
            mSerialPortModel.sendCommand(
                QString("S%1").arg(mSpindleFormController->getSpindleSpeed()),
                -2, mSettingsFormController->showUICommands()
            );
        }
        if (mIsUpdatingParserStatus)
        {
            mIsUpdatingParserStatus = false;
            mSerialPortModel.sendCommand("$G", -3, false);
        }
        if (mFeedFormController->isUpdatingFeed())
        {
            mFeedFormController->setUpdatingFeed(false);
            double originalFeedRate = mFeedFormController->getOriginalFeedRate();
            mSerialPortModel.sendCommand(
                QString("F%1").arg(
                    mFeedFormController->isFeedOverrideChecked() ?
                    originalFeedRate / 100 * mFeedFormController->getTxtFeedValue() :
                    originalFeedRate
                ),
                -1,
                mSettingsFormController->showUICommands()
            );
        }
    }
    */
}

void MainFormController::onTimerStateQuery()
{
    qDebug() << "MainFormController: onTImerStateQuery";
    /*
    if (mSerialPortModel.isPortOpen() &&
        mControlFormController->isResetCompleted() &&
        mSerialPortModel.isStatusReceived()) {
        mSerialPortModel.write(QByteArray(1, '?'));
        mSerialPortModel.setStatusReceived(false);
    }

    mVisualisationFormController->setGLWBufferState(
        QString(tr("Buffer: %1 / %2")).arg(
            bufferLength()).arg(mCommandsQueue.length()
        )
    );
    */
}

void MainFormController::resizeEvent(QResizeEvent *re)
{
    Q_UNUSED(re)
    qDebug() << "MainFormController: resizeEvent";
    /*

    mVisualisationFormController->placeVisualizerButtons();
    resizeCheckBoxes();
    mHeightMapFormController->resizeTableHeightMapSections();
    */
}

void MainFormController::timerEvent(QTimerEvent*)
{
    qDebug() << "MainFormController: timerEvent";

}

void MainFormController::resizeCheckBoxes()
{
    qDebug() << "MainFormController: resizeCheckBoxes";
 /*   static int widthCheckMode = mProgramFormController->getChkTestModeWidth();
    static int widthAutoScroll = mProgramFormController->getChkAutoScrollWidth();

    // Transform checkboxes

    mMainWindow->setUpdatesEnabled(false);

    updateLayouts();

    if (mUi.chkTestMode->sizeHint().width() > mUi.chkTestMode->width())
    {
        widthCheckMode = mUi.chkTestMode->sizeHint().width();
        mUi.chkTestMode->setText(tr("Check"));
        mUi.chkTestMode->setMinimumWidth(mUi.chkTestMode->sizeHint().width());
        updateLayouts();
    }

    if (mUi.chkAutoScroll->sizeHint().width() > mUi.chkAutoScroll->width()
        && mUi.chkTestMode->text() == tr("Check"))
    {
        widthAutoScroll = mUi.chkAutoScroll->sizeHint().width();
        mUi.chkAutoScroll->setText(tr("Scroll"));
        mUi.chkAutoScroll->setMinimumWidth(mUi.chkAutoScroll->sizeHint().width());
        updateLayouts();
    }

    if (mUi.spacerBot->geometry().width() + mUi.chkAutoScroll->sizeHint().width()
        - mUi.spacerBot->sizeHint().width() > widthAutoScroll && mUi.chkAutoScroll->text() == tr("Scroll"))
    {
        mUi.chkAutoScroll->setText(tr("Autoscroll"));
        updateLayouts();
    }

    if (mUi.spacerBot->geometry().width() + mUi.chkTestMode->sizeHint().width()
        - mUi.spacerBot->sizeHint().width() > widthCheckMode && mUi.chkTestMode->text() == tr("Check"))
    {
        mUi.chkTestMode->setText(tr("Check mode"));
        updateLayouts();
    }

    this->setUpdatesEnabled(true);
    this->repaint();*/
}

void MainFormController::closeEvent(QCloseEvent *ce)
{
    Q_UNUSED(ce)
    qDebug() << "MainFormController: closeEvent";
    /*
    bool mode = mHeightMapMode;
    mHeightMapMode = false;

    if (!saveChanges(mHeightMapMode))
    {
        ce->ignore();
        mHeightMapMode = mode;
        return;
    }

    if (mIsProcessingFile &&
        QMessageBox::warning(
            &mMainWindow,
            mMainWindow.windowTitle(),
            tr("File sending in progress. Terminate and exit?"),
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    {
        ce->ignore();
        mHeightMapFormController->setHeightMapMode(mode);
        return;
    }

    if (mSerialPortModel.isPortOpen())
    {
        mSerialPortModel.closePort();
    }

    if (mCommandsQueue.length() > 0)
    {
        mCommandsList.clear();
        mCommandsQueue.clear();
    }
    */
}

void MainFormController::dragEnterEvent(QDragEnterEvent *dee)
{
    Q_UNUSED(dee)
    qDebug() << "MainFormController: dragEnterEvent";
//    if (mIsProcessingFile)
//    {
//        return;
//    }

//    if (dee->mimeData().hasFormat("text/plain") && !mHeightMapMode)
//    {
//        dee->acceptProposedAction();
//    }

//    else if (dee->mimeData().hasFormat("text/uri-list") && dee->mimeData()->urls().count() == 1)
//    {
//        QString fileName = dee->mimeData()->urls().at(0).toLocalFile();

//        if  (
//                (!mHeightMapMode && isGcodeFile(fileName)) ||
//                (mHeightMapMode && isHeightmapFile(fileName))
//            )
//        {
//            dee->acceptProposedAction();
//        }
//    }
}

void MainFormController::dropEvent(QDropEvent *de)
{
    Q_UNUSED(de)
    qDebug() << "MainFormController: dropEvent";
//    QString fileName = de->mimeData()->urls().at(0).toLocalFile();

//    if (!mHeightMapMode)
//    {
//        if (!saveChanges(false))
//        {
//            return;
//        }

//        // Load dropped g-code file
//        if (!fileName.isEmpty())
//        {
//            addRecentFile(fileName);
//            updateRecentFilesMenu();
//            loadFile(fileName);
//            // Load dropped text
//        }
//        else
//        {
//            mProgramFileName.clear();
//            mFileChanged = true;
//            loadFile(de->mimeData()->text().split("\n"));
//        }
//    }
//    else
//    {
//        if (!saveChanges(true))
//        {
//            return;
//        }

//        // Load dropped heightmap file
//        addRecentHeightmap(fileName);
//        updateRecentFilesMenu();
//        loadHeightMap(fileName);
//    }
}

void MainFormController::onActFileExitTriggered()
{
    qDebug() << "MainFormController: onActFileExitTriggered";
    mMainWindow.close();
}

void MainFormController::storeParserState()
{
    qDebug() << "MainFormController: storeParserState";
    /*
    mStoredParserStatus = mVisualisationFormController->getParserStatus();
    mStoredParserStatus = mStoredParserStatus.remove(
        QRegExp("\\[|\\]|G[01234]\\s|M[0345]+\\s|\\sF[\\d\\.]+|\\sS[\\d\\.]+")
    );
    */
}

void MainFormController::onActSettingsTriggered()
{
    qDebug() << "MainFormController: onActSettingsTriggered";
    if (mSettingsFormController.exec())
    {
        qDebug() << "MainFormController: Applying settings";
        qDebug() << "Port:" << mSettingsFormController.getPortName()
                 << "Baud:" << mSettingsFormController.getBaudRate();

        if  (
                mSettingsFormController.getPortName() != "" &&
                (
                    mSettingsFormController.getPortName() != mSerialPortModel.getPortName() ||
                    mSettingsFormController.getBaudRate() != mSerialPortModel.getBaudRate()
                 )
             ) {

            if (mSerialPortModel.isPortOpen())
            {
                mSerialPortModel.closePort();
            }

            mSerialPortModel.setPortName(mSettingsFormController.getPortName());
            mSerialPortModel.setBaudRate(mSettingsFormController.getBaudRate());
            mSerialPortModel.openPort();
        }

        mUi.controlFormController->updateControlsState();
        mSettingsFormController.applySettings();
    }
    else
    {
        mSettingsFormController.undo();
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
        else if (mGcodeFileModel.isGcodeFile(fileName))
        {
            qDebug() << "MainFormController: Gcode file format";
            mGcodeFileModel.load(fileName);
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
    return mFormMode == MAIN_FORM_MODE_HEIGHTMAP;
}

bool MainFormController::isInGerberMode()
{
    qDebug() << "MainFormController: isInGerberMode";
    return mFormMode == MAIN_FORM_MODE_GERBER;
}

void MainFormController::onActFileNewTriggered()
{
    qDebug() << "MainFormController: onActFileNewTriggered";
    qDebug() << "MainFormController: changes:" << mGcodeFileModel.hasFileChanged()
             << mHeightMapFileModel.hasFileChanged();

    if (!saveChanges(mHeightMapMode))
    {
        return;
    }

    if (!mHeightMapMode)
    {
        // Reset tables
        /*
        clearTable();
        mProbeTableModel.clear();
        mProgramHeightmapTableModel.clear();
        mCurrentGcodeTableModel = &mProgramTableModel;
        // Reset parsers
        mViewParser.reset();
        mProbeParser.reset();
        // Reset code drawer
        mCodeDrawer->update();
        mCurrentDrawer = mCodeDrawer;
        mUi.glwVisualizer->fitDrawable();
        updateProgramEstimatedTime(QList<LineSegment*>());
        mProgramFileName = "";
        mUi.chkHeightMapUse->setChecked(false);
        mUi.grpHeightMap->setProperty("overrided", false);
        style()->unpolish(mUi.grpHeightMap);
        mUi.grpHeightMap->ensurePolished();
        // Reset tableview
        QByteArray headerState = mUi.tblProgram.horizontalHeader()->saveState();
        mUi.tblProgram->setModel(NULL);
        // Set table model
        mUi.tblProgram->setModel(&mProgramTableModel);
        mUi.tblProgram.horizontalHeader()->restoreState(headerState);
        // Update tableview
        connect(mUi.tblProgram->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex)));
        mUi.tblProgram->selectRow(0);
        // Clear selection marker
        m_selectionDrawer.setEndPosition(QVector3D(sNan, sNan, sNan));
        m_selectionDrawer.update();
        resetHeightmap();
        */
    }
    else
    {
        /*
        mHeightMapModel.clear();
        onCmdFileResetClicked();
        mUi.txtHeightMap->setText(tr("Untitled"));
        mHeightMapFileName.clear();
        updateHeightMapBorderDrawer();
        updateHeightMapGrid();
        mHeightMapChanged = false;
        */
    }
    //updateControlsState();
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

    if (!fileName.isEmpty())
    {
        //saveProgramToFile(fileName, &mHeightMapFileModel.getProgramHeightmapTableModel());
    }
}

void MainFormController::onActFileSaveAsTriggered()
{
    qDebug() << "MainFormController: onActFileSaveAsTriggered";
    /*
    if (!mHeightMapMode)
    {
        QString fileName = (QFileDialog::getSaveFileName(this, tr("Save file as"), mLastFolder, tr("G-Code files (*.nc *.ncc *.ngc *.tap *.txt)")));

        if (!fileName.isEmpty()) if (saveProgramToFile(fileName, &mProgramTableModel))
        {
            mProgramFileName = fileName;
            mFileChanged = false;

            addRecentFile(fileName);
            updateRecentFilesMenu();

            updateControlsState();
        }
    }
    else
    {
        QString fileName = (QFileDialog::getSaveFileName(this, tr("Save file as"), mLastFolder, tr("Heightmap files (*.map)")));

        if (!fileName.isEmpty()) if (saveHeightMap(fileName))
        {
            mUi.txtHeightMap->setText(fileName.mid(fileName.lastIndexOf("/") + 1));
            mHeightMapFileName = fileName;
            mHeightMapChanged = false;

            addRecentHeightmap(fileName);
            updateRecentFilesMenu();

            updateControlsState();
        }
    }
    */
}

void MainFormController::onActFileSaveTriggered()
{
    qDebug() << "MainFormController: onActFIleSaveTriggered";
    /*
    if (!mHeightMapMode)
    {
        // G-code saving
        if (mProgramFileName.isEmpty())
        {
            onActFileSaveAsTriggered();
        }
        else
        {
            saveProgramToFile(mProgramFileName, &mProgramTableModel);
            mFileChanged = false;
        }
    }
    else
    {
        // Height map saving
        if (mHeightMapFileName.isEmpty())
        {
            onActFileSaveAsTriggered();
        }
        else
        {
            saveHeightMap(mHeightMapFileName);
        }
    }
    */
}

void MainFormController::onActAboutTriggered()
{
    qDebug() << "MainFormController: onActAboutTriggered";
    mAboutFormController.exec();
}

bool MainFormController::dataIsEnd(QString data)
{
    qDebug() << "MainFormController: dataIsEnd";
    QStringList ends;

    ends << "ok";
    ends << "error";
    //    ends << "Reset to continue";
    //    ends << "'$' for help";
    //    ends << "'$H'|'$X' to unlock";
    //    ends << "Caution: Unlocked";
    //    ends << "Enabled";
    //    ends << "Disabled";
    //    ends << "Check Door";
    //    ends << "Pgm End";

    foreach (QString str, ends)
    {
        if (data.contains(str))
        {
            return true;
        }
    }

    return false;
}

bool MainFormController::dataIsFloating(QString data)
{
    qDebug() << "MainFormController: dataIsFloating";
    QStringList ends;

    ends << "Reset to continue";
    ends << "'$H'|'$X' to unlock";
    ends << "ALARM: Soft limit";
    ends << "ALARM: Hard limit";
    ends << "Check Door";

    foreach (QString str, ends)
    {
        if (data.contains(str)) return true;
    }
    return false;
}

bool MainFormController::dataIsReset(QString data)
{
    qDebug() << "MainFormController: dataIsReset";
    return QRegExp("^GRBL|GCARVIN\\s\\d\\.\\d.").indexIn(data.toUpper()) != -1;
}

void MainFormController::onGripFeedToggled(bool checked)
{
    Q_UNUSED(checked)
    qDebug() << "MainFormController: onGripFeedToggled";
    /*
    if (checked)
    {
        mUi.grpFeed->setTitle(tr("Feed"));
    }
    else if (mUi.chkFeedOverride->isChecked())
    {
        mUi.grpFeed->setTitle(tr("Feed") + QString(tr(" (%1)")).arg(mUi.txtFeed->text()));
    }
    updateLayouts();

    mUi.widgetFeed->setVisible(checked);
    */
}

void MainFormController::onGripSpindleToggled(bool checked)
{

    Q_UNUSED(checked)
    qDebug() << "MainFormController: onGripSpindleToggled";
    /*
    if (checked)
    {
        mUi.grpSpindle->setTitle(tr("Spindle"));
    }
    else if (mUi.cmdSpindle->isChecked())
    {
        mUi.grpSpindle->setTitle(tr("Spindle") + QString(tr(" (%1)")).arg(mUi.txtSpindleSpeed->text()));
    }
    updateLayouts();

    mUi.widgetSpindle->setVisible(checked);
    */
}

void MainFormController::onGripJogToggled(bool checked)
{

    Q_UNUSED(checked)
    qDebug() << "MainFormController: onGripJogToggled";
    /*
    if (checked)
    {
        mUi.grpJog->setTitle(tr("Jog"));
    }
    else if (mUi.chkKeyboardControl->isChecked())
    {
        mUi.grpJog->setTitle(tr("Jog") + QString(tr(" (%1)")).arg(mUi.txtJogStep->text()));
    }
    updateLayouts();

    mUi.widgetJog->setVisible(checked);
    */
}

void MainFormController::onGripUserCommandsToggled(bool checked)
{

    Q_UNUSED(checked)
    qDebug() << "MainFormController: onGripUserCommandsToggled";
    //mUi.widgetUserCommands->setVisible(checked);
}

void MainFormController::onHeightMapFileLoadStarted()
{
    qDebug() << "MainFormController: onHeightMapFileLoadStarted";

}

void MainFormController::onHeightMapFileLoadFinished()
{
    qDebug() << "MainFormController: onHeightMapFileLoadFinished";
    setFormMode(MAIN_FORM_MODE_HEIGHTMAP);
    qDebug() << "MainFormController: HeightMapFileModel says a new file \
        has loaded, do appropriate things...";

    onStatusUpdate(
        QString("Opened HeightMap File %s").arg(
            mGcodeFileModel.getCurrentFileName()
        )
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
    /*
    // Main form events
    if (obj == this || obj == mUi.tblProgram)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if (!mIsProcessingFile && keyEvent->key() == Qt::Key_ScrollLock && obj == this)
            {
                mUi.chkKeyboardControl->toggle();
                if (!mUi.chkKeyboardControl->isChecked())
                {
                    mUi.cboCommand->setFocus();
                }
            }

            if (!mIsProcessingFile && mUi.chkKeyboardControl->isChecked())
            {
                // Block only autorepeated keypresses
                if (keyIsMovement(keyEvent->key()) && !(mJogBlock && keyEvent->isAutoRepeat()))
                {
                    blockJogForRapidMovement(keyEvent->isAutoRepeat());
                    switch (keyEvent->key())
                    {
                        case Qt::Key_4:
                            sendCommand("G91G0X-" + mUi.txtJogStep->text(), -1, mSettingsForm->showUICommands());
                            break;
                        case Qt::Key_6:
                            sendCommand("G91G0X" + mUi.txtJogStep->text(), -1, mSettingsForm->showUICommands());
                            break;
                        case Qt::Key_8:
                            sendCommand("G91G0Y" + mUi.txtJogStep->text(), -1, mSettingsForm->showUICommands());
                            break;
                        case Qt::Key_2:
                            sendCommand("G91G0Y-" + mUi.txtJogStep->text(), -1, mSettingsForm->showUICommands());
                            break;
                        case Qt::Key_9:
                            sendCommand("G91G0Z" + mUi.txtJogStep->text(), -1, mSettingsForm->showUICommands());
                            break;
                        case Qt::Key_3:
                            sendCommand("G91G0Z-" + mUi.txtJogStep->text(), -1, mSettingsForm->showUICommands());
                            break;
                    }
                }
                else if (keyEvent->key() == Qt::Key_5 || keyEvent->key() == Qt::Key_Period)
                {
                    QList<StyledToolButton*> stepButtons = mUi.grpJog->findChildren<StyledToolButton*>(QRegExp("cmdJogStep\\d"));
                    std::sort(stepButtons.begin(), stepButtons.end(), buttonLessThan);

                    for (int i = 0; i < stepButtons.count(); i++)
                    {
                        if (stepButtons[i]->isChecked())
                        {
                            StyledToolButton *button = stepButtons[keyEvent->key() == Qt::Key_5
                                    ? (i == stepButtons.length() - 1 ? stepButtons.length() - 1 : i + 1)
                                    : (i == 0 ? 0 : i - 1)];

                            mUi.txtJogStep->setValue(button->text().toDouble());
                            foreach (StyledToolButton* button, mUi.grpJog->findChildren<StyledToolButton*>(QRegExp("cmdJogStep\\d")))
                            {
                                button->setChecked(false);
                            }
                            button->setChecked(true);

                            if (!mUi.grpJog->isChecked()) {
                                mUi.grpJog->setTitle(tr("Jog") + QString(tr(" (%1)")).arg(mUi.txtJogStep->text()));
                            }
                            break;
                        }
                    }
                }
                else if (keyEvent->key() == Qt::Key_0)
                {
                    mUi.cmdSpindle->click();
                }
                else if (keyEvent->key() == Qt::Key_7)
                {
                    mUi.sliSpindleSpeed->setSliderPosition(mUi.sliSpindleSpeed->sliderPosition() + 1);
                }
                else if (keyEvent->key() == Qt::Key_1)
                {
                    mUi.sliSpindleSpeed->setSliderPosition(mUi.sliSpindleSpeed->sliderPosition() - 1);
                }
            }

            if (obj == mUi.tblProgram && mIsProcessingFile)
            {
                if  (
                        keyEvent->key() == Qt::Key_PageDown ||
                        keyEvent->key() == Qt::Key_PageUp ||
                        keyEvent->key() == Qt::Key_Down ||
                        keyEvent->key() == Qt::Key_Up
                    )
                {
                    mUi.chkAutoScroll->setChecked(false);
                }
            }
        }

        // Splitter events
    }
    else if (obj == mUi.splitPanels && event->type() == QEvent::Resize)
    {
        // Resize splited widgets
        onPanelsSizeChanged(mUi.scrollAreaWidgetContents->sizeHint());

        // Splitter handle events
    }
    else if (obj == mUi.splitPanels.handle(1))
    {
        int minHeight = getConsoleMinHeight();
        switch (event->type())
        {
            case QEvent::MouseButtonPress:
                // Store current console group box minimum & real heights
                mStoredConsoleMinimumHeight = mUi.grpConsole->minimumHeight();
                mStoredConsoleHeight = mUi.grpConsole.height();

                // Update splited sizes
                mUi.splitPanels->setSizes(QList<int>() << mUi.scrollArea.height() << mUi.grpConsole.height());

                // Set new console mimimum height
                mUi.grpConsole->setMinimumHeight(qMax(minHeight, mUi.splitPanels.height()
                                                       - mUi.scrollAreaWidgetContents->sizeHint().height() - mUi.splitPanels.handleWidth() - 4));
                break;
            case QEvent::MouseButtonRelease:
                // Store new console minimum height if height was changed with split handle
                if (mUi.grpConsole.height() != mStoredConsoleHeight)
                {
                    mUi.grpConsole->setMinimumHeight(mUi.grpConsole.height());
                }
                else
                {
                    mUi.grpConsole->setMinimumHeight(mStoredConsoleMinimumHeight);
                }
                break;
            case QEvent::MouseButtonDblClick:
                // Switch to min/max console size
                if (mUi.grpConsole.height() == minHeight || !mUi.scrollArea->verticalScrollBar()->isVisible())
                {
                    mUi.splitPanels->setSizes(QList<int>() << mUi.scrollArea->minimumHeight()
                                               << mUi.splitPanels.height() - mUi.splitPanels.handleWidth() - mUi.scrollArea->minimumHeight());
                }
                else
                {
                    mUi.grpConsole->setMinimumHeight(minHeight);
                    onPanelsSizeChanged(mUi.scrollAreaWidgetContents->sizeHint());
                }
                break;
            default:
                break;
        }
    }

    */
    return mMainWindow.eventFilter(obj, event);
}

void MainFormController::onPanelsSizeChanged(QSize size)
{
    qDebug() << "MainFormController: onPandleSizeChanged";
    /*
    mUi.splitPanels->setSizes(QList<int>() << size.height() + 4
                               << mUi.splitPanels.height() - size.height()
                               - 4 - mUi.splitPanels.handleWidth());
    */
}

void MainFormController::onSplitterSplitterMoved(int pos, int index)
{
    qDebug() << "MainFormController: onSplitterSplitterMoved";
    /*
    Q_UNUSED(pos)
    Q_UNUSED(index)

    static bool tableCollapsed = mUi.splitter->sizes()[1] == 0;

    if ((mUi.splitter->sizes()[1] == 0) != tableCollapsed)
    {
        this->setUpdatesEnabled(false);
        mUi.chkAutoScroll->setVisible(mUi.splitter->sizes()[1] && !mHeightMapMode);
        updateLayouts();
        resizeCheckBoxes();

        this->setUpdatesEnabled(true);
        mUi.chkAutoScroll->repaint();

        // Store collapsed state
        tableCollapsed = mUi.splitter->sizes()[1] == 0;
    }
    */
}

void MainFormController::updateLayouts()
{
    qDebug() << "MainFormController: updateLayouts";
    /*
    mMainWindow->update();
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    */
}

void MainFormController::onActRecentFileTriggered()
{
    qDebug() << "MainFormController: onActRecentFileTriggered";
    /*
    QAction *action = static_cast<QAction*>(sender());
    QString fileName = action->text();

    if (action != NULL)
    {
        if (!saveChanges(mHeightMapMode))
        {
            return;
        }
        if (!mHeightMapMode)
        {
            loadFile(fileName);
        }
        else {
            loadHeightMap(fileName);
        }
    }
    */
}

void MainFormController::updateRecentFilesMenu()
{
    qDebug() << "MainFormController: updateRecentFilesMenu";
    /*
    foreach (QString file, !mHeightMapMode ? mRecentFiles : mRecentHeightmaps)
    {
        QAction *action = new QAction(file, this);
        connect(action, SIGNAL(triggered()), this, SLOT(onActRecentFileTriggered()));
        mUi.mnuRecent->insertAction(mUi.mnuRecent->actions()[0], action);
    }

    updateControlsState();
    */
}

void MainFormController::onActRecentClearTriggered()
{
    qDebug() << "MainFormController: onActRecentClearTriggered";
    /*
    if (!mHeightMapMode)
    {
        mRecentFiles.clear();
    }
    else
    {
        mRecentHeightmaps.clear();
    }
    updateRecentFilesMenu();
    */
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

void MainFormController::onStatusUpdate(QString status)
{
    qDebug() << "MainFormController: onStatusUpdate";
    mUi.statusBar->showMessage(status);
}

void MainFormController::onGcodeFileLoadStarted()
{
    qDebug() << "MainFormController: onGcodeFileLoadStarted";
}

void MainFormController::onGcodeFileLoadFinished()
{
    qDebug() << "MainFormController: onGcodeFileLoadFinished";
    setFormMode(MAIN_FORM_MODE_GERBER);
    qDebug() << "MainFormController: GcodeFileModel says a new file has loaded"
             << ", do appropriate things...";

    onStatusUpdate(
        QString("Opened Gcode File " + mGcodeFileModel.getCurrentFileName())
    );
}

void MainFormController::onCommandSent(QString command, int len)
{
    Q_UNUSED(command)
    Q_UNUSED(len)
    qDebug() << "MainFormController: onCommandSent";
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

void MainFormController::onSerialPortError(QString error)
{
    qDebug() << "MainFormController: onSerialPortError";
    QErrorMessage serialPortError;
    serialPortError.setWindowTitle("Serial Port Error");
    serialPortError.showMessage(
        QString("Serial Port Error:\n\n%s").arg(error)
    );
}
