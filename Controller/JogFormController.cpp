/*
 * JogFormController.cpp
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
#include "JogFormController.h"
#include <QPushButton>
#include <QtDebug>

JogFormController::JogFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "JogFormController: Constructing";
    mUi.setupUi(this);
    mUi.jogStepSpinner->setLocale(QLocale::C);
    setupSignalSlots();
}

JogFormController::~JogFormController()
{
    qDebug() << "JogFormController: Destructing";
}

void JogFormController::onCheckBoxKeyboardControlToggled(bool checked)
{
    /*
    mUi.grpJog->setProperty("overrided", checked);
    style()->unpolish(mUi.grpJog);
    mUi.grpJog->ensurePolished();

    // Store/restore coordinate system
    if (checked) {
        sendCommand("$G", -2, mSettingsForm->showUICommands());
        if (!mUi.grpJog->isChecked()) mUi.grpJog->setTitle(tr("Jog") + QString(tr(" (%1)")).arg(mUi.txtJogStep->text()));
    } else {
        if (m_absoluteCoordinates) sendCommand("G90", -1, mSettingsForm->showUICommands());
        mUi.grpJog->setTitle(tr("Jog"));
    }

    if (!m_processingFile) m_storedKeyboardControl = checked;

    updateControlsState();
    */
}


void JogFormController::blockJogForRapidMovement(bool repeated)
{
    Q_UNUSED(repeated)
    /*
    mJogBlock = true;

    const double acc = mSettingsForm->acceleration();    // Acceleration mm/sec^2
    double v = mSettingsForm->rapidSpeed() / 60;         // Rapid speed mm/sec
    double at = v / acc;                                // Acceleration time
    double s = acc * at * at / 2;                       // Distance on acceleration
    double time;
    double step = mUi.txtJogStep->text().toDouble();

    if (repeated) {
        time = step / v;
    } else {
        if (2 * s > step) {
            time = sqrt(step / acc);
        } else {
            time = (step - 2 * s) / v + 1 * at;
        }
    }

//    qDebug() << QString("acc: %1; v: %2; at: %3; s: %4; time: %5").arg(acc).arg(v).arg(at).arg(s).arg(time);
    QTimer::singleShot(time * 1000, Qt::PreciseTimer, this, SLOT(onJogTimer()));
    */
}


void JogFormController::onCmdYPlusClicked()
{
    // Query parser state to restore coordinate system, hide from table and console
//    sendCommand("$G", -2, mSettingsForm->showUICommands());
//    sendCommand("G91G0Y" + mUi.txtJogStep->text(), -1, mSettingsForm->showUICommands());
}

void JogFormController::onCmdYMinusClicked()
{
//    sendCommand("$G", -2, mSettingsForm->showUICommands());
//    sendCommand("G91G0Y-" + mUi.txtJogStep->text(), -1, mSettingsForm->showUICommands());
}

void JogFormController::onCmdXPlusClicked()
{
//    sendCommand("$G", -2, mSettingsForm->showUICommands());
//    sendCommand("G91G0X" + mUi.txtJogStep->text(), -1, mSettingsForm->showUICommands());
}

void JogFormController::onCmdXMinusClicked()
{
//    sendCommand("$G", -2, mSettingsForm->showUICommands());
//    sendCommand("G91G0X-" + mUi.txtJogStep->text(), -1, mSettingsForm->showUICommands());
}

void JogFormController::onCmdZPlusClicked()
{
//    sendCommand("$G", -2, mSettingsForm->showUICommands());
//    sendCommand("G91G0Z" + mUi.txtJogStep->text(), -1, mSettingsForm->showUICommands());
}

void JogFormController::onCmdZMinusClicked()
{
//    sendCommand("$G", -2, mSettingsForm->showUICommands());
//    sendCommand("G91G0Z-" + mUi.txtJogStep->text(), -1, mSettingsForm->showUICommands());
}


void JogFormController::onCmdJogStepClicked()
{
    /*
    mUi.txtJogStep->setValue(static_cast<QPushButton*>(sender())->text().toDouble());

    foreach (StyledToolButton* button, mUi.grpJog->findChildren<StyledToolButton*>(QRegExp("cmdJogStep\\d")))
    {
        button->setChecked(false);
    }
    static_cast<QPushButton*>(sender())->setChecked(true);
    */
}


void JogFormController::onJogTimer()
{
    mJogBlock = false;
}

// Jog

bool JogFormController::keyIsMovement(int key)
{
    return key == Qt::Key_4 || key == Qt::Key_6 || key == Qt::Key_8 || key == Qt::Key_2 || key == Qt::Key_9 || key == Qt::Key_3;
}

void JogFormController::setupSignalSlots()
{

    qDebug() << "JogFormController: Setup Signals/Slots";
}

