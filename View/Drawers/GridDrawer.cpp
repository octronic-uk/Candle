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
      mSize(size,size,size),
      mMajorColor(QColor("Black")),
      mMinorColor(QColor("DarkGray")),
      mTextColor(QColor(255,255,255))
{
    mLineWidth = 1;
    mMajorColor.setAlpha(255);
    mMinorColor.setAlpha(255);
    mTextColor.setAlpha(255);
}

GridDrawer::~GridDrawer()
{

}

QVector3D GridDrawer::getSizes()
{
    return QVector3D();
}

QVector3D GridDrawer::getMinimumExtremes() const
{
    return QVector3D();

}

QVector3D GridDrawer::getMaximumExtremes() const
{
    return QVector3D();
}

int GridDrawer::getVertexCount()
{
    return mLines.size()*2;
}

bool GridDrawer::updateData()
{
    // TODO - support rectangle sizes
    mLines.clear();

    float start = 0;//-(mSize/2);
    float end = mSize.x();//(mSize/2);

    // X Lines
    for (float yPos = start; yPos <= end; yPos += mMinorSpacing)
    {
        QVector4D color = fmod(abs(yPos), mMajorSpacing) == 0.0f ?
            Util::colorToVector(mMajorColor) :
            Util::colorToVector(mMinorColor);

        mLines.append({QVector3D(start, yPos, 0), color, QVector3D(sNan, sNan, sNan)});
        mLines.append({QVector3D(end, yPos, 0),color,QVector3D(sNan, sNan, sNan)});
    }

    // Y Lines
    for (float xPos = start; xPos <= end; xPos += mMinorSpacing)
    {
        QVector4D color = fmod(abs(xPos), mMajorSpacing) == 0.0f ?
            Util::colorToVector(mMajorColor) :
            Util::colorToVector(mMinorColor);

        mLines.append({QVector3D(xPos, start, 0),color,QVector3D(sNan, sNan, sNan)});
        mLines.append({QVector3D(xPos, end, 0),color,QVector3D(sNan, sNan, sNan)});
    }

   return true;
}

QVector3D GridDrawer::getSize() const
{
    return mSize;
}

void GridDrawer::setSize(QVector3D size)
{
    mSize = size;
}

void GridDrawer::onSizeUpdated(QVector3D size)
{
    setSize(size);
    updateData();
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
