/*
 * VisualisationFormController.cpp
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
#include "VisualisationFormController.h"

VisualisationFormController::VisualisationFormController(QWidget *parent)
    : AbstractFormController(parent),
      mButtonPadding(4),
      mSpindleClockwise(false)
{
    qDebug() << "VisualisationFormController: Constructing ";

    mUi.setupUi(this);
    /*
    mUi.cmdFit->setParent(mUi.glwVisualizer);
    mUi.cmdIsometric->setParent(mUi.glwVisualizer);
    mUi.cmdTop->setParent(mUi.glwVisualizer);
    mUi.cmdFront->setParent(mUi.glwVisualizer);
    mUi.cmdLeft->setParent(mUi.glwVisualizer);
    */

    mCodeDrawer.setViewParser(&mViewParser);
    mProbeDrawer.setViewParser(&mProbeParser);
    mProbeDrawer.setVisible(false);
    mToolDrawer.setToolPosition(QVector3D(0, 0, 0));
    mLastDrawnLineIndex = 0;

    mOriginDrawer.setVisible(true);

    mUi.glwVisualizer->addDrawable(&mOriginDrawer);
    mUi.glwVisualizer->addDrawable(&mCodeDrawer);
    mUi.glwVisualizer->addDrawable(&mToolDrawer);
    mUi.glwVisualizer->addDrawable(&mSelectionDrawer);
    /*
    mUi.glwVisualizer->addDrawable(&mProbeDrawer);
    mUi.glwVisualizer->addDrawable(&mHeightMapBorderDrawer);
    mUi.glwVisualizer->addDrawable(&mHeightMapGridDrawer);
    mUi.glwVisualizer->addDrawable(&mHeightMapInterpolationDrawer);
    */
    mUi.glwVisualizer->fitDrawable();
    placeVisualizerButtons();
    setupSignalSlots();

    mToolAnimationTimer.start(250, this);
    //mUi.cmdSpindle->setChecked(true);
    emit spindleEnabledSignal(true);
}

VisualisationFormController::~VisualisationFormController()
{
    qDebug() << "VisualisationFormController: Destructing";
}

void VisualisationFormController::placeVisualizerButtons()
{
    /*
    qDebug() << "VisualisationFormController: placeVisualizerButtons";
    int xPos = mButtonPadding;
    mUi.cmdIsometric->move(xPos,0); //mButtonPadding);
    mUi.cmdTop->move(xPos, mUi.cmdIsometric->geometry().bottom() + mButtonPadding);
    mUi.cmdLeft->move(xPos, mUi.cmdTop->geometry().bottom() + mButtonPadding);
    mUi.cmdFront->move(xPos, mUi.cmdLeft->geometry().bottom() + mButtonPadding);
    mUi.cmdFit->move(xPos, mUi.cmdFront->geometry().bottom() + mButtonPadding);
    */
}

void VisualisationFormController::onTopButtonClicked()
{
    qDebug() << "VisualisationFormController: onCmdTopClicked";
    mUi.glwVisualizer->setTopView();
}

void VisualisationFormController::onFrontButtonClicked()
{
    qDebug() << "VisualisationFormController: onCmdFrontClicked";
    mUi.glwVisualizer->setFrontView();
}

void VisualisationFormController::onLeftButtonClicked()
{
    qDebug() << "VisualisationFormController: onCmdLeftClicked";
    mUi.glwVisualizer->setLeftView();
}

void VisualisationFormController::onCmdIsometricClicked()
{
    qDebug() << "VisualisationFormController: isCmdIsometricClicked";
    mUi.glwVisualizer->setIsometricView();
}

double VisualisationFormController::toolZPosition()
{
    qDebug() << "VisualisationFormController: toolZPosition";
    return static_cast<double>(mToolDrawer.toolPosition().z());
}

QString VisualisationFormController::getParserStatus()
{
    qDebug() << "VisualisationFormController: getParserStatus";
    return mUi.glwVisualizer->parserStatus();
}

void VisualisationFormController::onVisualizatorRotationChanged()
{
    //qDebug() << "VisualisationFormController: onVisualizatorRotationChanged";
    mUi.cmdIsometric->setChecked(false);
}

void VisualisationFormController::onFitButtonClicked()
{
    qDebug() << "VisualisationFormController: onCmdFitClicked";
    mUi.glwVisualizer->fitDrawable(&mCodeDrawer);
}

