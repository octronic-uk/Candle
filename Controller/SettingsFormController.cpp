// This file is a part of "Cocoanut" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "SettingsFormController.h"
#include "ui_SettingsForm.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QScrollBar>
#include <QColorDialog>

SettingsFormController::SettingsFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "SettingsFormController: Constructing";
    mUi.setupUi(&mDialog);
    setupSignalSlots();
    /*
    mUi.setupUi(&mDialog);
    mDialog.setLocale(QLocale::C);
    mIntValidator.setBottom(1);
    mIntValidator.setTop(999);
    mUi.cboFps->setValidator(&mIntValidator);
    mUi.cboFontSize->setValidator(&mIntValidator);

    foreach (QGroupBox *box, this->findChildren<QGroupBox*>())
    {
        mUi.listCategories->addItem(box->title());
        mUi.listCategories->item(mUi.listCategories->count() - 1)->setData(Qt::UserRole, box->objectName());
    }

    mUi.listCategories->item(0)->setSelected(true);
    connect(mUi.scrollSettings->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onScrollBarValueChanged(int)));

    searchPorts();

    if (mSerialPortName != "")
    {
        //mSerialPort.setPortName(mSettingsForm->port());
        //mSerialPort.setBaudRate(mSettingsForm->baud());
    }
    */
}

SettingsFormController::~SettingsFormController()
{
    qDebug() << "SettingsFormController: Destructing";
}

int SettingsFormController::exec()
{
    // Store settings to undo
    mStoredValues.clear();
    mStoredChecks.clear();
    mStoredCombos.clear();
    mStoredColors.clear();

    foreach (QAbstractSpinBox* sb, this->findChildren<QAbstractSpinBox*>())
    {
        mStoredValues.append(sb->property("value").toDouble());
    }

    foreach (QAbstractButton* cb, this->findChildren<QAbstractButton*>())
    {
        mStoredChecks.append(cb->isChecked());
    }

    foreach (QComboBox* cb, this->findChildren<QComboBox*>())
    {
        mStoredCombos.append(cb->currentText());
    }

    /*foreach (ColorPicker* pick, this->findChildren<ColorPicker*>())
    {
        //mStoredColors.append(pick->color());
    }
    */

    return mDialog.exec();
}

void SettingsFormController::undo()
{
    foreach (QAbstractSpinBox* sb, this->findChildren<QAbstractSpinBox*>())
    {
        sb->setProperty("value", mStoredValues.takeFirst());
    }

    foreach (QAbstractButton* cb, this->findChildren<QAbstractButton*>())
    {
        cb->setChecked(mStoredChecks.takeFirst());
    }
    foreach (QComboBox* cb, this->findChildren<QComboBox*>())
    {
        cb->setCurrentText(mStoredCombos.takeFirst());
    }
    /*foreach (ColorPicker* pick, this->findChildren<ColorPicker*>())
    {
        pick->setColor(mStoredColors.takeFirst());
    }
    */
}

QString SettingsFormController::getPortName()
{
    return mUi.cboPort->currentText();
}

void SettingsFormController::setPortName(QString port)
{
    mUi.cboPort->setCurrentText(port);
}

int SettingsFormController::getBaudRate()
{
    return mUi.cboBaud->currentText().toInt();
}

void SettingsFormController::setBaudRate(int baud)
{
    mUi.cboBaud->setCurrentText(QString::number(baud));
}

double SettingsFormController::toolDiameter()
{
    return mUi.txtToolDiameter->value();
}

void SettingsFormController::setToolDiameter(double diameter)
{
    mUi.txtToolDiameter->setValue(diameter);
}

double SettingsFormController::toolLength()
{
    return mUi.txtToolLength->value();
}

void SettingsFormController::setToolLength(double length)
{
    mUi.txtToolLength->setValue(length);
}

bool SettingsFormController::antialiasing()
{
    return mUi.chkAntialiasing->isChecked();
}

void SettingsFormController::setAntialiasing(bool antialiasing)
{
    mUi.chkAntialiasing->setChecked(antialiasing);
}

bool SettingsFormController::zBuffer()
{
    return mUi.chkZBuffer->isChecked();
}

void SettingsFormController::setZBuffer(bool zBuffer)
{
    mUi.chkZBuffer->setChecked(zBuffer);
}

