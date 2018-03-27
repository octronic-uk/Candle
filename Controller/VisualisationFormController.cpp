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
    : AbstractFormController(parent)
{
    QMessageLogger().info("Constructing VisualisationFormController");
    mUi.setupUi(this);
    /*
    mUi.cmdFit->setParent(mUi.glwVisualizer);
    mUi.cmdIsometric->setParent(mUi.glwVisualizer);
    mUi.cmdTop->setParent(mUi.glwVisualizer);
    mUi.cmdFront->setParent(mUi.glwVisualizer);
    mUi.cmdLeft->setParent(mUi.glwVisualizer);

    mCodeDrawer.setViewParser(&mViewParser);
    mProbeDrawer.setViewParser(&mProbeParser);
    mProbeDrawer.setVisible(false);
    //mCurrentDrawer = mCodeDrawer;
    mToolDrawer.setToolPosition(QVector3D(0, 0, 0));
    mLastDrawnLineIndex = 0;

    mUi.glwVisualizer->addDrawable(&mOriginDrawer);
    mUi.glwVisualizer->addDrawable(&mCodeDrawer);
    mUi.glwVisualizer->addDrawable(&mProbeDrawer);
    mUi.glwVisualizer->addDrawable(&mToolDrawer);
    mUi.glwVisualizer->addDrawable(&mHeightMapBorderDrawer);
    mUi.glwVisualizer->addDrawable(&mHeightMapGridDrawer);
    mUi.glwVisualizer->addDrawable(&mHeightMapInterpolationDrawer);
    mUi.glwVisualizer->addDrawable(&mSelectionDrawer);
    mUi.glwVisualizer->fitDrawable();

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
    */
}

VisualisationFormController::~VisualisationFormController()
{
    QMessageLogger().info("Denstructing VisualisationFormController");
}

void VisualisationFormController::placeVisualizerButtons()
{
    mUi.cmdIsometric->move(mUi.glwVisualizer->width() - mUi.cmdIsometric->width() - 8, 8);
    mUi.cmdTop->move(mUi.cmdIsometric->geometry().left() - mUi.cmdTop->width() - 8, 8);
    mUi.cmdLeft->move(mUi.glwVisualizer->width() - mUi.cmdLeft->width() - 8, mUi.cmdIsometric->geometry().bottom() + 8);
    mUi.cmdFront->move(mUi.cmdLeft->geometry().left() - mUi.cmdFront->width() - 8, mUi.cmdIsometric->geometry().bottom() + 8);
//    mUi.cmdFit->move(mUi.cmdTop->geometry().left() - mUi.cmdFit->width() - 10, 10);
    mUi.cmdFit->move(mUi.glwVisualizer->width() - mUi.cmdFit->width() - 8, mUi.cmdLeft->geometry().bottom() + 8);
}

void VisualisationFormController::onCmdTopClicked()
{
    mUi.glwVisualizer->setTopView();
}

void VisualisationFormController::onCmdFrontClicked()
{
    mUi.glwVisualizer->setFrontView();
}

void VisualisationFormController::onCmdLeftClicked()
{
    mUi.glwVisualizer->setLeftView();
}

void VisualisationFormController::onCmdIsometricClicked()
{
    mUi.glwVisualizer->setIsometricView();
}

double VisualisationFormController::toolZPosition()
{
    return mToolDrawer.toolPosition().z();
}

QString VisualisationFormController::getParserStatus()
{
   return mUi.glwVisualizer->parserStatus();
}

void VisualisationFormController::onVisualizatorRotationChanged()
{
    mUi.cmdIsometric->setChecked(false);
}

void VisualisationFormController::onCmdFitClicked()
{
    mUi.glwVisualizer->fitDrawable(&mCurrentDrawer);
}

void VisualisationFormController::updateParser()
{
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

void VisualisationFormController::showEvent(QShowEvent *se)
{
    /*
    Q_UNUSED(se)

    placeVisualizerButtons();

    #ifdef WINDOWS
        if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
            if (m_taskBarButton == NULL) {
                m_taskBarButton = new QWinTaskbarButton(this);
                m_taskBarButton->setWindow(this->windowHandle());
                m_taskBarProgress = m_taskBarButton->progress();
            }
        }
    #endif

    mUi->glwVisualizer->setUpdatesEnabled(true);

    resizeCheckBoxes();
    */
}

void VisualisationFormController::hideEvent(QHideEvent *he)
{
    /*
    Q_UNUSED(he)
    mUi->glwVisualizer->setUpdatesEnabled(false);
    */
}

void VisualisationFormController::resizeEvent(QResizeEvent *re)
{
    /*
    Q_UNUSED(re)
    placeVisualizerButtons();
    */
}

GLWidget* VisualisationFormController::getVisualiser()
{
    return mUi.glwVisualizer;
}

void VisualisationFormController::setGLWBufferState(QString state)
{

}

void VisualisationFormController::timerEvent(QTimerEvent *te)
{
    /*
    if (te->timerId() == mToolAnimationTimer.timerId())
    {
        mToolDrawer.rotate(
            (mSpindleCW ? -40 : 40) *
            (double)(mUi.txtSpindleSpeed->value()) /
            (mUi.txtSpindleSpeed->maximum())
        );
    }
    else
    {
        QMainWindow::timerEvent(te);
    }
    */
}
