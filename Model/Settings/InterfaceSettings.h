/*
 * InterfaceSettings.h
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

#pragma once

#include "Model/Settings/Sql/AbstractDatabaseRecord.h"

class Profile;

class InterfaceSettings : public AbstractDatabaseRecord
{
public:
    InterfaceSettings(Profile* parent, int id = -1);

    Profile* getParentHandle() const;
    int getParentID();

    float getLineWidth() const;
    void setLineWidth(float lineWidth);

    int getFpsLock() const;
    void setFpsLock(int fpsLock);

    bool getAntiAliasing() const;
    void setAntiAliasing(bool antiAliasing);

    bool getVsync() const;
    void setVsync(bool vsync);

    bool getMsaa() const;
    void setMsaa(bool msaa);

    bool getZbuffer() const;
    void setZbuffer(bool zbuffer);

    bool getGcodeDrawMode() const;
    void setGcodeDrawMode(bool gcodeDrawMode);

    bool getSimplifyGeometry() const;
    void setSimplifyGeometry(bool simplifyGeometry);

    float getSimplifyPrecision() const;
    void setSimplifyPrecision(float simplifyPrecision);

    bool getGrayscaleSegments() const;
    void setGrayscaleSegments(bool grayscaleSegments);

    bool getGrayscaleCode() const;
    void setGrayscaleCode(bool grayscaleCode);

private:
    Profile* mParentHandle;
    float mLineWidth;
    int mFpsLock;
    bool mAntiAliasing;
    bool mVsync;
    bool mMsaa;
    bool mZbuffer;
    bool mGcodeDrawMode;
    bool mSimplifyGeometry;
    float mSimplifyPrecision;
    bool mGrayscaleSegments;
    bool mGrayscaleCode;
};
