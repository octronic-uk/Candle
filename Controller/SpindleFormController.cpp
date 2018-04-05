/*
 * SpindleFormController.cpp
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
#include "SpindleFormController.h"

SpindleFormController::SpindleFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "SpindleFormController: Constructing";
    mUi.setupUi(this);
    setupSignalSlots();
}

SpindleFormController::~SpindleFormController()
{
    qDebug() << "SpindleFormController: Destructing";

}

bool SpindleFormController::isUpdatingSpindleSpeed()
{

}

void SpindleFormController::setUpdatingSpindleSpeed(bool)
{

}

QString SpindleFormController::getSpindleSpeed()
{

}

void SpindleFormController::setFormActive(bool active)
{

}

void SpindleFormController::initialise()
{

}

void SpindleFormController::onCmdSpindleToggled(bool checked)
{
    /*
    mUi->grpSpindle->setProperty("overrided", checked);
    style()->unpolish(mUi->grpSpindle);
    mUi->grpSpindle->ensurePolished();

    if (checked) {
        if (!mUi->grpSpindle->isChecked()) mUi->grpSpindle->setTitle(tr("Spindle") + QString(tr(" (%1)")).arg(mUi->txtSpindleSpeed->text()));
    } else {
        mUi->grpSpindle->setTitle(tr("Spindle"));
    }
    */
}

void SpindleFormController::onCmdSpindleClicked(bool checked)
{
    //sendCommand(checked ? QString("M3 S%1").arg(mUi->txtSpindleSpeed->text()) : "M5", -1, mSettingsForm->showUICommands());
}

void SpindleFormController::onTextSpindleSpeedEditingFinished()
{
    /*
    mUi->txtSpindleSpeed->setStyleSheet("color: red;");
    mUi->sliSpindleSpeed->setValue(mUi->txtSpindleSpeed->value() / 100);
    m_updateSpindleSpeed = true;
    */
}

void SpindleFormController::onSliderSpindleSpeedValueChanged(int value)
{
    /*
    Q_UNUSED(value)

    mUi->txtSpindleSpeed->setStyleSheet("color: red;");

    if (!mUi->grpSpindle->isChecked() && mUi->cmdSpindle->isChecked())
        mUi->grpSpindle->setTitle(tr("Spindle") + QString(tr(" (%1)")).arg(mUi->txtSpindleSpeed->text()));
        */
}

void SpindleFormController::onSliderSpindleSpeedActionTriggered(int action)
{
    /*
    mUi->txtSpindleSpeed->setValue(mUi->sliSpindleSpeed->sliderPosition() * 100);
    m_updateSpindleSpeed = true;
    */
}

void SpindleFormController::setupSignalSlots()
{

    qDebug() << "SpindleFormController: Setup Signals/Slots";
}

