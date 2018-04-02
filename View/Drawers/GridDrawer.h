/*
 * GridDrawer.h
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

#pragma once

#include "ShaderDrawable.h"

class GridDrawer : public ShaderDrawable
{
public:
    GridDrawer(float size = 100, float major = 10, float minor = 1);
    virtual ~GridDrawer() override;

    // ShaderDrawable interface
public:
    QVector3D getSizes() override;
    QVector3D getMinimumExtremes() override;
    QVector3D getMaximumExtremes() override;
    int getVertexCount() override;

    QColor getMajorColor() const;
    void setMajorColor(const QColor& majorColor);

    QColor getMinorColor() const;
    void setMinorColor(const QColor& minorColor);

    QColor getTextColor() const;
    void setTextColor(const QColor& borderColor);

    float getMajorSpacing() const;
    void setMajorSpacing(float majorSpacing);

    float getMinorSpacing() const;
    void setMinorSpacing(float minorSpacing);

    float getSize() const;
    void setSize(float size);

    int getOpacity() const;
    void setOpacity(int opacity);

    void updateOpacity();

protected:
    bool updateData() override;

private:
    float mMajorSpacing;
    float mMinorSpacing;
    float mSize;
    int mOpacity;
    QColor mMajorColor;
    QColor mMinorColor;
    QColor mTextColor;
};