double SettingsFormController::lineWidth()
{
    return mUi.txtLineWidth->value();
}

void SettingsFormController::setLineWidth(double lineWidth)
{
    mUi.txtLineWidth->setValue(lineWidth);
}

double SettingsFormController::arcLength()
{
    return mUi.txtArcLength->value();
}

void SettingsFormController::setArcLength(double arcPrecision)
{
    mUi.txtArcLength->setValue(arcPrecision);
}

double SettingsFormController::arcDegree()
{
    return mUi.txtArcDegree->value();
}

void SettingsFormController::setArcDegree(double arcDegree)
{
    mUi.txtArcDegree->setValue(arcDegree);
}

double SettingsFormController::arcPrecision()
{
    return mUi.radArcDegreeMode->isChecked() ? mUi.txtArcDegree->value() : mUi.txtArcLength->value();
}

bool SettingsFormController::arcDegreeMode()
{
    return mUi.radArcDegreeMode->isChecked();
}

void SettingsFormController::setArcDegreeMode(bool arcDegreeMode)
{
    mUi.radArcDegreeMode->setChecked(arcDegreeMode);
}

bool SettingsFormController::showProgramCommands()
{
    return mUi.chkShowProgramCommands->isChecked();
}

void SettingsFormController::setShowProgramCommands(bool showAllCommands)
{
    mUi.chkShowProgramCommands->setChecked(showAllCommands);
}

bool SettingsFormController::showUICommands()
{
    return mUi.chkShowUICommands->isChecked();
}

void SettingsFormController::setShowUICommands(bool showUICommands)
{
    mUi.chkShowUICommands->setChecked(showUICommands);
}

QString SettingsFormController::safePositionCommand()
{
    return mUi.txtSafeCommand->text();
}

void SettingsFormController::setSafePositionCommand(QString command)
{
    mUi.txtSafeCommand->setText(command);
}

bool SettingsFormController::moveOnRestore()
{
    return mUi.chkMoveOnRestore->isChecked();
}

void SettingsFormController::setMoveOnRestore(bool value)
{
    mUi.chkMoveOnRestore->setChecked(value);
}

int SettingsFormController::restoreMode()
{
    return mUi.cboRestoreMode->currentIndex();
}

void SettingsFormController::setRestoreMode(int value)
{
    mUi.cboRestoreMode->setCurrentIndex(value);
}

int SettingsFormController::spindleSpeedMin()
{
    return mUi.txtSpindleSpeedMin->value();
}

void SettingsFormController::setSpindleSpeedMin(int speed)
{
    mUi.txtSpindleSpeedMin->setValue(speed);
}

int SettingsFormController::spindleSpeedMax()
{
    return mUi.txtSpindleSpeedMax->value();
}

void SettingsFormController::setSpindleSpeedMax(int speed)
{
    mUi.txtSpindleSpeedMax->setValue(speed);
}

int SettingsFormController::laserPowerMin()
{
    return mUi.txtLaserPowerMin->value();
}

void SettingsFormController::setLaserPowerMin(int value)
{
    mUi.txtLaserPowerMin->setValue(value);
}

int SettingsFormController::laserPowerMax()
{
    return mUi.txtLaserPowerMax->value();
}

void SettingsFormController::setLaserPowerMax(int value)
{
    mUi.txtLaserPowerMax->setValue(value);
}

int SettingsFormController::rapidSpeed()
{
    return mUi.txtRapidSpeed->value();
}

void SettingsFormController::setRapidSpeed(int rapidSpeed)
{
    mUi.txtRapidSpeed->setValue(rapidSpeed);
}

int SettingsFormController::heightmapProbingFeed()
{
    return mUi.txtHeightMapProbingFeed->value();
}

void SettingsFormController::setHeightmapProbingFeed(int heightmapProbingFeed)
{
    mUi.txtHeightMapProbingFeed->setValue(heightmapProbingFeed);
}

int SettingsFormController::acceleration()
{
    return mUi.txtAcceleration->value();
}

void SettingsFormController::setAcceleration(int acceleration)
{
    mUi.txtAcceleration->setValue(acceleration);
}

