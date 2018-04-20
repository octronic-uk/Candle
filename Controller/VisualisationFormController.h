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
#include "View/Drawers/GridDrawer.h"
#include "View/Drawers/WorkAreaDrawer.h"
#include "View/Drawers/SelectionDrawer.h"
#include "Model/Gcode/GcodeCommand.h"

using namespace Ui;

class GcodeFileModel;
class SqlSettingsModel;

class VisualisationFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit VisualisationFormController(QWidget *parent = nullptr);
    ~VisualisationFormController() override;
    void setFormActive(bool active) override;
    void initialise() override;
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
    void onGcodeFileLoadFinished(GcodeFileModel*);
    void onGcodeParserUpdated(GcodeParser*);

    void onToggleGridButtonToggled(bool toggled);
    void onToggleToolButtonToggled(bool toggled);
    void onUpdateWorkPosition(const QVector3D);
    void onSettingsModelReady(SqlSettingsModel*);
    void onUpdateWCO(const QVector3D);
private slots:

    void onFollowToolButtonToggled(bool toggled);
private:
    int mButtonPadding;
    VisualisationForm mUi;

    // GL Drawers
    GcodeDrawer mCodeDrawer;
    GcodeDrawer mProbeDrawer;
    OriginDrawer mOriginDrawer;
    GridDrawer mGridDrawer;
    ToolDrawer mToolDrawer;
    //WorkAreaDrawer mWorkAreaDrawer;
    SelectionDrawer mSelectionDrawer;
    bool mFollowTool;

    // Parsers
    QSharedPointer<GcodeViewParser> mViewParser;
    QSharedPointer<GcodeViewParser> mProbeParser;
    QBasicTimer mToolAnimationTimer;
    int mLastDrawnLineIndex;
    void updateParser();
    bool mSpindleClockwise;
    SqlSettingsModel* mSettingsModelHandle;

public:
    void setupSignalSlots() override;
};
