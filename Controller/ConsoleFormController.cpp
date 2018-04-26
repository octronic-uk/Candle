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

void ConsoleFormController::onCmdClearConsoleClicked()
{
    mUi.txtConsole->clear();
}

void ConsoleFormController::setFormActive(bool active)
{
    mUi.cboCommand->setEnabled(active);
    mUi.consoleClearButton->setEnabled(active);
    mUi.commandSendButton->setEnabled(active);
}

void ConsoleFormController::initialise()
{
    mUi.txtConsole->clear();
}


// Console
void ConsoleFormController::onCommandSendAction()
{
    QString commandText = mUi.cboCommand->currentText();
    if (commandText.isEmpty())
    {
        return;
    }
    // TODO - Store in combobox history
    mUi.cboCommand->setCurrentText("");
    // TODO - Leak Spin
    emit gcodeCommandSendSignal(new GcodeCommand(commandText));
}

void ConsoleFormController::onAppendResponseToConsole(const GrblResponse& response)
{
    QString txt = response.getData();
    if (txt.isEmpty())
    {
        return;
    }
    //qDebug() << "ConsoleFormController: Appending Response:" << txt;
    mUi.txtConsole->setTextColor(QColor("Gray"));
    mUi.txtConsole->append("CNC --> "+txt);
}

void ConsoleFormController::onAppendCommandToConsole(GcodeCommand* command)
{
    QString cmd = command->getCommand();
    //qDebug() << "ConsoleFormController: Appending Command:" << cmd;
    mUi.txtConsole->setTextColor(QColor("Black"));
    mUi.txtConsole->append("CNC <-- "+cmd.trimmed());
}

void ConsoleFormController::setupSignalSlots()
{
    //qDebug() << "ConsoleFormController: Setup Signals/Slots";
    connect(
        mUi.commandSendButton, SIGNAL(clicked()),
        this, SLOT(onCommandSendAction())
    );
}
