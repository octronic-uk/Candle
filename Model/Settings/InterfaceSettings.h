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
#include <QString>

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

    bool getShowUiCommands() const;
    void setShowUiCommands(bool showUiCommands);

    bool getSimplifyGeometry() const;
    void setSimplifyGeometry(bool simplifyGeometry);

    float getSimplifyPrecision() const;
    void setSimplifyPrecision(float simplifyPrecision);

    bool getGrayscaleSegments() const;
    void setGrayscaleSegments(bool grayscaleSegments);

    bool getGrayscaleCode() const;
    void setGrayscaleCode(bool grayscaleCode);

    QString getVisualiserColor() const;
    void setVisualiserColor(const QString& visualiserColor);

    QString getBackgroundColor() const;
    void setBackgroundColor(const QString& backgroundColor);

    QString getToolColor() const;
    void setToolColor(const QString& toolColor);

    QString getToolPathColor() const;
    void setToolPathColor(const QString& toolPathColor);

    QString getNormalColor() const;
    void setNormalColor(const QString& normalColor);

    QString getHighlightColor() const;
    void setHighlightColor(const QString& highlightColor);

    QString getStartPointColor() const;
    void setStartPointColor(const QString& startPointColor);

    QString getEndPointColor() const;
    void setEndPointColor(const QString& endPointColor);

    QString getTextColor() const;
    void setTextColor(const QString& textColor);

    QString getDrawnColor() const;
    void setDrawnColor(const QString& drawnColor);

    QString getZMovementColor() const;
    void setZMovementColor(const QString& zMovementColor);

private:
    Profile* mParentHandle;
    float mLineWidth;
    int mFpsLock;
    bool mAntiAliasing;
    bool mVsync;
    bool mMsaa;
    bool mZbuffer;
    bool mShowUiCommands;
    QString mVisualiserColor;
    QString mBackgroundColor;
    QString mToolColor;
    QString mToolPathColor;
    QString mNormalColor;
    QString mHighlightColor;
    QString mStartPointColor;
    QString mEndPointColor;
    QString mTextColor;
    QString mDrawnColor;
    QString mZMovementColor;

};
