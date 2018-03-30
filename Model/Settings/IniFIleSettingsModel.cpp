/*
 * IniFileSettingsModel.cpp
 *
 * Created: 24 2018 by Ashley
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

#include "IniFileSettingsModel.h"
#include <QStandardPaths>
#include <QGLFormat>

IniFileSettingsModel::IniFileSettingsModel(QObject* parent)
    : AbstractSettingsModel (parent)
{
    mSettingsDirectory = QDir(
        QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0)
    );
}

IniFileSettingsModel::~IniFileSettingsModel()
{

}

void IniFileSettingsModel::preload()
{
    mSettings.setIniCodec("UTF-8");
    // Update v-sync in glformat
    QGLFormat fmt = QGLFormat::defaultFormat();
    fmt.setSwapInterval(mSettings.value("vsync", false).toBool() ? 1 : 0);
    QGLFormat::setDefaultFormat(fmt);
}

void IniFileSettingsModel::onLoadSettings()
{
    mSettingsLoading = true;

    // Serial
    emit settingChangedSignal(SERIAL_PORT_NAME, mSettings.value(SERIAL_PORT_NAME).toString());
    emit settingChangedSignal(SERIAL_BAUD_RATE, mSettings.value(SERIAL_BAUD_RATE).toInt());

    emit settingChangedSignal(IGNORE_ERRORS, mSettings.value(IGNORE_ERRORS, false).toBool());
    emit settingChangedSignal(AUTO_LINE, mSettings.value(AUTO_LINE, true).toBool());

    // Tool
    emit settingChangedSignal(TOOL_DIAMETER, mSettings.value(TOOL_DIAMETER, 3).toDouble());
    emit settingChangedSignal(TOOL_LENGTH, mSettings.value(TOOL_LENGTH, 15).toDouble());
    emit settingChangedSignal(TOOL_ANGLE, mSettings.value(TOOL_ANGLE, 0).toDouble());
    emit settingChangedSignal(TOOL_TYPE, mSettings.value(TOOL_TYPE, 0).toInt());

    // Gfx
    emit settingChangedSignal(GFX_ANTIALIASING, mSettings.value(GFX_ANTIALIASING, true).toBool());
    emit settingChangedSignal(GFX_MSAA, mSettings.value(GFX_MSAA, true).toBool());
    emit settingChangedSignal(GFX_VSYNC, mSettings.value(GFX_VSYNC, false).toBool());
    emit settingChangedSignal(GFX_ZBUFFER, mSettings.value(GFX_ZBUFFER, false).toBool());
    emit settingChangedSignal(GFX_SIMPLIFY, mSettings.value(GFX_SIMPLIFY, false).toBool());
    emit settingChangedSignal(GFX_SIMPLIFY_PRECISION, mSettings.value(GFX_SIMPLIFY_PRECISION, 0).toDouble());
    emit settingChangedSignal(GFX_GRAYSCALE_SEGMENTS, mSettings.value(GFX_GRAYSCALE_SEGMENTS, false).toBool());
    emit settingChangedSignal(GFX_GRAYSCALE_S_CODE, mSettings.value(GFX_GRAYSCALE_S_CODE, true).toBool());
    emit settingChangedSignal(GFX_DRAW_MODE_VECTORS, mSettings.value(GFX_DRAW_MODE_VECTORS, true).toBool());

    // Ui
    emit settingChangedSignal(UI_JOG_STEP, mSettings.value(UI_JOG_STEP, 1).toDouble());
    emit settingChangedSignal(UI_SPINDLE_SPEED, mSettings.value(UI_SPINDLE_SPEED, 0).toInt());
    emit settingChangedSignal(UI_MOVE_ON_RESTORE, mSettings.value(UI_MOVE_ON_RESTORE, false).toBool());
    emit settingChangedSignal(UI_RESTORE_MODE, mSettings.value(UI_RESTORE_MODE, 0).toInt());
    emit settingChangedSignal(UI_SHOW_PROGRAM_COMMANDS, mSettings.value(UI_SHOW_PROGRAM_COMMANDS, false).toBool());
    /*
    emit settingChangedSignal(setShowUICommands(mSettings.value("showUICommands", 0).toBool());
    emit settingChangedSignal(setSpindleSpeedMin(mSettings.value("spindleSpeedMin", 0).toInt());
    emit settingChangedSignal(setSpindleSpeedMax(mSettings.value("spindleSpeedMax", 100).toInt());
    emit settingChangedSignal(setLaserPowerMin(mSettings.value("laserPowerMin", 0).toInt());
    emit settingChangedSignal(setLaserPowerMax(mSettings.value("laserPowerMax", 100).toInt());
    emit settingChangedSignal(setPanelUserCommands(mSettings.value("panelUserCommandsVisible", true).toBool());
    emit settingChangedSignal(setPanelHeightmap(mSettings.value("panelHeightmapVisible", true).toBool());
    emit settingChangedSignal(setPanelSpindle(mSettings.value("panelSpindleVisible", true).toBool());
    emit settingChangedSignal(setPanelFeed(mSettings.value("panelFeedVisible", true).toBool());
    emit settingChangedSignal(setPanelJog(mSettings.value("panelJogVisible", true).toBool());
    mUi.grpConsole->setMinimumHeight(mSettings.value("consoleMinHeight", 100).toInt());
    mUi.chkAutoScroll->setChecked(mSettings.value("autoScroll", false).toBool());
    mUi.txtSpindleSpeed->setValue(mSettings.value("spindleSpeed", 100).toInt());
    mUi.chkFeedOverride->setChecked(mSettings.value("feedOverride", false).toBool());
    mUi.sliFeed->setValue(mSettings.value("feed", 100).toInt());
    emit settingChangedSignal(setUnits(mSettings.value("units", 0).toInt());
    mLastFolder = mSettings.value("lastFolder", QDir::homePath()).toString();
    m_storedKeyboardControl = mSettings.value("keyboardControl", false).toBool();
    emit settingChangedSignal(setAutoCompletion(mSettings.value("autoCompletion", true).toBool());
    emit settingChangedSignal(setTouchCommand(mSettings.value("touchCommand").toString());
    emit settingChangedSignal(setSafePositionCommand(mSettings.value("safePositionCommand").toString());
    mUi.tblProgram->horizontalHeader()->restoreState(mSettings.value("header", QByteArray()).toByteArray());
    mUi.cboCommand->addItems(mSettings.value("recentCommands", QStringList()).toStringList());
    mUi.cboCommand->setCurrentIndex(-1);

    // Visualiser
    emit settingChangedSignal(setLineWidth(mSettings.value("lineWidth", 1).toDouble());
    emit settingChangedSignal(setArcLength(mSettings.value("arcLength", 0).toDouble());
    emit settingChangedSignal(setArcDegree(mSettings.value("arcDegree", 0).toDouble());
    emit settingChangedSignal(setArcDegreeMode(mSettings.value("arcDegreeMode", true).toBool());
    emit settingChangedSignal(setRapidSpeed(mSettings.value("rapidSpeed", 0).toInt());
    emit settingChangedSignal(setHeightmapProbingFeed(mSettings.value("heightmapProbingFeed", 0).toInt());
    emit settingChangedSignal(setAcceleration(mSettings.value("acceleration", 10).toInt());
    emit settingChangedSignal(setFps(mSettings.value("fps", 60).toInt());
    emit settingChangedSignal(setQueryStateTime(mSettings.value("queryStateTime", 250).toInt());

    // HeightMap
    mUi.txtHeightMapBorderX->setValue(mSettings.value("heightmapBorderX", 0).toDouble());
    mUi.txtHeightMapBorderY->setValue(mSettings.value("heightmapBorderY", 0).toDouble());
    mUi.txtHeightMapBorderWidth->setValue(mSettings.value("heightmapBorderWidth", 1).toDouble());
    mUi.txtHeightMapBorderHeight->setValue(mSettings.value("heightmapBorderHeight", 1).toDouble());
    mUi.chkHeightMapBorderShow->setChecked(mSettings.value("heightmapBorderShow", false).toBool());
    mUi.txtHeightMapGridX->setValue(mSettings.value("heightmapGridX", 1).toDouble());
    mUi.txtHeightMapGridY->setValue(mSettings.value("heightmapGridY", 1).toDouble());
    mUi.txtHeightMapGridZTop->setValue(mSettings.value("heightmapGridZTop", 1).toDouble());
    mUi.txtHeightMapGridZBottom->setValue(mSettings.value("heightmapGridZBottom", -1).toDouble());
    mUi.chkHeightMapGridShow->setChecked(mSettings.value("heightmapGridShow", false).toBool());
    mUi.txtHeightMapInterpolationStepX->setValue(mSettings.value("heightmapInterpolationStepX", 1).toDouble());
    mUi.txtHeightMapInterpolationStepY->setValue(mSettings.value("heightmapInterpolationStepY", 1).toDouble());
    mUi.cboHeightMapInterpolationType->setCurrentIndex(mSettings.value("heightmapInterpolationType", 0).toInt());
    mUi.chkHeightMapInterpolationShow->setChecked(mSettings.value("heightmapInterpolationShow", false).toBool());

    m_settingsLoading = false;
    */
}

