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

#include "ToolHolderGeometry.h"
#include <QtDebug>

ToolHolderGeometry::ToolHolderGeometry
(int id, int toolHolderId, int index, float height, float upper, float lower)
    : AbstractDatabaseRecord(id),
      mToolHolderId(toolHolderId),
      mIndex(index),
      mHeight(height),
      mUpperDiameter(upper),
      mLowerDiameter(lower)
{
    qDebug() << "ToolHolderModelGeometryItem: Creating with index" << index;
}

float ToolHolderGeometry::getUpperDiameter() const
{
    return mUpperDiameter;
}

void ToolHolderGeometry::setUpperDiameter(float upperDiameter)
{
    mUpperDiameter = upperDiameter <= 0 ? 1 : upperDiameter;
}

float ToolHolderGeometry::getLowerDiameter() const
{
    return mLowerDiameter;
}

void ToolHolderGeometry::setLowerDiameter(float lowerDiameter)
{
    mLowerDiameter = lowerDiameter <= 0 ? 1 : lowerDiameter;
}

float ToolHolderGeometry::getHeight() const
{
    return mHeight;
}

void ToolHolderGeometry::setHeight(float height)
{
    mHeight = height <= 0 ? 1 : height;
}

int ToolHolderGeometry::getIndex() const
{
    return mIndex;
}

void ToolHolderGeometry::setIndex(int index)
{
    mIndex = index;
}

int ToolHolderGeometry::getToolHolderID() const
{
    return mToolHolderId;
}

void ToolHolderGeometry::setToolHolderID(int toolHolderID)
{
   mToolHolderId = toolHolderID;
}
