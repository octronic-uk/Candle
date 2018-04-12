/*
 * ToolHolderGeometry.h
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

#pragma once

#include "Model/Settings/ToolHolder/ToolHolder.h"
#include "Model/Settings/Sql/AbstractDatabaseRecord.h"

class ToolHolder;

class ToolHolderGeometry : public AbstractDatabaseRecord
{
public:
    ToolHolderGeometry
    (
        ToolHolder* parent,
        int id=-1,
        int index = -1,
        float height = 1,
        float upper=1,
        float lower = 1
    );

    ToolHolderGeometry(const ToolHolderGeometry& other);

    bool operator==(const ToolHolderGeometry& other);

    float getUpperDiameter() const;
    void setUpperDiameter(float upperDiameter);

    float getLowerDiameter() const;
    void setLowerDiameter(float lowerDiameter);

    float getHeight() const;
    void setHeight(float height);

    int getIndex() const;
    void setIndex(int index);

    int getToolHolderID() const;

    ToolHolder* getParentHandle() const;

private:
    ToolHolder* mParentHandle;
    int mIndex;
    float mHeight;
    float mUpperDiameter;
    float mLowerDiameter;
};
