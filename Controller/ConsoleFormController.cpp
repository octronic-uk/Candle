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
    QMessageLogger().info("Constructing ConsoleFormController");
    mUi.setupUi(this);
    /*

#ifndef UNIX
    mUi.cboCommand->setStyleSheet("QComboBox {padding: 2;} QComboBox::drop-down {width: 0; border-style: none;} QComboBox::down-arrow {image: url(noimg);	border-width: 0;}");
#endif
    mSendMenu.addAction(tr("Send from current line"), this, SLOT(onActSendFromLineTriggered()));
    connect(mUi.cboCommand, SIGNAL(returnPressed()), this, SLOT(onCboCommandReturnPressed()));

 // Console window handling
    connect(
        mUi.grpConsole, SIGNAL(resized(QSize)),
        this, SLOT(onConsoleResized(QSize))
    );
    */
}

ConsoleFormController::~ConsoleFormController()
{
    QMessageLogger().info("Destructing ConsoleFormController");


}
void ConsoleFormController::onComboBoxCommandReturnPressed()
{
    /*
    QString command = mUi.cboCommand->currentText();
    if (command.isEmpty()) return;

    mUi.cboCommand->setCurrentText("");
    sendCommand(command, -1);
    */
}



void ConsoleFormController::onConsoleResized(QSize size)
{
    /*
    Q_UNUSED(size)

    int minHeight = getConsoleMinHeight();
    bool visible = mUi.grpConsole->height() > minHeight;
    if (mUi.txtConsole->isVisible() != visible)
    {
        mUi.txtConsole->setVisible(visible);
    }
    */
}

int ConsoleFormController::getConsoleMinHeight()
{
    /*
    return
        mUi.grpConsole->height() -
        mUi.grpConsole->contentsRect().height() +
        mUi.spacerConsole->geometry().height() +
        mUi.grpConsole->layout()->margin() * 2 +
        mUi.cboCommand->height();
        */
    return -1;
}

void ConsoleFormController::onCmdClearConsoleClicked()
{
    mUi.txtConsole->clear();
}


// Console
void ConsoleFormController::onCmdCommandSendClicked()
{
    /*
    QString command = mUi->cboCommand->currentText();
    if (command.isEmpty()) return;

    mUi->cboCommand->storeText();
    mUi->cboCommand->setCurrentText("");
    sendCommand(command, -1);
    */
}

