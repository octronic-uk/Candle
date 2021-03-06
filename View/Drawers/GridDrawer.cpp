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
      mMajorColor(QColor("#333333")),
      mMinorColor(QColor("#CCCCCC")),
      mTextColor(QColor(255,255,255))
{
    mLineWidth = 2;
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
    QVector<VertexData> majorLines;
    QVector<VertexData> minorLines;

    mLines.clear();

    float xStart = 0;
    float xEnd = mSize.x();

    float yStart = 0;
    float yEnd = mSize.y();

    qDebug() << "GridDrawer: xStart" << xStart << "xEnd" << xEnd
             << "yStart" << yStart << "yEnd" << yEnd;

    // X Lines
    // -----
    // -----
    // -----
    for (float yPos = yStart; yPos <= yEnd; yPos += mMinorSpacing)
    {
        QVector4D color;
        if (fmod(abs(yPos), mMajorSpacing) == 0.0f)
        {
            color = Util::colorToVector(mMajorColor);
            majorLines.append({QVector3D(xStart, yPos, 0), color, QVector3D(sNan, sNan, sNan)});
            majorLines.append({QVector3D(xEnd, yPos, 0),color,QVector3D(sNan, sNan, sNan)});
        }
        else
        {
            color = Util::colorToVector(mMinorColor);
            minorLines.append({QVector3D(xStart, yPos, 0), color, QVector3D(sNan, sNan, sNan)});
            minorLines.append({QVector3D(xEnd, yPos, 0),color,QVector3D(sNan, sNan, sNan)});
        }
    }

    // Y Lines
    // |  |  |
    // |  |  |
    // |  |  |
    for (float xPos = xStart; xPos <= xEnd; xPos += mMinorSpacing)
    {
        QVector4D color;
        if (fmod(abs(xPos), mMajorSpacing) == 0.0f)
        {
            color = Util::colorToVector(mMajorColor);
            majorLines.append({QVector3D(xPos, yStart, 0),color,QVector3D(sNan, sNan, sNan)});
            majorLines.append({QVector3D(xPos, yEnd, 0),color,QVector3D(sNan, sNan, sNan)});
        }
        else
        {
            color = Util::colorToVector(mMinorColor);
            minorLines.append({QVector3D(xPos, yStart, 0),color,QVector3D(sNan, sNan, sNan)});
            minorLines.append({QVector3D(xPos, yEnd, 0),color,QVector3D(sNan, sNan, sNan)});
        }
    }

    // Draw major lines after minor lines so they're on top
    mLines.append(minorLines);
    mLines.append(majorLines);

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
    mNeedsUpdateGeometry = true;
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