void VisualisationFormController::updateParser()
{
    qDebug() << "VisualisationFormController: updateParser";
    /*
    QTime time;

    qDebug() << "updating parser:" << mCurrentGCodeTableModel << mCurrentDrawer;
    time.start();

    GcodeViewParse *parser = mCurrentDrawer->viewParser();

    GcodeParser gp;
    gp.setTraverseSpeed(mSettingsForm->rapidSpeed());
    if (mCodeDrawer->getIgnoreZ()) gp.reset(QVector3D(qQNaN(), qQNaN(), 0));

    mUi->tblProgram->setUpdatesEnabled(false);

    QString stripped;
    QList<QString> args;

    QProgressDialog progress(tr("Updating..."), tr("Abort"), 0, mCurrentGCodeTableModel->rowCount() - 2, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setFixedSize(progress.sizeHint());

    if (mCurrentGCodeTableModel->rowCount() > PROGRESS_MIN_LINES) {
        progress.show();
        progress.setStyleSheet("QProgressBar {text-align: center; qproperty-format: \"\"}");
    }

    for (int i = 0; i < mCurrentGCodeTableModel->rowCount() - 1; i++) {
        // Get stored args
        args = mCurrentGCodeTableModel->data().at(i).args;

        // Store args if none
        if (args.isEmpty()) {
            stripped = GcodePreprocessorUtils::removeComment(mCurrentGCodeTableModel->data().at(i).command);
            args = GcodePreprocessorUtils::splitCommand(stripped);
            mCurrentGCodeTableModel->data()[i].args = args;
        }

        // Add command to parser
        gp.addCommand(args);

        // Update table model
        mCurrentGCodeTableModel->data()[i].state = GCodeItem::InQueue;
        mCurrentGCodeTableModel->data()[i].response = QString();
        mCurrentGCodeTableModel->data()[i].line = gp.getCommandNumber();

        if (progress.isVisible() && (i % PROGRESS_STEP == 0)) {
            progress.setValue(i);
            qApp->processEvents();
            if (progress.wasCanceled()) break;
        }
    }
    progress.close();

    mUi->tblProgram->setUpdatesEnabled(true);

    parser->reset();

    updateProgramEstimatedTime(parser->getLinesFromParser(&gp, mSettingsForm->arcPrecision(), mSettingsForm->arcDegreeMode()));
    mCurrentDrawer->update();
    mUi->glwVisualizer->updateExtremes(mCurrentDrawer);
    updateControlsState();

    if (mCurrentGCodeTableModel == &mProgramTableModel) m_fileChanged = true;

    qDebug() << "Update parser time: " << time.elapsed();
    */
}

void VisualisationFormController::setupSignalSlots()
{
    qDebug() << "VisualisationFormController: Setup Signals/Slots";
    connect
            (
                getVisualiser(), SIGNAL(rotationChanged()),
                this, SLOT(onVisualizatorRotationChanged())
                );
    connect
            (
                getVisualiser(), SIGNAL(resized()),
                this, SLOT(placeVisualizerButtons())
                );
    // Command Buttons
    connect(
                mUi.cmdFit, SIGNAL(clicked()),
                this, SLOT(onFitButtonClicked())
                );
    connect(
                mUi.cmdFront, SIGNAL(clicked()),
                this, SLOT(onFrontButtonClicked())
                );
    connect(
                mUi.cmdIsometric, SIGNAL(clicked()),
                this, SLOT(onCmdIsometricClicked())
                );
    connect(
                mUi.cmdLeft, SIGNAL(clicked()),
                this, SLOT(onLeftButtonClicked())
                );
    connect(
                mUi.cmdTop, SIGNAL(clicked()),
                this, SLOT(onTopButtonClicked())
                );
}

void VisualisationFormController::showEvent(QShowEvent *se)
{
    qDebug() << "VisualisationFormController: showEvent";
    Q_UNUSED(se)
    placeVisualizerButtons();
    mUi.glwVisualizer->setUpdatesEnabled(true);
}

void VisualisationFormController::hideEvent(QHideEvent *he)
{
    qDebug() << "VisualisationFormController: hideEvent";
    Q_UNUSED(he)
    mUi.glwVisualizer->setUpdatesEnabled(false);
}

void VisualisationFormController::resizeEvent(QResizeEvent *re)
{
//    qDebug() << "VisualisationFormController: resizeEvent";
    Q_UNUSED(re)
    placeVisualizerButtons();
}

void VisualisationFormController::onGcodeFileLoadStarted()
{

}

GLWidget* VisualisationFormController::getVisualiser()
{
    qDebug() << "VisualisationFormController: getVisualizer";
    return mUi.glwVisualizer;
}

void VisualisationFormController::setGLWBufferState(QString state)
{
    //qDebug() << "VisualisationFormController: setGLWBufferState";
    Q_UNUSED(state)
}

void VisualisationFormController::timerEvent(QTimerEvent *te)
{
    //qDebug() << "VisualisationFormController: timerEvent";
    if (te->timerId() == mToolAnimationTimer.timerId())
    {
        mToolDrawer.rotate(mSpindleClockwise ? -1 : 1);
            /*(double)(mUi.txtSpindleSpeed->value()) / */
            /*(mUi.txtSpindleSpeed->maximum())*/
    }
    mUi.glwVisualizer->repaint();
    mUi.glwVisualizer->timerEvent(te);
}

void VisualisationFormController::onGcodeFileLoadFinished(QList<GcodeItem> items)
{
    qDebug() << "VisualisationFormController: onGcodeFileLoadFinished";
}

void VisualisationFormController::onGcodeParserUpdated(GcodeParser *parser)
{
    mViewParser.getLinesFromParser(parser,5.0,true);
    mCodeDrawer.setViewParser(&mViewParser);
    mCodeDrawer.update();
    mUi.glwVisualizer->setUpdatesEnabled(true);
    mUi.glwVisualizer->fitDrawable(&mCodeDrawer);
    mCodeDrawer.setVisible(true);
}