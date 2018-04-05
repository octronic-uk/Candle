/*
 * StateFormController.cpp
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
#include "StateFormController.h"


StateFormController::StateFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "StateFormController: Constructing";
    mUi.setupUi(this);
    setupSignalSlots();
    mUi.statusText->setAutoFillBackground(false);

    QFont font(mUi.statusText->font());
    font.setBold(true);
    mUi.statusText->setFont(font);
}

StateFormController::~StateFormController()
{
    qDebug() << "StateFormController: Destructing";

}

void StateFormController::onStatusTextUpdate(QString text, QColor textColor, QColor bgColor)
{
    qDebug() << "StateFormController:" << text << textColor << bgColor;
    mUi.statusText->setText(text);
    QPalette p = mUi.statusText->palette();
    p.setColor(mUi.statusText->foregroundRole(), textColor);
    p.setColor(mUi.statusText->backgroundRole(), bgColor);
    mUi.statusText->setPalette(p);
}

void StateFormController::setFormActive(bool active)
{

}

void StateFormController::initialise()
{

}

void StateFormController::setupSignalSlots()
{

    qDebug() << "StateFormController: Setup Signals/Slots";
}


