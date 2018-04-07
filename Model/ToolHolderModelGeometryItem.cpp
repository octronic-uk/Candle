/*
 * ToolHolderModelGeometryItem.cpp
 *
 * Created: 06 2018 by Ashley
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
#include "ToolHolderModelGeometryItem.h"

ToolHolderModelGeometryItem::ToolHolderModelGeometryItem()
{

}

ToolHolderModelGeometryItem::ToolHolderModelGeometryItem
(float height, float upper, float lower)
    : mHeight(height),
      mUpperDiameter(upper),
      mLowerDiameter(lower)
{}

float ToolHolderModelGeometryItem::getUpperDiameter() const
{
    return mUpperDiameter;
}

void ToolHolderModelGeometryItem::setUpperDiameter(float upperDiameter)
{
    mUpperDiameter = upperDiameter <= 0 ? 1 : upperDiameter;
}

float ToolHolderModelGeometryItem::getLowerDiameter() const
{
    return mLowerDiameter;
}

void ToolHolderModelGeometryItem::setLowerDiameter(float lowerDiameter)
{
    mLowerDiameter = lowerDiameter <= 0 ? 1 : lowerDiameter;
}

float ToolHolderModelGeometryItem::getHeight() const
{
    return mHeight;
}

void ToolHolderModelGeometryItem::setHeight(float height)
{
    mHeight = height <= 0 ? 1 : height;
}