void IniFileSettingsModel::onSaveSettings()
{
    mSettings.sync();
    /*
    QSettings set(mSettingsFileName, QSettings::IniFormat);
    mSettings.setIniCodec("UTF-8");

    mSettings.setValue("port", emit settingChangedSignal(port());
    mSettings.setValue("baud", emit settingChangedSignal(baud());
    mSettings.setValue("ignoreErrors", emit settingChangedSignal(ignoreErrors());
    mSettings.setValue("autoLine", emit settingChangedSignal(autoLine());
    mSettings.setValue("toolDiameter", emit settingChangedSignal(toolDiameter());
    mSettings.setValue("toolLength", emit settingChangedSignal(toolLength());
    mSettings.setValue("antialiasing", emit settingChangedSignal(antialiasing());
    mSettings.setValue("msaa", emit settingChangedSignal(msaa());
    mSettings.setValue("vsync", emit settingChangedSignal(vsync());
    mSettings.setValue("zBuffer", emit settingChangedSignal(zBuffer());
    mSettings.setValue("simplify", emit settingChangedSignal(simplify());
    mSettings.setValue("simplifyPrecision", emit settingChangedSignal(simplifyPrecision());
    mSettings.setValue("grayscaleSegments", emit settingChangedSignal(grayscaleSegments());
    mSettings.setValue("grayscaleSCode", emit settingChangedSignal(grayscaleSCode());
    mSettings.setValue("drawModeVectors", emit settingChangedSignal(drawModeVectors());
    mSettings.setValue("jogStep", mUi.txtJogStep->value());
    mSettings.setValue("spindleSpeed", mUi.txtSpindleSpeed->text());
    mSettings.setValue("lineWidth", emit settingChangedSignal(lineWidth());
    mSettings.setValue("arcLength", emit settingChangedSignal(arcLength());
    mSettings.setValue("arcDegree", emit settingChangedSignal(arcDegree());
    mSettings.setValue("arcDegreeMode", emit settingChangedSignal(arcDegreeMode());
    mSettings.setValue("showProgramCommands", emit settingChangedSignal(showProgramCommands());
    mSettings.setValue("showUICommands", emit settingChangedSignal(showUICommands());
    mSettings.setValue("spindleSpeedMin", emit settingChangedSignal(spindleSpeedMin());
    mSettings.setValue("spindleSpeedMax", emit settingChangedSignal(spindleSpeedMax());
    mSettings.setValue("laserPowerMin", emit settingChangedSignal(laserPowerMin());
    mSettings.setValue("laserPowerMax", emit settingChangedSignal(laserPowerMax());
    mSettings.setValue("moveOnRestore", emit settingChangedSignal(moveOnRestore());
    mSettings.setValue("restoreMode", emit settingChangedSignal(restoreMode());
    mSettings.setValue("rapidSpeed", emit settingChangedSignal(rapidSpeed());
    mSettings.setValue("heightmapProbingFeed", emit settingChangedSignal(heightmapProbingFeed());
    mSettings.setValue("acceleration", emit settingChangedSignal(acceleration());
    mSettings.setValue("toolAngle", emit settingChangedSignal(toolAngle());
    mSettings.setValue("toolType", emit settingChangedSignal(toolType());
    mSettings.setValue("fps", emit settingChangedSignal(fps());
    mSettings.setValue("queryStateTime", emit settingChangedSignal(queryStateTime());
    mSettings.setValue("autoScroll", mUi.chkAutoScroll->isChecked());
    mSettings.setValue("header", mUi.tblProgram->horizontalHeader()->saveState());
    mSettings.setValue("splitter", mUi.splitter->saveState());
    mSettings.setValue("formGeometry", this->saveGeometry());
    mSettings.setValue("formSettingsSize", emit settingChangedSignal(size());
    mSettings.setValue("spindleSpeed", mUi.txtSpindleSpeed->value());
    mSettings.setValue("feedOverride", mUi.chkFeedOverride->isChecked());
    mSettings.setValue("feed", mUi.txtFeed->value());
    mSettings.setValue("userCommandsPanel", mUi.grpUserCommands->isChecked());
    mSettings.setValue("heightmapPanel", mUi.grpHeightMap->isChecked());
    mSettings.setValue("spindlePanel", mUi.grpSpindle->isChecked());
    mSettings.setValue("feedPanel", mUi.grpFeed->isChecked());
    mSettings.setValue("jogPanel", mUi.grpJog->isChecked());
    mSettings.setValue("keyboardControl", mUi.chkKeyboardControl->isChecked());
    mSettings.setValue("autoCompletion", emit settingChangedSignal(autoCompletion());
    mSettings.setValue("units", emit settingChangedSignal(units());
    mSettings.setValue("storedX", m_storedX);
    mSettings.setValue("storedY", m_storedY);
    mSettings.setValue("storedZ", m_storedZ);
    mSettings.setValue("recentFiles", m_recentFiles);
    mSettings.setValue("recentHeightmaps", m_recentHeightmaps);
    mSettings.setValue("lastFolder", mLastFolder);
    mSettings.setValue("touchCommand", emit settingChangedSignal(touchCommand());
    mSettings.setValue("safePositionCommand", emit settingChangedSignal(safePositionCommand());
    mSettings.setValue("panelUserCommandsVisible", emit settingChangedSignal(panelUserCommands());
    mSettings.setValue("panelHeightmapVisible", emit settingChangedSignal(panelHeightmap());
    mSettings.setValue("panelSpindleVisible", emit settingChangedSignal(panelSpindle());
    mSettings.setValue("panelFeedVisible", emit settingChangedSignal(panelFeed());
    mSettings.setValue("panelJogVisible", emit settingChangedSignal(panelJog());
    mSettings.setValue("fontSize", emit settingChangedSignal(fontSize());
    mSettings.setValue("consoleMinHeight", mUi.grpConsole->minimumHeight());

    foreach (StyledToolButton* button, this->findChildren<StyledToolButton*>(QRegExp("cmdUser\\d"))) {
        int i = button->objectName().right(1).toInt();
        mSettings.setValue(QString("userCommands%1").arg(i), emit settingChangedSignal(userCommands(i));
    }

    mSettings.setValue("heightmapBorderX", mUi.txtHeightMapBorderX->value());
    mSettings.setValue("heightmapBorderY", mUi.txtHeightMapBorderY->value());
    mSettings.setValue("heightmapBorderWidth", mUi.txtHeightMapBorderWidth->value());
    mSettings.setValue("heightmapBorderHeight", mUi.txtHeightMapBorderHeight->value());
    mSettings.setValue("heightmapBorderShow", mUi.chkHeightMapBorderShow->isChecked());

    mSettings.setValue("heightmapGridX", mUi.txtHeightMapGridX->value());
    mSettings.setValue("heightmapGridY", mUi.txtHeightMapGridY->value());
    mSettings.setValue("heightmapGridZTop", mUi.txtHeightMapGridZTop->value());
    mSettings.setValue("heightmapGridZBottom", mUi.txtHeightMapGridZBottom->value());
    mSettings.setValue("heightmapGridShow", mUi.chkHeightMapGridShow->isChecked());

    mSettings.setValue("heightmapInterpolationStepX", mUi.txtHeightMapInterpolationStepX->value());
    mSettings.setValue("heightmapInterpolationStepY", mUi.txtHeightMapInterpolationStepY->value());
    mSettings.setValue("heightmapInterpolationType", mUi.cboHeightMapInterpolationType->currentIndex());
    mSettings.setValue("heightmapInterpolationShow", mUi.chkHeightMapInterpolationShow->isChecked());

    foreach (ColorPicker* pick, emit settingChangedSignal(colors()) {
        mSettings.setValue(pick->objectName().mid(3), pick->color().name());
    }

    QStringList list;

    for (int i = 0; i < mUi.cboCommand->count(); i++) list.append(mUi.cboCommand->itemText(i));
    mSettings.setValue("recentCommands", list);
    */
}

