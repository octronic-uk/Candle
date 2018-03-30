/*
 * VisualisationFormController.h
 *
 * Created: 22 2018 by Ashley
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

#include "AbstractFormController.h"
#include "ui_VisualisationForm.h"


#include "View/Drawers/OriginDrawer.h"
#include "View/Drawers/GcodeDrawer.h"
#include "View/Drawers/ToolDrawer.h"
#include "View/Drawers/SelectionDrawer.h"
#include "View/Drawers/HeightMapGridDrawer.h"
#include "View/Drawers/HeightMapBorderDrawer.h"
#include "View/Drawers/HeightMapInterpolationDrawer.h"
#include "Model/GcodeItem.h"

using namespace Ui;

class VisualisationFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit VisualisationFormController(QWidget *parent = nullptr);
    ~VisualisationFormController() override;
signals:
    void spindleEnabledSignal(bool);
public slots:
    void onFitButtonClicked();
    void onTopButtonClicked();
    void onFrontButtonClicked();
    void onLeftButtonClicked();
    void onCmdIsometricClicked();
    double toolZPosition();
    QString getParserStatus();
    GLWidget* getVisualiser();
    void setGLWBufferState(QString state); // glwBuffer.setBufferState
    void placeVisualizerButtons();
    void onVisualizatorRotationChanged();

    void timerEvent(QTimerEvent* te) override;
    void showEvent(QShowEvent* se) override;
    void hideEvent(QHideEvent* he) override;
    void resizeEvent(QResizeEvent* re) override;
    void onGcodeFileLoadStarted();
    void onGcodeFileLoadFinished(QList<GcodeItem>);
    void onGcodeParserUpdated(GcodeParser*);
private slots:

private:
    int mButtonPadding;
    VisualisationForm mUi;

    // GL Drawers
    GcodeDrawer mCodeDrawer;
    GcodeDrawer mProbeDrawer;
    OriginDrawer mOriginDrawer;
    ToolDrawer mToolDrawer;
    SelectionDrawer mSelectionDrawer;
    HeightMapBorderDrawer mHeightMapBorderDrawer;
    HeightMapGridDrawer mHeightMapGridDrawer;
    HeightMapInterpolationDrawer mHeightMapInterpolationDrawer;

    // Parsers
    GcodeViewParse mViewParser;
    GcodeViewParse mProbeParser;
    QBasicTimer mToolAnimationTimer;
    int mLastDrawnLineIndex;
    void updateParser();
    bool mSpindleClockwise;

public:
    void setupSignalSlots() override;
};