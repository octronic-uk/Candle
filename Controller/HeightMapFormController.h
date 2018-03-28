/*
 * HeightMapFormController.h
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
#include "ui_HeightMapForm.h"

#include "View/Drawers/heightmapborderdrawer.h"
#include "View/Drawers/HeightMapGridDrawer.h"
#include "View/Drawers/HeightMapInterpolationDrawer.h"
#include "Model/Tables/HeightMapTableModel.h"
#include "Model/Tables/GCodeTableModel.h"

class LineSegment;

using namespace Ui;

class HeightMapFormController : public AbstractFormController
{
    Q_OBJECT
public:
    explicit HeightMapFormController(QWidget *parent = nullptr);
    ~HeightMapFormController();
    void updateHeightMapInterpolationDrawer(bool reset = false);

    void updateHeightMapGrid(double arg1);
    void updateHeightMapBorderDrawer();
    bool updateHeightMapGrid();

    void resizeTableHeightMapSections();
    void setHeightMapMode(bool);

signals:
public slots:
    void onCheckBoxHeightMapBorderShowToggled(bool checked);
    void onTextHeightMapBorderXValueChanged(double arg1);
    void onTextHeightMapBorderWidthValueChanged(double arg1);
    void onTextHeightMapBorderYValueChanged(double arg1);
    void onTextHeightMapBorderHeightValueChanged(double arg1);
    void onTextHeightMapGridXValueChanged(double arg1);
    void onTextHeightMapGridYValueChanged(double arg1);
    void onTextHeightMapGridZBottomValueChanged(double arg1);
    void onTextHeightMapGridZTopValueChanged(double arg1);
    void onCheckBoxHeightMapGridShowToggled(bool checked);
    void onCmdHeightMapModeToggled(bool checked);
    void onCheckBoxHeightMapInterpolationShowToggled(bool checked);
    void onCmdHeightMapLoadClicked();
    void onTextHeightMapInterpolationStepXValueChanged(double arg1);
    void onTextHeightMapInterpolationStepYValueChanged(double arg1);
    void onCheckBoxHeightMapUseClicked(bool checked);
    void onCmdHeightMapCreateClicked();
    void onCmdHeightMapBorderAutoClicked();
    void resizeEvent(QResizeEvent* re) override;
    void onGripHeightMapToggled(bool arg1);
private:
    HeightMapForm mUi;
    HeightMapTableModel mHeightMapModel;
    GcodeTableModel mProgramHeightmapTableModel;
    GcodeTableModel mProbeTableModel;
    bool mHeightMapMode;

    QList<LineSegment*> subdivideSegment(LineSegment* segment);
    QRectF borderRectFromTextboxes();
    QRectF borderRectFromExtremes();
    void resetHeightmap();


    // AbstractFormController interface
public:
    void setupSignalSlots() override;
};
