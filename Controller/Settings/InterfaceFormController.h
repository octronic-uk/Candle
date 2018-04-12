/*
 * InterfaceForm.h
 *
 * Created: 08 2018 by Ashley
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

#include "Controller/AbstractFormController.h"
#include "ui_InterfaceForm.h"

class SqlSettingsModel;
class Profile;

class InterfaceFormController : public AbstractFormController
{
    Q_OBJECT

public:
    explicit InterfaceFormController(QWidget *parent = nullptr);
    ~InterfaceFormController() override;

    void setupSignalSlots() override;
    void setFormActive(bool) override;
    void initialise() override;

    bool antialiasing();
    void setAntialiasing(bool antialiasing);

    bool zBuffer();
    void setZBuffer(bool zBuffer);

    double lineWidth();
    void setLineWidth(double lineWidth);

    bool showProgramCommands();
    void setShowProgramCommands(bool showProgramCommands);

    bool showUICommands();
    void setShowUICommands(bool showUICommands);

    int fps();
    void setFps(int fps);

    bool vsync();
    void setVsync(bool value);

    bool msaa();
    void setMsaa(bool msaa);

    bool autoCompletion();
    void setAutoCompletion(bool autoCompletion);

    bool simplify();
    void setSimplify(bool simplify);

    double simplifyPrecision();
    void setSimplifyPrecision(double simplifyPrecision);

    bool grayscaleSegments();
    void setGrayscaleSegments(bool value);

    bool grayscaleSCode();
    void setGrayscaleSCode(bool value);

    bool drawModeVectors();
    void setDrawModeVectors(bool value);

    void setSettingsModel(SqlSettingsModel* handle);
    void onProfileChanged(Profile* profile);


private slots:
    void onLineWidthValueChanged(QString);
    void onFpsCurrentIndexChanged(QString);
    void onAntiAliasingToggled(bool);
    void onMsaaToggled(bool);
    void onVsyncToggled(bool);
    void onZBufferToggled(bool);
    void onDrawModeVectorsToggled(bool checked);
    void onDrawModeRasterToggled(bool checked);
    void onSimplifyToggled(bool);
    void onSimplifyPrecisionValueChanged(QString);
    void onGrayscaleToggled(bool);
    void onGrayscaleSToggled(bool checked);
    void onGrayscaleZToggled(bool checked);
    void onShowProgramCommandsToggled(bool);
    void onShowUiCommandsToggled(bool);
    void onAutoCompletionToggled(bool);

private:
    Ui::InterfaceForm mUi;
    SqlSettingsModel* mSettingsModelHandle;
};
