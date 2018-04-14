/*
 * InterfaceSettings.cpp
 *
 * Created: 10 2018 by Ashley
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

#include "InterfaceSettings.h"
#include "Model/Settings/Profile/Profile.h"
#include <QtDebug>

InterfaceSettings::InterfaceSettings(Profile* parent, int id)
    : AbstractDatabaseRecord(id),
      mParentHandle(parent),
      mLineWidth(0.0f),
      mFpsLock(60),
      mAntiAliasing(true),
      mVsync(true),
      mMsaa(true),
      mZbuffer(true),
      mShowUiCommands(false),
      mVisualiserColor(""),
      mBackgroundColor(""),
      mToolColor(""),
      mToolPathColor(""),
      mNormalColor(""),
      mHighlightColor(""),
      mStartPointColor(""),
      mEndPointColor(""),
      mTextColor(""),
      mDrawnColor(""),
      mZMovementColor("")

{
    qDebug() << "InterfaceSettings: Constructing";
}

Profile* InterfaceSettings::getParentHandle() const
{
    return mParentHandle;
}

int InterfaceSettings::getParentID()
{
   return mParentHandle->getID();
}

float InterfaceSettings::getLineWidth() const
{
    return mLineWidth;
}

void InterfaceSettings::setLineWidth(float lineWidth)
{
    mLineWidth = lineWidth;
}

int InterfaceSettings::getFpsLock() const
{
    return mFpsLock;
}

void InterfaceSettings::setFpsLock(int fpsLock)
{
    mFpsLock = fpsLock;
}

bool InterfaceSettings::getAntiAliasing() const
{
    return mAntiAliasing;
}

void InterfaceSettings::setAntiAliasing(bool antiAliasing)
{
    mAntiAliasing = antiAliasing;
}

bool InterfaceSettings::getVsync() const
{
    return mVsync;
}

void InterfaceSettings::setVsync(bool vsync)
{
    mVsync = vsync;
}

bool InterfaceSettings::getMsaa() const
{
    return mMsaa;
}

void InterfaceSettings::setMsaa(bool msaa)
{
    mMsaa = msaa;
}

bool InterfaceSettings::getZbuffer() const
{
    return mZbuffer;
}

void InterfaceSettings::setZbuffer(bool zbuffer)
{
    mZbuffer = zbuffer;
}

bool InterfaceSettings::getShowUiCommands() const
{
    return mShowUiCommands;
}

void InterfaceSettings::setShowUiCommands(bool show)
{
    mShowUiCommands = show;
}

QString InterfaceSettings::getVisualiserColor() const
{
    return mVisualiserColor;
}

void InterfaceSettings::setVisualiserColor(const QString& visualiserColor)
{
    mVisualiserColor = visualiserColor;
}

QString InterfaceSettings::getBackgroundColor() const
{
    return mBackgroundColor;
}

void InterfaceSettings::setBackgroundColor(const QString& backgroundColor)
{
    mBackgroundColor = backgroundColor;
}

QString InterfaceSettings::getToolColor() const
{
    return mToolColor;
}

void InterfaceSettings::setToolColor(const QString& toolColor)
{
    mToolColor = toolColor;
}

QString InterfaceSettings::getToolPathColor() const
{
    return mToolPathColor;
}

void InterfaceSettings::setToolPathColor(const QString& toolPathColor)
{
    mToolPathColor = toolPathColor;
}

QString InterfaceSettings::getNormalColor() const
{
    return mNormalColor;
}

void InterfaceSettings::setNormalColor(const QString& normalColor)
{
    mNormalColor = normalColor;
}

QString InterfaceSettings::getHighlightColor() const
{
    return mHighlightColor;
}

void InterfaceSettings::setHighlightColor(const QString& highlightColor)
{
    mHighlightColor = highlightColor;
}

QString InterfaceSettings::getStartPointColor() const
{
    return mStartPointColor;
}

void InterfaceSettings::setStartPointColor(const QString& startPointColor)
{
    mStartPointColor = startPointColor;
}

QString InterfaceSettings::getEndPointColor() const
{
    return mEndPointColor;
}

void InterfaceSettings::setEndPointColor(const QString& endPointColor)
{
    mEndPointColor = endPointColor;
}

QString InterfaceSettings::getTextColor() const
{
    return mTextColor;
}

void InterfaceSettings::setTextColor(const QString& textColor)
{
    mTextColor = textColor;
}

QString InterfaceSettings::getDrawnColor() const
{
    return mDrawnColor;
}

void InterfaceSettings::setDrawnColor(const QString& drawnColor)
{
    mDrawnColor = drawnColor;
}

QString InterfaceSettings::getZMovementColor() const
{
    return mZMovementColor;
}

void InterfaceSettings::setZMovementColor(const QString& zMovementColor)
{
    mZMovementColor = zMovementColor;
}
