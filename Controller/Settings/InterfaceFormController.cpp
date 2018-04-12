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

InterfaceFormController::InterfaceFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    mUi.setupUi(this);
}

InterfaceFormController::~InterfaceFormController() {}

void InterfaceFormController::setupSignalSlots()
{
    // Visualiser
    connect(mUi.txtLineWidth, SIGNAL(valueChanged(QString)), this, SLOT(onLineWidthValueChanged(QString)));
    connect(mUi.cboFps, SIGNAL(currentIndexChanged(QString)), this, SLOT(onFpsCurrentIndexChanged(QString)));
    connect(mUi.chkAntialiasing, SIGNAL(toggled(bool)), this, SLOT(onAntiAliasingToggled(bool)));
    connect(mUi.radMSAA, SIGNAL(toggled(bool)), this, SLOT(onMsaaToggled(bool)));
    connect(mUi.chkVSync, SIGNAL(toggled(bool)), this, SLOT(onVsyncToggled(bool)));
    connect(mUi.chkZBuffer, SIGNAL(toggled(bool)), this, SLOT(onZBufferToggled(bool)));
    connect(mUi.radDrawModeVectors, SIGNAL(toggled(bool)), this, SLOT(onDrawModeVectorsToggled(bool)));
    connect(mUi.radDrawModeRaster,  SIGNAL(toggled(bool)), this, SLOT(onDrawModeRasterToggled(bool)));
    connect(mUi.chkSimplify, SIGNAL(toggled(bool)), this, SLOT(onSimplifyToggled(bool)));
    connect(mUi.txtSimplifyPrecision, SIGNAL(valueChanged(QString)), this, SLOT(onSimplifyPrecisionValueChanged(QString)));
    connect(mUi.chkGrayscale, SIGNAL(toggled(bool)), this, SLOT(onGrayscaleToggled(bool)));
    connect(mUi.radGrayscaleS, SIGNAL(toggled(bool)), this, SLOT(onGrayscaleSToggled(bool)));
    connect(mUi.radGrayscaleZ, SIGNAL(toggled(bool)), this, SLOT(onGrayscaleZToggled(bool)));
    // Console
    connect(mUi.chkShowProgramCommands, SIGNAL(toggled(bool)), this, SLOT(onShowProgramCommandsToggled(bool)));
    connect(mUi.chkShowUICommands, SIGNAL(toggled(bool)), this, SLOT(onShowUiCommandsToggled(bool)));
    connect(mUi.chkAutocompletion, SIGNAL(toggled(bool)), this, SLOT(onAutoCompletionToggled(bool)));
    // Colors
}

void InterfaceFormController::setFormActive(bool)
{

}

void InterfaceFormController::initialise()
{
    setLineWidth(1.5);
    setAntialiasing(true);
    setMsaa(true);
    setSimplify(true);
    setSimplifyPrecision(0.0);
    setFps(60);
    setZBuffer(false);
    setGrayscaleSegments(false);
    setGrayscaleSCode(true);
    setDrawModeVectors(true);
    setShowProgramCommands(false);
    setAutoCompletion(true);
}

void InterfaceFormController::onDrawModeVectorsToggled(bool checked)
{
    mUi.chkSimplify->setEnabled(checked);
    mUi.lblSimpilyPrecision->setEnabled(checked && mUi.chkSimplify->isChecked());
    mUi.txtSimplifyPrecision->setEnabled(checked && mUi.chkSimplify->isChecked());
    mUi.radDrawModeRaster->setChecked(!checked);
}

void InterfaceFormController::onDrawModeRasterToggled(bool checked)
{
    mUi.radDrawModeVectors->setChecked(!checked);
}

void InterfaceFormController::onGrayscaleSToggled(bool checked)
{
    mUi.radGrayscaleZ->setChecked(!checked);
}

void InterfaceFormController::onGrayscaleZToggled(bool checked)
{
    mUi.radGrayscaleS->setChecked(!checked);
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

bool InterfaceFormController::showProgramCommands()
{
    return mUi.chkShowProgramCommands->isChecked();
}

void InterfaceFormController::setShowProgramCommands(bool showAllCommands)
{
    mUi.chkShowProgramCommands->setChecked(showAllCommands);
}

bool InterfaceFormController::showUICommands()
{
    return mUi.chkShowUICommands->isChecked();
}

void InterfaceFormController::setShowUICommands(bool showUICommands)
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

bool InterfaceFormController::autoCompletion()
{
    return mUi.chkAutocompletion->isChecked();
}

void InterfaceFormController::setAutoCompletion(bool autoCompletion)
{
    mUi.chkAutocompletion->setChecked(autoCompletion);
}

bool InterfaceFormController::simplify()
{
    return mUi.chkSimplify->isChecked();
}

void InterfaceFormController::setSimplify(bool simplify)
{
    mUi.chkSimplify->setChecked(simplify);
}

double InterfaceFormController::simplifyPrecision()
{
    return mUi.txtSimplifyPrecision->value();
}

void InterfaceFormController::setSimplifyPrecision(double simplifyPrecision)
{
    mUi.txtSimplifyPrecision->setValue(simplifyPrecision);
}

bool InterfaceFormController::grayscaleSegments()
{
    return mUi.chkGrayscale->isChecked();
}

void InterfaceFormController::setGrayscaleSegments(bool value)
{
    mUi.chkGrayscale->setChecked(value);
}

bool InterfaceFormController::grayscaleSCode()
{
    return mUi.radGrayscaleS->isChecked();
}

void InterfaceFormController::setGrayscaleSCode(bool value)
{
    mUi.radGrayscaleS->setChecked(value);
    mUi.radGrayscaleZ->setChecked(!value);
}

bool InterfaceFormController::drawModeVectors()
{
    return mUi.radDrawModeVectors->isChecked();
}

void InterfaceFormController::setDrawModeVectors(bool value)
{
    mUi.radDrawModeVectors->setChecked(value);
    mUi.radDrawModeRaster->setChecked(!value);
}

void InterfaceFormController::setSettingsModel(SqlSettingsModel* handle)
{
    mSettingsModelHandle = handle;
}

void InterfaceFormController::onProfileChanged(Profile* profile)
{

}

void InterfaceFormController::onLineWidthValueChanged(QString)
{

}

void InterfaceFormController::onFpsCurrentIndexChanged(QString)
{

}

void InterfaceFormController::onAntiAliasingToggled(bool)
{

}

void InterfaceFormController::onMsaaToggled(bool)
{

}

void InterfaceFormController::onVsyncToggled(bool)
{

}

void InterfaceFormController::onZBufferToggled(bool)
{

}

void InterfaceFormController::onSimplifyToggled(bool value)
{
}

void InterfaceFormController::onSimplifyPrecisionValueChanged(QString value)
{
}

void InterfaceFormController::onGrayscaleToggled(bool value)
{
}

void InterfaceFormController::onShowProgramCommandsToggled(bool value)
{
}

void InterfaceFormController::onShowUiCommandsToggled(bool value)
{
}

void InterfaceFormController::onAutoCompletionToggled(bool value)
{
}
