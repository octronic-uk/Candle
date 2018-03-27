/*
 * SettingsModel.cpp
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
#include "SettingsModel.h"

SettingsModel::SettingsModel(QObject* parent)
    : QObject(parent)
{

    /*
    mSettingsFileName = qApp->applicationDirPath() + "/settings.ini";
    preloadSettings();
    */
}

SettingsModel::~SettingsModel()
{

}

void SettingsModel::preload()
{
    /*
    QSettings set(mSettingsFileName, QSettings::IniFormat);
    set.setIniCodec("UTF-8");

    qApp->setStyleSheet(QString(qApp->styleSheet()).replace(QRegExp("font-size:\\s*\\d+"), "font-size: " + set.value("fontSize", "8").toString()));

    // Update v-sync in glformat
    QGLFormat fmt = QGLFormat::defaultFormat();
    fmt.setSwapInterval(set.value("vsync", false).toBool() ? 1 : 0);
    QGLFormat::setDefaultFormat(fmt);
    */
}

void SettingsModel::load()
{
    /*
    QSettings set(mSettingsFileName, QSettings::IniFormat);
    set.setIniCodec("UTF-8");

    m_settingsLoading = true;

    mSettingsForm->setFontSize(set.value("fontSize", 8).toInt());
    mSettingsForm->setPort(set.value("port").toString());
    mSettingsForm->setBaud(set.value("baud").toInt());
    mSettingsForm->setIgnoreErrors(set.value("ignoreErrors", false).toBool());
    mSettingsForm->setAutoLine(set.value("autoLine", true).toBool());
    mSettingsForm->setToolDiameter(set.value("toolDiameter", 3).toDouble());
    mSettingsForm->setToolLength(set.value("toolLength", 15).toDouble());
    mSettingsForm->setAntialiasing(set.value("antialiasing", true).toBool());
    mSettingsForm->setMsaa(set.value("msaa", true).toBool());
    mSettingsForm->setVsync(set.value("vsync", false).toBool());
    mSettingsForm->setZBuffer(set.value("zBuffer", false).toBool());
    mSettingsForm->setSimplify(set.value("simplify", false).toBool());
    mSettingsForm->setSimplifyPrecision(set.value("simplifyPrecision", 0).toDouble());
    mSettingsForm->setGrayscaleSegments(set.value("grayscaleSegments", false).toBool());
    mSettingsForm->setGrayscaleSCode(set.value("grayscaleSCode", true).toBool());
    mSettingsForm->setDrawModeVectors(set.value("drawModeVectors", true).toBool());
    mUi.txtJogStep->setValue(set.value("jogStep", 1).toDouble());
    mUi.sliSpindleSpeed->setValue(set.value("spindleSpeed", 0).toInt());
    mSettingsForm->setMoveOnRestore(set.value("moveOnRestore", false).toBool());
    mSettingsForm->setRestoreMode(set.value("restoreMode", 0).toInt());
    mSettingsForm->setLineWidth(set.value("lineWidth", 1).toDouble());
    mSettingsForm->setArcLength(set.value("arcLength", 0).toDouble());
    mSettingsForm->setArcDegree(set.value("arcDegree", 0).toDouble());
    mSettingsForm->setArcDegreeMode(set.value("arcDegreeMode", true).toBool());
    mSettingsForm->setShowProgramCommands(set.value("showProgramCommands", 0).toBool());
    mSettingsForm->setShowUICommands(set.value("showUICommands", 0).toBool());
    mSettingsForm->setSpindleSpeedMin(set.value("spindleSpeedMin", 0).toInt());
    mSettingsForm->setSpindleSpeedMax(set.value("spindleSpeedMax", 100).toInt());
    mSettingsForm->setLaserPowerMin(set.value("laserPowerMin", 0).toInt());
    mSettingsForm->setLaserPowerMax(set.value("laserPowerMax", 100).toInt());
    mSettingsForm->setRapidSpeed(set.value("rapidSpeed", 0).toInt());
    mSettingsForm->setHeightmapProbingFeed(set.value("heightmapProbingFeed", 0).toInt());
    mSettingsForm->setAcceleration(set.value("acceleration", 10).toInt());
    mSettingsForm->setToolAngle(set.value("toolAngle", 0).toDouble());
    mSettingsForm->setToolType(set.value("toolType", 0).toInt());
    mSettingsForm->setFps(set.value("fps", 60).toInt());
    mSettingsForm->setQueryStateTime(set.value("queryStateTime", 250).toInt());

    mSettingsForm->setPanelUserCommands(set.value("panelUserCommandsVisible", true).toBool());
    mSettingsForm->setPanelHeightmap(set.value("panelHeightmapVisible", true).toBool());
    mSettingsForm->setPanelSpindle(set.value("panelSpindleVisible", true).toBool());
    mSettingsForm->setPanelFeed(set.value("panelFeedVisible", true).toBool());
    mSettingsForm->setPanelJog(set.value("panelJogVisible", true).toBool());

    mUi.grpConsole->setMinimumHeight(set.value("consoleMinHeight", 100).toInt());

    mUi.chkAutoScroll->setChecked(set.value("autoScroll", false).toBool());
    mUi.sliSpindleSpeed->setValue(set.value("spindleSpeed", 100).toInt() / 100);
    mUi.txtSpindleSpeed->setValue(set.value("spindleSpeed", 100).toInt());
    mUi.chkFeedOverride->setChecked(set.value("feedOverride", false).toBool());
    mUi.sliFeed->setValue(set.value("feed", 100).toInt());
    mSettingsForm->setUnits(set.value("units", 0).toInt());
    m_storedX = set.value("storedX", 0).toDouble();
    m_storedY = set.value("storedY", 0).toDouble();
    m_storedZ = set.value("storedZ", 0).toDouble();

    mUi.cmdRestoreOrigin->setToolTip(QString(tr("Restore origin:\n%1, %2, %3")).arg(m_storedX).arg(m_storedY).arg(m_storedZ));

    m_recentFiles = set.value("recentFiles", QStringList()).toStringList();
    m_recentHeightmaps = set.value("recentHeightmaps", QStringList()).toStringList();
    mLastFolder = set.value("lastFolder", QDir::homePath()).toString();

    this->restoreGeometry(set.value("formGeometry", QByteArray()).toByteArray());
    mSettingsForm->resize(set.value("formSettingsSize", mSettingsForm->size()).toSize());
    QByteArray splitterState = set.value("splitter", QByteArray()).toByteArray();

    if (splitterState.length() == 0) {
        mUi.splitter->setStretchFactor(0, 1);
        mUi.splitter->setStretchFactor(1, 1);
    } else mUi.splitter->restoreState(splitterState);

    mUi.chkAutoScroll->setVisible(mUi.splitter->sizes()[1]);
    resizeCheckBoxes();

    mUi.cboCommand->setMinimumHeight(mUi.cboCommand->height());
    mUi.cmdClearConsole->setFixedHeight(mUi.cboCommand->height());
    mUi.cmdCommandSend->setFixedHeight(mUi.cboCommand->height());

    m_storedKeyboardControl = set.value("keyboardControl", false).toBool();

    mSettingsForm->setAutoCompletion(set.value("autoCompletion", true).toBool());
    mSettingsForm->setTouchCommand(set.value("touchCommand").toString());
    mSettingsForm->setSafePositionCommand(set.value("safePositionCommand").toString());

    foreach (StyledToolButton* button, this->findChildren<StyledToolButton*>(QRegExp("cmdUser\\d"))) {
        int i = button->objectName().right(1).toInt();
        mSettingsForm->setUserCommands(i, set.value(QString("userCommands%1").arg(i)).toString());
    }

    mUi.txtHeightMapBorderX->setValue(set.value("heightmapBorderX", 0).toDouble());
    mUi.txtHeightMapBorderY->setValue(set.value("heightmapBorderY", 0).toDouble());
    mUi.txtHeightMapBorderWidth->setValue(set.value("heightmapBorderWidth", 1).toDouble());
    mUi.txtHeightMapBorderHeight->setValue(set.value("heightmapBorderHeight", 1).toDouble());
    mUi.chkHeightMapBorderShow->setChecked(set.value("heightmapBorderShow", false).toBool());

    mUi.txtHeightMapGridX->setValue(set.value("heightmapGridX", 1).toDouble());
    mUi.txtHeightMapGridY->setValue(set.value("heightmapGridY", 1).toDouble());
    mUi.txtHeightMapGridZTop->setValue(set.value("heightmapGridZTop", 1).toDouble());
    mUi.txtHeightMapGridZBottom->setValue(set.value("heightmapGridZBottom", -1).toDouble());
    mUi.chkHeightMapGridShow->setChecked(set.value("heightmapGridShow", false).toBool());

    mUi.txtHeightMapInterpolationStepX->setValue(set.value("heightmapInterpolationStepX", 1).toDouble());
    mUi.txtHeightMapInterpolationStepY->setValue(set.value("heightmapInterpolationStepY", 1).toDouble());
    mUi.cboHeightMapInterpolationType->setCurrentIndex(set.value("heightmapInterpolationType", 0).toInt());
    mUi.chkHeightMapInterpolationShow->setChecked(set.value("heightmapInterpolationShow", false).toBool());

    foreach (ColorPicker* pick, mSettingsForm->colors()) {
        pick->setColor(QColor(set.value(pick->objectName().mid(3), "black").toString()));
    }

    updateRecentFilesMenu();

    mUi.tblProgram->horizontalHeader()->restoreState(set.value("header", QByteArray()).toByteArray());

    // Update right panel width
    applySettings();
    show();
    mUi.scrollArea->updateMinimumWidth();

    // Restore panels state
    mUi.grpUserCommands->setChecked(set.value("userCommandsPanel", true).toBool());
    mUi.grpHeightMap->setChecked(set.value("heightmapPanel", true).toBool());
    mUi.grpSpindle->setChecked(set.value("spindlePanel", true).toBool());
    mUi.grpFeed->setChecked(set.value("feedPanel", true).toBool());
    mUi.grpJog->setChecked(set.value("jogPanel", true).toBool());

    // Restore last commands list
    mUi.cboCommand->addItems(set.value("recentCommands", QStringList()).toStringList());
    mUi.cboCommand->setCurrentIndex(-1);

    m_settingsLoading = false;
    */
}

