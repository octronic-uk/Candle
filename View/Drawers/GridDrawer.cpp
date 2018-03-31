/*
 * GridDrawer.cpp
 *
 * Created: 31 2018 by Ashley
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
#include "GridDrawer.h"
#include "Utils/Util.h"
#include <math.h>

GridDrawer::GridDrawer(float size, float major, float minor)
    : ShaderDrawable (),
      mMajorSpacing(major),
      mMinorSpacing(minor),
      mSize(size),
      mOpacity(128),
      mMajorColor(QColor("blue")),
      mMinorColor(QColor("darkBlue")),
      mTextColor(QColor("white"))
{
    mLineWidth = 3;
}

GridDrawer::~GridDrawer()
{

}

QVector3D GridDrawer::getSizes()
{
    return QVector3D();
}

QVector3D GridDrawer::getMinimumExtremes()
{
    return QVector3D();

}

QVector3D GridDrawer::getMaximumExtremes()
{
    return QVector3D();
}

int GridDrawer::getVertexCount()
{
    return mLines.size();
}

bool GridDrawer::updateData()
{
    mLines.clear();
    updateOpacity();

    float start = -(mSize/2);
    float end = (mSize/2);

    // X Lines
    for (float yPos = start; yPos <= end; yPos += mMinorSpacing)
    {
        QVector4D color =  fmod(abs(yPos), mMajorSpacing) == 0.0f ?
            Util::colorToVector4(mMajorColor) :
            Util::colorToVector4(mMinorColor);

        mLines.append({QVector3D(start, yPos, 0), color, QVector3D(sNan, sNan, sNan)});
        mLines.append({QVector3D(end, yPos, 0),color,QVector3D(sNan, sNan, sNan)});
    }

    // Y Lines
    for (float xPos = start; xPos <= end; xPos += mMinorSpacing)
    {
        QVector4D color = fmod(abs(xPos), mMajorSpacing) == 0.0f ?
            Util::colorToVector4(mMajorColor) :
            Util::colorToVector4(mMinorColor);

        mLines.append({QVector3D(xPos, start, 0),color,QVector3D(sNan, sNan, sNan)});
        mLines.append({QVector3D(xPos, end, 0),color,QVector3D(sNan, sNan, sNan)});
    }

   return true;
}

int GridDrawer::getOpacity() const
{
    return mOpacity;
}

void GridDrawer::setOpacity(int opacity)
{
    mOpacity = opacity;
}

void GridDrawer::updateOpacity()
{
    mMajorColor.setAlpha(mOpacity);
    mMinorColor.setAlpha(mOpacity);
    mTextColor.setAlpha(mOpacity);
}

float GridDrawer::getSize() const
{
    return mSize;
}

void GridDrawer::setSize(float size)
{
    mSize = size;
}

float GridDrawer::getMinorSpacing() const
{
    return mMinorSpacing;
}

void GridDrawer::setMinorSpacing(float minorSpacing)
{
    mMinorSpacing = minorSpacing;
}

float GridDrawer::getMajorSpacing() const
{
    return mMajorSpacing;
}

void GridDrawer::setMajorSpacing(float majorSpacing)
{
    mMajorSpacing = majorSpacing;
}

QColor GridDrawer::getTextColor() const
{
    return mTextColor;
}

void GridDrawer::setTextColor(const QColor& borderColor)
{
    mTextColor = borderColor;
}

QColor GridDrawer::getMinorColor() const
{
    return mMinorColor;
}

void GridDrawer::setMinorColor(const QColor& minorColor)
{
    mMinorColor = minorColor;
}

QColor GridDrawer::getMajorColor() const
{
    return mMajorColor;
}

void GridDrawer::setMajorColor(const QColor& majorColor)
{
    mMajorColor = majorColor;
}