int SettingsFormController::queryStateTime()
{
    return mUi.txtQueryStateTime->value();
}

void SettingsFormController::setQueryStateTime(int queryStateTime)
{
    mUi.txtQueryStateTime->setValue(queryStateTime);
}

int SettingsFormController::toolType()
{
    return mUi.cboToolType->currentIndex();
}

void SettingsFormController::setToolType(int toolType)
{
    mUi.cboToolType->setCurrentIndex(toolType);
}

double SettingsFormController::toolAngle()
{
    return mUi.txtToolAngle->value();
}

void SettingsFormController::setToolAngle(double toolAngle)
{
    mUi.txtToolAngle->setValue(toolAngle);
}

int SettingsFormController::fps()
{
    return mUi.cboFps->currentText().toInt();
}

void SettingsFormController::setFps(int fps)
{
    mUi.cboFps->setCurrentText(QString::number(fps));
}

bool SettingsFormController::vsync()
{
    return mUi.chkVSync->isChecked();
}

void SettingsFormController::setVsync(bool value)
{
    mUi.chkVSync->setChecked(value);
}

bool SettingsFormController::msaa()
{
    return mUi.radMSAA->isChecked();
}

void SettingsFormController::setMsaa(bool msaa)
{
    mUi.radMSAA->setChecked(msaa);
}

bool SettingsFormController::autoCompletion()
{
    return mUi.chkAutocompletion->isChecked();
}

void SettingsFormController::setAutoCompletion(bool autoCompletion)
{
    mUi.chkAutocompletion->setChecked(autoCompletion);
}

int SettingsFormController::units()
{
    return mUi.cboUnits->currentIndex();
}

void SettingsFormController::setUnits(int units)
{
    mUi.cboUnits->setCurrentIndex(units);
}

QString SettingsFormController::touchCommand()
{
    return mUi.txtTouchCommand->text();
}

void SettingsFormController::setTouchCommand(QString touchCommand)
{
    mUi.txtTouchCommand->setText(touchCommand);
}

bool SettingsFormController::simplify()
{
    return mUi.chkSimplify->isChecked();
}

void SettingsFormController::setSimplify(bool simplify)
{
    mUi.chkSimplify->setChecked(simplify);
}

double SettingsFormController::simplifyPrecision()
{
    return mUi.txtSimplifyPrecision->value();
}

void SettingsFormController::setSimplifyPrecision(double simplifyPrecision)
{
    mUi.txtSimplifyPrecision->setValue(simplifyPrecision);
}

bool SettingsFormController::panelUserCommands()
{
    return mUi.chkPanelUserCommands->isChecked();
}

void SettingsFormController::setPanelUserCommands(bool value)
{
    mUi.chkPanelUserCommands->setChecked(value);
}

bool SettingsFormController::panelHeightmap()
{
    return mUi.chkPanelHeightmap->isChecked();
}

void SettingsFormController::setPanelHeightmap(bool panelHeightmap)
{
    mUi.chkPanelHeightmap->setChecked(panelHeightmap);
}

bool SettingsFormController::panelSpindle()
{
    return mUi.chkPanelSpindle->isChecked();
}

void SettingsFormController::setPanelSpindle(bool panelSpindle)
{
    mUi.chkPanelSpindle->setChecked(panelSpindle);
}

bool SettingsFormController::panelFeed()
{
    return mUi.chkPanelFeed->isChecked();
}

void SettingsFormController::setPanelFeed(bool panelFeed)
{
    mUi.chkPanelFeed->setChecked(panelFeed);
}

bool SettingsFormController::panelJog()
{
    return mUi.chkPanelJog->isChecked();
}

void SettingsFormController::setPanelJog(bool panelJog)
{
    mUi.chkPanelJog->setChecked(panelJog);
}

/*QList<ColorPicker *> SettingsFormController::colors()
{
    return this->findChildren<ColorPicker*>();
}
*/

/*
QColor SettingsFormController::colors(QString name)
{
    ColorPicker *pick = this->findChildren<ColorPicker*>("clp" + name).at(0);
    if (pick) return pick->color(); else return QColor();
}
*/

int SettingsFormController::fontSize()
{
    return mUi.cboFontSize->currentText().toInt();
}

