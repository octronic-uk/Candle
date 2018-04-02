/*
 * ConsoleForm.cpp
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

#include <QMenu>
#include "ConsoleFormController.h"
#include "ui_ConsoleForm.h"

using namespace Ui;

ConsoleFormController::ConsoleFormController(QWidget *parent)
    : AbstractFormController (parent)
{
    qDebug() << "Constructing ConsoleFormController";
    mUi.setupUi(this);
    setupSignalSlots();
}

ConsoleFormController::~ConsoleFormController()
{
    qDebug() << "Destructing ConsoleFormController";
}

void ConsoleFormController::onComboBoxCommandReturnPressed()
{
    QString command = mUi.cboCommand->currentText();
    if (command.isEmpty())
    {
        return;
    }

    mUi.cboCommand->setCurrentText("");
    emit commandSentSignal(command, -1);
}

void ConsoleFormController::onCmdClearConsoleClicked()
{
    mUi.txtConsole->clear();
}

void ConsoleFormController::setFormActive(bool active)
{
    mUi.cboCommand->setEnabled(active);
    mUi.cmdClearConsole->setEnabled(active);
    mUi.cmdCommandSend->setEnabled(active);
}


// Console
void ConsoleFormController::onCmdCommandSendClicked()
{
    QString command = mUi.cboCommand->currentText();
    if (command.isEmpty())
    {
        return;
    }

    // TODO - Store in combobox history
    mUi.cboCommand->setCurrentText("");
    emit commandSentSignal(command,-1);
}

void ConsoleFormController::onAppendToConsole(QString text)
{
    mUi.txtConsole->appendPlainText(text);
}

void ConsoleFormController::setupSignalSlots()
{
    qDebug() << "ConsoleFormController: Setup Signals/Slots";
    /* Signal does not exist
    connect(
        mUi.cboCommand, SIGNAL(returnPressed()),
        this, SLOT(onComboBoxCommandReturnPressed())
    );
    */
}

