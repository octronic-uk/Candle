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
#include "Model/Settings/Sql/SqlSettingsModel.h"

VisualisationFormController::VisualisationFormController(QWidget *parent)
    : AbstractFormController(parent),
      mButtonPadding(4),
      mFollowTool(false),
      mSpindleClockwise(false),
      mSettingsModelHandle(nullptr)
{
    qDebug() << "VisualisationFormController: Constructing ";

    mUi.setupUi(this);
    mWorkArea = QVector3D(280,280,80);
    mWCO = QVector3D(0,0,0);
    mMachinePosition = QVector3D(0,0,0);
    mWorkPosition = QVector3D(0,0,0);
    mToolDrawer.setToolPosition(QVector3D(0, 0, 0));
    mLastDrawnLineIndex = 0;

    mOriginDrawer.setVisible(true);
    mGridDrawer.setVisible(true);

    mUi.glwVisualizer->setAntialiasing(true);
    mUi.glwVisualizer->setVsync(true);
    mUi.glwVisualizer->addDrawable(&mGridDrawer);
    mUi.glwVisualizer->addDrawable(&mOriginDrawer);
    mUi.glwVisualizer->addDrawable(&mCodeDrawer);
    mUi.glwVisualizer->addDrawable(&mToolDrawer);
    mUi.glwVisualizer->addDrawable(&mSafePositionDrawer);
    mUi.glwVisualizer->addDrawable(&mSelectionDrawer);
    mUi.glwVisualizer->fitDrawable(mCodeDrawer);
    setupSignalSlots();

    mSafePositionDrawer.setVisible(false);
}

VisualisationFormController::~VisualisationFormController()
{
    qDebug() << "VisualisationFormController: Destructing";
}

void VisualisationFormController::setFormActive(bool active)
{
    mUi.cmdFit->setEnabled(active);
    mUi.cmdFront->setEnabled(active);
    mUi.cmdIsometric->setEnabled(active);
    mUi.cmdLeft->setEnabled(active);
    mUi.cmdTop->setEnabled(active);
    mUi.toggleFollowToolButton->setEnabled(active);
}

void VisualisationFormController::initialise()
{
    mCodeDrawer.initialise();
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
    mUi.cmdIsometric->setChecked(false);
}

void VisualisationFormController::onFitButtonClicked()
{
    qDebug() << "VisualisationFormController: onCmdFitClicked";
    mUi.glwVisualizer->fitDrawable(mCodeDrawer);
}