void SettingsFormController::setFontSize(int fontSize)
{
    mUi.cboFontSize->setCurrentText(QString::number(fontSize));
}

bool SettingsFormController::grayscaleSegments()
{
    return mUi.chkGrayscale->isChecked();
}

void SettingsFormController::setGrayscaleSegments(bool value)
{
    mUi.chkGrayscale->setChecked(value);
}

bool SettingsFormController::grayscaleSCode()
{
    return mUi.radGrayscaleS->isChecked();
}

void SettingsFormController::setGrayscaleSCode(bool value)
{
    mUi.radGrayscaleS->setChecked(value);
    mUi.radGrayscaleZ->setChecked(!value);
}

bool SettingsFormController::drawModeVectors()
{
    return mUi.radDrawModeVectors->isChecked();
}

void SettingsFormController::setDrawModeVectors(bool value)
{
    mUi.radDrawModeVectors->setChecked(value);
    mUi.radDrawModeRaster->setChecked(!value);
}

QString SettingsFormController::userCommands(int index)
{
    return this->findChild<QLineEdit*>(QString("txtUserCommand%1").arg(index))->text();
}

void SettingsFormController::setUserCommands(int index, QString commands)
{
    this->findChild<QLineEdit*>(QString("txtUserCommand%1").arg(index))->setText(commands);
}

bool SettingsFormController::ignoreErrors()
{
    return mUi.chkIgnoreErrors->isChecked();
}

void SettingsFormController::setIgnoreErrors(bool value)
{
    mUi.chkIgnoreErrors->setChecked(value);
}

bool SettingsFormController::autoLine()
{
    return mUi.chkAutoLine->isChecked();
}

void SettingsFormController::setAutoLine(bool value)
{
    mUi.chkAutoLine->setChecked(value);
}

void SettingsFormController::showEvent(QShowEvent *se)
{
    Q_UNUSED(se)

   // mUi.scrollSettings->updateMinimumWidth();
}

void SettingsFormController::searchPorts()
{
    mUi.cboPort->clear();

    foreach (QSerialPortInfo info ,QSerialPortInfo::availablePorts()) {
//        mUi.cboPort->addItem(info.portName());
        mUi.cboPort->insertItem(0, info.portName());
    }
}

void SettingsFormController::setupSignalSlots()
{

    qDebug() << "SettingsFormController: Setup Signals/Slots";
}

void SettingsFormController::onCmdRefreshClicked()
{
    searchPorts();
}

void SettingsFormController::onCmdOKClicked()
{
    mDialog.accept();
}

void SettingsFormController::onCmdCancelClicked()
{
    mDialog.reject();
}

void SettingsFormController::onComboBoxToolTypeCurrentIndexChanged(int index)
{
    mUi.lblToolAngle->setEnabled(index == 1);
    mUi.txtToolAngle->setEnabled(index == 1);
}

void SettingsFormController::onCmdDefaultsClicked()
{
    QMessageBox::StandardButton result = QMessageBox::warning
    (
        &mDialog,
        qApp->applicationDisplayName(),
        tr("Reset settings to default values?"),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
    );

    if (result != QMessageBox::Yes)
    {
        return;
    }

    setPortName("");
    setBaudRate(115200);

    setIgnoreErrors(false);

    setQueryStateTime(40);
    setRapidSpeed(2000);
    setAcceleration(100);
    setSpindleSpeedMin(0);
    setSpindleSpeedMax(10000);
    setLaserPowerMin(0);
    setLaserPowerMax(100);
    setTouchCommand("G21G91G38.2Z-30F100; G0Z1; G38.2Z-2F10");
    setSafePositionCommand("G21G90; G53G0Z0");
    setMoveOnRestore(false);
    setRestoreMode(0);
    setHeightmapProbingFeed(10);
    setUnits(0);

    setArcLength(0.0);
    setArcDegreeMode(true);
    setArcDegree(5.0);

    setLineWidth(1.5);
    setAntialiasing(true);
    setMsaa(true);
    setSimplify(true);
    setSimplifyPrecision(0.0);
    setFps(60);
    setZBuffer(false);
    setGrayscaleSegments(false);
    setGrayscaleSCode(true);
    setDrawModeVectors(true);

    setToolType(1);
    setToolAngle(15.0);
    setToolDiameter(3.0);
    setToolLength(30.0);

    setShowProgramCommands(false);
    setAutoCompletion(true);

    setPanelFeed(true);
    setPanelHeightmap(true);
    setPanelJog(true);
    setPanelSpindle(true);

    setFontSize(9);
}

