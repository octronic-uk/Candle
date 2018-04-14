/*
 * InterfaceForm.cpp
 *
 * Created: 08 2018 by Ashley
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

#include "InterfaceFormController.h"
#include "Model/Settings/Settings.h"
#include "Model/Settings/Sql/SqlSettingsModel.h"
#include <QColorDialog>

InterfaceFormController::InterfaceFormController(QWidget *parent)
    : AbstractFormController(parent),
      mSettingsModelHandle(nullptr)
{
    mUi.setupUi(this);
    setupSignalSlots();
}

InterfaceFormController::~InterfaceFormController() {}

void InterfaceFormController::setupSignalSlots()
{
    // Visualiser
    connect(mUi.txtLineWidth, SIGNAL(valueChanged(QString)), this, SLOT(onLineWidthValueChanged(QString)));
    connect(mUi.cboFps, SIGNAL(activated(QString)), this, SLOT(onFpsCurrentIndexChanged(QString)));
    connect(mUi.chkAntialiasing, SIGNAL(toggled(bool)), this, SLOT(onAntiAliasingToggled(bool)));
    connect(mUi.radMSAA, SIGNAL(toggled(bool)), this, SLOT(onMsaaToggled(bool)));
    connect(mUi.chkVSync, SIGNAL(toggled(bool)), this, SLOT(onVsyncToggled(bool)));
    connect(mUi.chkZBuffer, SIGNAL(toggled(bool)), this, SLOT(onZBufferToggled(bool)));
    // Console
    connect(mUi.chkShowUICommands, SIGNAL(toggled(bool)), this, SLOT(onShowUiCommandsToggled(bool)));
    // Colors
    connect(mUi.visualiserColorButton, SIGNAL(clicked()),this,SLOT(onVisualiserColorButtonClicked()));
    connect(mUi.backgroundColorButton, SIGNAL(clicked()),this,SLOT(onBackgroundColorButtonClicked()));
    connect(mUi.toolColorButton, SIGNAL(clicked()),this,SLOT(onToolColorButtonClicked()));
    connect(mUi.toolpathColorButton, SIGNAL(clicked()),this,SLOT(onToolPathColorButtonClicked()));
    connect(mUi.normalColorButton, SIGNAL(clicked()),this,SLOT(onNormalColorButtonClicked()));
    connect(mUi.highlightColorButton, SIGNAL(clicked()),this,SLOT(onHighlightColorButtonClicked()));
    connect(mUi.startPointColorButton, SIGNAL(clicked()),this,SLOT(onStartPointColorButtonClicked()));
    connect(mUi.endPointColorButton, SIGNAL(clicked()),this,SLOT(onEndPointColorButtonClicked()));
    connect(mUi.textColorButton, SIGNAL(clicked()),this,SLOT(onTextColorButtonClicked()));
    connect(mUi.drawnColorButton, SIGNAL(clicked()),this,SLOT(onDrawnColorButtonClicked()));
    connect(mUi.zMovementColorButton, SIGNAL(clicked()),this,SLOT(onZMovementColorButtonClicked()));
}

void InterfaceFormController::setFormActive(bool)
{

}

void InterfaceFormController::initialise()
{
    if (isModelValid())
    {
        auto settings = getSettings();
        setLineWidth(settings->getLineWidth());
        setAntialiasing(settings->getAntiAliasing());
        setMsaa(settings->getMsaa());
        setFps(settings->getFpsLock());
        setVsync(settings->getVsync());
        setZBuffer(settings->getZbuffer());
        setShowUiCommands(settings->getShowUiCommands());
    }
}

bool InterfaceFormController::antialiasing()
{
    return mUi.chkAntialiasing->isChecked();
}

void InterfaceFormController::setAntialiasing(bool antialiasing)
{
    mUi.chkAntialiasing->setChecked(antialiasing);
}

bool InterfaceFormController::zBuffer()
{
    return mUi.chkZBuffer->isChecked();
}

void InterfaceFormController::setZBuffer(bool zBuffer)
{
    mUi.chkZBuffer->setChecked(zBuffer);
}

double InterfaceFormController::lineWidth()
{
    return mUi.txtLineWidth->value();
}

void InterfaceFormController::setLineWidth(double lineWidth)
{
    mUi.txtLineWidth->setValue(lineWidth);
}

bool InterfaceFormController::showUiCommands()
{
    return mUi.chkShowUICommands->isChecked();
}

void InterfaceFormController::setShowUiCommands(bool showUICommands)
{
    mUi.chkShowUICommands->setChecked(showUICommands);
}

int InterfaceFormController::fps()
{
    return mUi.cboFps->currentText().toInt();
}

void InterfaceFormController::setFps(int fps)
{
    mUi.cboFps->setCurrentText(QString::number(fps));
}

bool InterfaceFormController::vsync()
{
    return mUi.chkVSync->isChecked();
}

void InterfaceFormController::setVsync(bool value)
{
    mUi.chkVSync->setChecked(value);
}

bool InterfaceFormController::msaa()
{
    return mUi.radMSAA->isChecked();
}

void InterfaceFormController::setMsaa(bool msaa)
{
    mUi.radMSAA->setChecked(msaa);
}

void InterfaceFormController::setSettingsModel(SqlSettingsModel* handle)
{
    mSettingsModelHandle = handle;
    initialise();
}

void InterfaceFormController::onProfileChanged(Profile* profile)
{
    qDebug() << "InterfaceFormController: onProfileChanged";
    initialise();
}

void InterfaceFormController::onLineWidthValueChanged(QString value)
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: LineWidth" << value.toFloat();
        auto settings = getSettings();
        settings->setLineWidth(value.toFloat());
        commit();
    }
}

void InterfaceFormController::onFpsCurrentIndexChanged(QString value)
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: fps";
        auto settings = getSettings();
        settings->setFpsLock(value.toInt());
        commit();
    }
}

void InterfaceFormController::onAntiAliasingToggled(bool value)
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: alntialiasing";
        auto settings = getSettings();
        settings->setAntiAliasing(value);
        commit();
    }
}

void InterfaceFormController::onMsaaToggled(bool value)
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: Msaa";
        auto settings = getSettings();
        settings->setMsaa(value);
        commit();
    }
}

void InterfaceFormController::onVsyncToggled(bool value)
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: Vsync";
        auto settings = getSettings();
        settings->setVsync(value);
        commit();
    }
}

void InterfaceFormController::onZBufferToggled(bool value)
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: zbuffer";
        auto settings = getSettings();
        settings->setZbuffer(value);
        commit();
    }
}

void InterfaceFormController::onShowUiCommandsToggled(bool value)
{
        if (isModelValid())
    {
        qDebug() << "InterfaceFormController: show ui commands";
        auto settings = getSettings();
        settings->setShowUiCommands(value);
        commit();
    }
}

void InterfaceFormController::onVisualiserColorButtonClicked()
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: visualiser Color";
        auto settings = getSettings();
        settings->setVisualiserColor(QColorDialog::getColor().name());
        commit();
    }
}

void InterfaceFormController::onBackgroundColorButtonClicked()
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: background Color";
        auto settings = getSettings();
        settings->setBackgroundColor(QColorDialog::getColor().name());
        commit();
    }
}

void InterfaceFormController::onToolColorButtonClicked()
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: Tool Color";
        auto settings = getSettings();
        settings->setToolColor(QColorDialog::getColor().name());
        commit();
    }
}

void InterfaceFormController::onToolPathColorButtonClicked()
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: ToolPath Color";
        auto settings = getSettings();
        settings->setToolPathColor(QColorDialog::getColor().name());
        commit();
    }
}

void InterfaceFormController::onNormalColorButtonClicked()
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: Normal Color";
        auto settings = getSettings();
        settings->setNormalColor(QColorDialog::getColor().name());
        commit();
    }
}

void InterfaceFormController::onHighlightColorButtonClicked()
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: Highlight Color";
        auto settings = getSettings();
        settings->setHighlightColor(QColorDialog::getColor().name());
        commit();
    }
}

void InterfaceFormController::onStartPointColorButtonClicked()
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: StartPoint Color";
        auto settings = getSettings();
        settings->setStartPointColor(QColorDialog::getColor().name());
        commit();
    }
}

void InterfaceFormController::onEndPointColorButtonClicked()
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: EndPoint Color";
        auto settings = getSettings();
        settings->setEndPointColor(QColorDialog::getColor().name());
        commit();
    }
}

void InterfaceFormController::onTextColorButtonClicked()
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: TextColor Color";
        auto settings = getSettings();
        settings->setTextColor(QColorDialog::getColor().name());
        commit();
    }
}

void InterfaceFormController::onDrawnColorButtonClicked()
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: Drawn Color";
        auto settings = getSettings();
        settings->setDrawnColor(QColorDialog::getColor().name());
        commit();
    }
}

void InterfaceFormController::onZMovementColorButtonClicked()
{
    if (isModelValid())
    {
        qDebug() << "InterfaceFormController: ZMovement Color";
        auto settings = getSettings();
        settings->setZMovementColor(QColorDialog::getColor().name());
        commit();
    }
}

InterfaceSettings* InterfaceFormController::getSettings()
{
    return mSettingsModelHandle
            ->getCurrentProfileHandle()
            ->getInterfaceSettingsHandle();
}

bool InterfaceFormController::isModelValid()
{
    if (!mSettingsModelHandle)
    {
        qDebug() << "InterfaceFormController: mSettingsModel = nullptr";
        return false;
    }

    Profile* profile = mSettingsModelHandle->getCurrentProfileHandle();

    if(!profile)
    {
        qDebug() << "InterfaceFormController: profile = nullptr";
        return false;
    }

    InterfaceSettings* settings = profile->getInterfaceSettingsHandle();

    if (!settings)
    {
        qDebug() << "InterfaceFormController: settings = nullptr";
        return false;
    }
    return true;
}

void InterfaceFormController::commit()
{
    auto settings = getSettings();
    mSettingsModelHandle->onInterfaceSettingsUpdated(settings);
}