void VisualisationFormController::setupSignalSlots()
{
    qDebug() << "VisualisationFormController: Setup Signals/Slots";
    connect(
        getVisualiser(), SIGNAL(rotationChanged()),
        this, SLOT(onVisualizatorRotationChanged())
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
    connect(
        mUi.toggleGridButton, SIGNAL(toggled(bool)),
        this, SLOT(onToggleGridButtonToggled(bool))
    );
    connect(
        mUi.toggleToolButton, SIGNAL(toggled(bool)),
        this, SLOT(onToggleToolButtonToggled(bool))
    );
    connect(
        mUi.toggleFollowToolButton, SIGNAL(toggled(bool)),
        this, SLOT(onFollowToolButtonToggled(bool))
    );
    connect(mUi.toggleSafePositionButton,SIGNAL(toggled(bool)),this,SLOT(onShowSafePositionToggled(bool)));
}

void VisualisationFormController::onGcodeFileLoadStarted()
{

}

GLWidget* VisualisationFormController::getVisualiser()
{
    qDebug() << "VisualisationFormController: getVisualizer";
    return mUi.glwVisualizer;
}

void VisualisationFormController::onGcodeFileLoadFinished(GcodeFileModel* items)
{
    Q_UNUSED(items)
    qDebug() << "VisualisationFormController: onGcodeFileLoadFinished";
}

void VisualisationFormController::onGcodeParserUpdated(GcodeParser* parser)
{
    qDebug() << "VisualisationFormController: onGcodeParserUpdated";
    mCodeDrawer.updateViewParser(parser);
    mUi.glwVisualizer->setUpdatesEnabled(true);
    mUi.glwVisualizer->fitDrawable(mCodeDrawer);
    mCodeDrawer.setVisible(true);
}

void VisualisationFormController::onToggleGridButtonToggled(bool toggled)
{
   mGridDrawer.setVisible(toggled);
}

void VisualisationFormController::onToggleToolButtonToggled(bool toggled)
{
    mToolDrawer.setVisible(toggled);
}

void VisualisationFormController::onShowSafePositionToggled(bool toggled)
{
   mSafePositionDrawer.setVisible(toggled);
}

void VisualisationFormController::onFollowToolButtonToggled(bool toggled)
{
    mFollowTool = toggled;
}

void VisualisationFormController::onSafePositionSetSignal()
{
   mUi.toggleSafePositionButton->setChecked(true);
   mSafePositionDrawer.setVisible(true);
   mSafePositionDrawer.setPosition(mToolDrawer.toolPosition());
   mSafePositionDrawer.setNeedsUpdate();
}

void VisualisationFormController::onUpdateWorkPosition(const QVector3D pos)
{
    mWorkPosition = pos;
}

void VisualisationFormController::onUpdateMachinePosition(const QVector3D pos)
{
    mMachinePosition = pos;

    QVector3D adjustedPos = pos;
    adjustedPos.setX(adjustedPos.x() + mWorkArea.x());
    adjustedPos.setY(adjustedPos.y() + mWorkArea.y());
    adjustedPos.setZ(adjustedPos.z() + mWorkArea.z());

    mToolDrawer.setToolPosition(adjustedPos);
    if (mFollowTool)
    {
        mUi.glwVisualizer->onLookAt(adjustedPos);
    }
    else
    {
        mUi.glwVisualizer->onLookAt(mOriginDrawer.getPosition());
    }

}

void VisualisationFormController::onSettingsModelReady(SqlSettingsModel* settings)
{
    mSettingsModelHandle = settings;
    if (mSettingsModelHandle)
    {
        mToolDrawer.setSettingsModelHandle(mSettingsModelHandle);
        Profile* currentProfile =  mSettingsModelHandle->getCurrentProfileHandle();
        Tool* tool = currentProfile->getToolListModelHandle()->getData(0);

        if (tool)
        {
            // TODO - Set the current tool dynamically based to tool change events
            qDebug() << "VisualisationFormController: settings tool";
            mToolDrawer.setToolHandle(tool);
            mToolDrawer.update();
        }
    }
}

void VisualisationFormController::onUpdateWCO(const QVector3D wco)
{
    if (wco != mWCO)
    {
       mWCO = wco;
       mOriginDrawer.setPosition(mWCO + mWorkArea);
       mCodeDrawer.setPosition(mWCO + mWorkArea);
    }

}



void VisualisationFormController::onFirmwareConfigurationRead(int param, QString value)
{
    qDebug() << "VisualisationFormController: Firmware Config Read";

    switch (param)
    {
        case 130:
            qDebug() << "VisualisationFormController: Setting work area x" << value.toFloat();
            mWorkArea.setX(value.toFloat());
            mGridDrawer.onSizeUpdated(mWorkArea);
            break;

        case 131:
            qDebug() << "VisualisationFormController: Setting work area y" << value.toFloat();
            mWorkArea.setY(value.toFloat());
            mGridDrawer.onSizeUpdated(mWorkArea);
            break;

        case 132:
            qDebug() << "VisualisationFormController: Setting work area z" << value.toFloat();
            mWorkArea.setZ(value.toFloat());
            mGridDrawer.onSizeUpdated(mWorkArea);
            break;

        default:
            break;
    }
}

void VisualisationFormController::onSpindleSpeedChanged(int speed)
{
    mToolDrawer.setSpindleRotating(speed > 0);
    mToolDrawer.setSpindleSpeed(speed);
}
