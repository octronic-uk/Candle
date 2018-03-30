/*
 * AbstractSettingsModel.h
 *
 * Created: 30 2018 by Ashley
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

#include <QSettings>

class AbstractSettingsModel : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSettingsModel(QObject *parent = nullptr);
    virtual ~AbstractSettingsModel() = 0;

signals:
    void settingChangedSignal(QString settings, QVariant value);
public slots:
   virtual void onSaveSettings() = 0;
   virtual void onLoadSettings() = 0;

protected:
    QSettings mSettings;
    bool mSettingsLoading;
    bool mSettingsChanged;

    const QString SERIAL_PORT_NAME = "Serial/PortName";
    const QString SERIAL_BAUD_RATE = "Serial/BaudRate";

    const QString IGNORE_ERRORS = "IgnoreErrors";
    const QString AUTO_LINE = "AutoLine";

    const QString TOOL_DIAMETER = "Tool/Diameter";
    const QString TOOL_LENGTH = "Tool/Length";
    const QString TOOL_ANGLE = "Tool/Angle";
    const QString TOOL_TYPE = "Tool/Type";

    const QString GFX_ANTIALIASING = "Graphics/AntiAliasing";
    const QString GFX_MSAA = "Graphics/MSAA";
    const QString GFX_VSYNC = "Graphics/VSync";
    const QString GFX_ZBUFFER = "Graphics/ZBuffer";
    const QString GFX_SIMPLIFY = "Graphics/Simplify";
    const QString GFX_SIMPLIFY_PRECISION = "Graphics/SimplifyPrecision";
    const QString GFX_GRAYSCALE_SEGMENTS = "Graphics/GrayscaleSegments";
    const QString GFX_GRAYSCALE_S_CODE = "Graphics/GrayscaleSCode";
    const QString GFX_DRAW_MODE_VECTORS = "Graphics/DrawModeVectors";

    const QString UI_JOG_STEP = "Ui/JogStep";
    const QString UI_SPINDLE_SPEED = "Ui/SpindleSpeed";
    const QString UI_MOVE_ON_RESTORE = "Ui/MoveOnRestore";
    const QString UI_RESTORE_MODE = "Ui/RestoreMode";
    const QString UI_SHOW_PROGRAM_COMMANDS = "Ui/ShowProgramCommands";
};