void SettingsFormController::onComboBoxFontSizeCurrentTextChanged(const QString &arg1)
{
    qApp->setStyleSheet(QString(qApp->styleSheet()).replace(QRegExp("font-size:\\s*\\d+"), "font-size: " + arg1));
}

void SettingsFormController::onRadioBtnDrawModeVectorsToggled(bool checked)
{
    mUi.chkSimplify->setEnabled(checked);
    mUi.lblSimpilyPrecision->setEnabled(checked && mUi.chkSimplify->isChecked());
    mUi.txtSimplifyPrecision->setEnabled(checked && mUi.chkSimplify->isChecked());

    mUi.radDrawModeRaster->setChecked(!checked);
}

void SettingsFormController::onRadioBtnDrawModeRasterToggled(bool checked)
{
    mUi.radDrawModeVectors->setChecked(!checked);
}

void SettingsFormController::onRadioBtnGrayscaleSToggled(bool checked)
{
    mUi.radGrayscaleZ->setChecked(!checked);
}

void SettingsFormController::onRadioBtnGrayscaleZToggled(bool checked)
{
    mUi.radGrayscaleS->setChecked(!checked);
}

// Settings
void SettingsFormController::applySettings()
{
//    mOriginDrawer->setLineWidth(mSettingsForm->lineWidth());
//    m_toolDrawer.setToolDiameter(mSettingsForm->toolDiameter());
//    m_toolDrawer.setToolLength(mSettingsForm->toolLength());
//    m_toolDrawer.setLineWidth(mSettingsForm->lineWidth());
//    mCodeDrawer->setLineWidth(mSettingsForm->lineWidth());
//    m_heightMapBorderDrawer.setLineWidth(mSettingsForm->lineWidth());
//    m_heightMapGridDrawer.setLineWidth(0.1);
//    m_heightMapInterpolationDrawer.setLineWidth(mSettingsForm->lineWidth());
//    mUi->glwVisualizer->setLineWidth(mSettingsForm->lineWidth());
//    mStateQueryTimer.setInterval(mSettingsForm->queryStateTime());

//    m_toolDrawer.setToolAngle(mSettingsForm->toolType() == 0 ? 180 : mSettingsForm->toolAngle());
//    m_toolDrawer.setColor(mSettingsForm->colors("Tool"));
//    m_toolDrawer.update();

//    mUi->glwVisualizer->setAntialiasing(mSettingsForm->antialiasing());
//    mUi->glwVisualizer->setMsaa(mSettingsForm->msaa());
//    mUi->glwVisualizer->setZBuffer(mSettingsForm->zBuffer());
//    mUi->glwVisualizer->setVsync(mSettingsForm->vsync());
//    mUi->glwVisualizer->setFps(mSettingsForm->fps());
//    mUi->glwVisualizer->setColorBackground(mSettingsForm->colors("VisualizerBackground"));
//    mUi->glwVisualizer->setColorText(mSettingsForm->colors("VisualizerText"));

//    mUi->txtSpindleSpeed->setMinimum(mSettingsForm->spindleSpeedMin());
//    mUi->txtSpindleSpeed->setMaximum(mSettingsForm->spindleSpeedMax());
//    mUi->sliSpindleSpeed->setMinimum(mUi->txtSpindleSpeed->minimum() / 100);
//    mUi->sliSpindleSpeed->setMaximum(mUi->txtSpindleSpeed->maximum() / 100);

//    mUi->scrollArea->setVisible(mSettingsForm->panelHeightmap() || mSettingsForm->panelFeed()
//                                || mSettingsForm->panelJog() || mSettingsForm->panelSpindle());

//    mUi->grpUserCommands->setVisible(mSettingsForm->panelUserCommands());
//    mUi->grpHeightMap->setVisible(mSettingsForm->panelHeightmap());
//    mUi->grpSpindle->setVisible(mSettingsForm->panelSpindle());
//    mUi->grpFeed->setVisible(mSettingsForm->panelFeed());
//    mUi->grpJog->setVisible(mSettingsForm->panelJog());

//    mUi->cboCommand->setAutoCompletion(mSettingsForm->autoCompletion());

//    mCodeDrawer->setSimplify(mSettingsForm->simplify());
//    mCodeDrawer->setSimplifyPrecision(mSettingsForm->simplifyPrecision());
//    mCodeDrawer->setColorNormal(mSettingsForm->colors("ToolpathNormal"));
//    mCodeDrawer->setColorDrawn(mSettingsForm->colors("ToolpathDrawn"));
//    mCodeDrawer->setColorHighlight(mSettingsForm->colors("ToolpathHighlight"));
//    mCodeDrawer->setColorZMovement(mSettingsForm->colors("ToolpathZMovement"));
//    mCodeDrawer->setColorStart(mSettingsForm->colors("ToolpathStart"));
//    mCodeDrawer->setColorEnd(mSettingsForm->colors("ToolpathEnd"));
//    mCodeDrawer->setIgnoreZ(mSettingsForm->grayscaleSegments() || !mSettingsForm->drawModeVectors());
//    mCodeDrawer->setGrayscaleSegments(mSettingsForm->grayscaleSegments());
//    mCodeDrawer->setGrayscaleCode(mSettingsForm->grayscaleSCode() ? GcodeDrawer::S : GcodeDrawer::Z);
//    mCodeDrawer->setDrawMode(mSettingsForm->drawModeVectors() ? GcodeDrawer::Vectors : GcodeDrawer::Raster);
//    mCodeDrawer->setGrayscaleMin(mSettingsForm->laserPowerMin());
//    mCodeDrawer->setGrayscaleMax(mSettingsForm->laserPowerMax());
//    mCodeDrawer->update();

//    m_selectionDrawer.setColor(mSettingsForm->colors("ToolpathHighlight"));

//    // Adapt visualizer buttons colors
//    const int LIGHTBOUND = 127;
//    const int NORMALSHIFT = 40;
//    const int HIGHLIGHTSHIFT = 80;

//    QColor base = mSettingsForm->colors("VisualizerBackground");
//    bool light = base.value() > LIGHTBOUND;
//    QColor normal, highlight;

//    normal.setHsv(base.hue(), base.saturation(), base.value() + (light ? -NORMALSHIFT : NORMALSHIFT));
//    highlight.setHsv(base.hue(), base.saturation(), base.value() + (light ? -HIGHLIGHTSHIFT : HIGHLIGHTSHIFT));

//    mUi->glwVisualizer->setStyleSheet(
//        QString("QToolButton {border: 1px solid %1; background-color: %3} QToolButton:hover {border: 1px solid %2;}")
//            .arg(normal.name())
//            .arg(highlight.name())
//            .arg(base.name())
//    );

//    mUi->cmdFit->setIcon(QIcon(":/images/fit_1.png"));
//    mUi->cmdIsometric->setIcon(QIcon(":/images/cube.png"));
//    mUi->cmdFront->setIcon(QIcon(":/images/cubeFront.png"));
//    mUi->cmdLeft->setIcon(QIcon(":/images/cubeLeft.png"));
//    mUi->cmdTop->setIcon(QIcon(":/images/cubeTop.png"));

//    if (!light) {
//        Util::invertButtonIconColors(mUi->cmdFit);
//        Util::invertButtonIconColors(mUi->cmdIsometric);
//        Util::invertButtonIconColors(mUi->cmdFront);
//        Util::invertButtonIconColors(mUi->cmdLeft);
//        Util::invertButtonIconColors(mUi->cmdTop);
//    }

//    mUi->cboCommand->setMinimumHeight(mUi->cboCommand->height());
//    mUi->cmdClearConsole->setFixedHeight(mUi->cboCommand->height());
//    mUi->cmdCommandSend->setFixedHeight(mUi->cboCommand->height());

//    foreach (StyledToolButton* button, this->findChildren<StyledToolButton*>(QRegExp("cmdUser\\d")))
//    {
//        button->setToolTip(mSettingsForm->userCommands(button->objectName().right(1).toInt()));
//        button->setEnabled(!button->toolTip().isEmpty());
//    }
}