void SettingsModel::save()
{
    /*
    QSettings set(mSettingsFileName, QSettings::IniFormat);
    set.setIniCodec("UTF-8");

    set.setValue("port", mSettingsForm->port());
    set.setValue("baud", mSettingsForm->baud());
    set.setValue("ignoreErrors", mSettingsForm->ignoreErrors());
    set.setValue("autoLine", mSettingsForm->autoLine());
    set.setValue("toolDiameter", mSettingsForm->toolDiameter());
    set.setValue("toolLength", mSettingsForm->toolLength());
    set.setValue("antialiasing", mSettingsForm->antialiasing());
    set.setValue("msaa", mSettingsForm->msaa());
    set.setValue("vsync", mSettingsForm->vsync());
    set.setValue("zBuffer", mSettingsForm->zBuffer());
    set.setValue("simplify", mSettingsForm->simplify());
    set.setValue("simplifyPrecision", mSettingsForm->simplifyPrecision());
    set.setValue("grayscaleSegments", mSettingsForm->grayscaleSegments());
    set.setValue("grayscaleSCode", mSettingsForm->grayscaleSCode());
    set.setValue("drawModeVectors", mSettingsForm->drawModeVectors());
    set.setValue("jogStep", mUi.txtJogStep->value());
    set.setValue("spindleSpeed", mUi.txtSpindleSpeed->text());
    set.setValue("lineWidth", mSettingsForm->lineWidth());
    set.setValue("arcLength", mSettingsForm->arcLength());
    set.setValue("arcDegree", mSettingsForm->arcDegree());
    set.setValue("arcDegreeMode", mSettingsForm->arcDegreeMode());
    set.setValue("showProgramCommands", mSettingsForm->showProgramCommands());
    set.setValue("showUICommands", mSettingsForm->showUICommands());
    set.setValue("spindleSpeedMin", mSettingsForm->spindleSpeedMin());
    set.setValue("spindleSpeedMax", mSettingsForm->spindleSpeedMax());
    set.setValue("laserPowerMin", mSettingsForm->laserPowerMin());
    set.setValue("laserPowerMax", mSettingsForm->laserPowerMax());
    set.setValue("moveOnRestore", mSettingsForm->moveOnRestore());
    set.setValue("restoreMode", mSettingsForm->restoreMode());
    set.setValue("rapidSpeed", mSettingsForm->rapidSpeed());
    set.setValue("heightmapProbingFeed", mSettingsForm->heightmapProbingFeed());
    set.setValue("acceleration", mSettingsForm->acceleration());
    set.setValue("toolAngle", mSettingsForm->toolAngle());
    set.setValue("toolType", mSettingsForm->toolType());
    set.setValue("fps", mSettingsForm->fps());
    set.setValue("queryStateTime", mSettingsForm->queryStateTime());
    set.setValue("autoScroll", mUi.chkAutoScroll->isChecked());
    set.setValue("header", mUi.tblProgram->horizontalHeader()->saveState());
    set.setValue("splitter", mUi.splitter->saveState());
    set.setValue("formGeometry", this->saveGeometry());
    set.setValue("formSettingsSize", mSettingsForm->size());
    set.setValue("spindleSpeed", mUi.txtSpindleSpeed->value());
    set.setValue("feedOverride", mUi.chkFeedOverride->isChecked());
    set.setValue("feed", mUi.txtFeed->value());
    set.setValue("userCommandsPanel", mUi.grpUserCommands->isChecked());
    set.setValue("heightmapPanel", mUi.grpHeightMap->isChecked());
    set.setValue("spindlePanel", mUi.grpSpindle->isChecked());
    set.setValue("feedPanel", mUi.grpFeed->isChecked());
    set.setValue("jogPanel", mUi.grpJog->isChecked());
    set.setValue("keyboardControl", mUi.chkKeyboardControl->isChecked());
    set.setValue("autoCompletion", mSettingsForm->autoCompletion());
    set.setValue("units", mSettingsForm->units());
    set.setValue("storedX", m_storedX);
    set.setValue("storedY", m_storedY);
    set.setValue("storedZ", m_storedZ);
    set.setValue("recentFiles", m_recentFiles);
    set.setValue("recentHeightmaps", m_recentHeightmaps);
    set.setValue("lastFolder", mLastFolder);
    set.setValue("touchCommand", mSettingsForm->touchCommand());
    set.setValue("safePositionCommand", mSettingsForm->safePositionCommand());
    set.setValue("panelUserCommandsVisible", mSettingsForm->panelUserCommands());
    set.setValue("panelHeightmapVisible", mSettingsForm->panelHeightmap());
    set.setValue("panelSpindleVisible", mSettingsForm->panelSpindle());
    set.setValue("panelFeedVisible", mSettingsForm->panelFeed());
    set.setValue("panelJogVisible", mSettingsForm->panelJog());
    set.setValue("fontSize", mSettingsForm->fontSize());
    set.setValue("consoleMinHeight", mUi.grpConsole->minimumHeight());

    foreach (StyledToolButton* button, this->findChildren<StyledToolButton*>(QRegExp("cmdUser\\d"))) {
        int i = button->objectName().right(1).toInt();
        set.setValue(QString("userCommands%1").arg(i), mSettingsForm->userCommands(i));
    }

    set.setValue("heightmapBorderX", mUi.txtHeightMapBorderX->value());
    set.setValue("heightmapBorderY", mUi.txtHeightMapBorderY->value());
    set.setValue("heightmapBorderWidth", mUi.txtHeightMapBorderWidth->value());
    set.setValue("heightmapBorderHeight", mUi.txtHeightMapBorderHeight->value());
    set.setValue("heightmapBorderShow", mUi.chkHeightMapBorderShow->isChecked());

    set.setValue("heightmapGridX", mUi.txtHeightMapGridX->value());
    set.setValue("heightmapGridY", mUi.txtHeightMapGridY->value());
    set.setValue("heightmapGridZTop", mUi.txtHeightMapGridZTop->value());
    set.setValue("heightmapGridZBottom", mUi.txtHeightMapGridZBottom->value());
    set.setValue("heightmapGridShow", mUi.chkHeightMapGridShow->isChecked());

    set.setValue("heightmapInterpolationStepX", mUi.txtHeightMapInterpolationStepX->value());
    set.setValue("heightmapInterpolationStepY", mUi.txtHeightMapInterpolationStepY->value());
    set.setValue("heightmapInterpolationType", mUi.cboHeightMapInterpolationType->currentIndex());
    set.setValue("heightmapInterpolationShow", mUi.chkHeightMapInterpolationShow->isChecked());

    foreach (ColorPicker* pick, mSettingsForm->colors()) {
        set.setValue(pick->objectName().mid(3), pick->color().name());
    }

    QStringList list;

    for (int i = 0; i < mUi.cboCommand->count(); i++) list.append(mUi.cboCommand->itemText(i));
    set.setValue("recentCommands", list);
    */
}

