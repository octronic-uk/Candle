/*
 * HeightMapFormController.cpp
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
#include "HeightMapFormController.h"

HeightMapFormController::HeightMapFormController(QWidget *parent)
    : AbstractFormController(parent)
{
    qDebug() << "HeightMapFormController: Contructing";
    mUi.setupUi(this);
    setupSignalSlots();
    /*
    mHeightMapGridDrawer.setModel(&mHeightMapModel);
    mHeightMapMode = false;
    connect
    (
        &mProgramHeightmapTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
        this, SLOT(onTableCellChanged(QModelIndex,QModelIndex))
    );

    connect
    (
        &mProbeTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
        this, SLOT(onTableCellChanged(QModelIndex,QModelIndex))
    );

    connect
    (
        &mHeightMapModel, SIGNAL(dataChangedByUserInput()),
        this, SLOT(updateHeightMapInterpolationDrawer())
    );
    */
}

HeightMapFormController::~HeightMapFormController()
{
    qDebug() << "HeightMapFormController: Destructing";
}

void HeightMapFormController::onCheckBoxHeightMapBorderShowToggled(bool checked)
{
    /*
    Q_UNUSED(checked)

    updateControlsState();
    */
}

void HeightMapFormController::onTextHeightMapBorderXValueChanged(double arg1)
{
    updateHeightMapBorderDrawer();
    updateHeightMapGrid(arg1);
}

void HeightMapFormController::onTextHeightMapBorderWidthValueChanged(double arg1)
{
    updateHeightMapBorderDrawer();
    updateHeightMapGrid(arg1);
}

void HeightMapFormController::onTextHeightMapBorderYValueChanged(double arg1)
{
    updateHeightMapBorderDrawer();
    updateHeightMapGrid(arg1);
}

void HeightMapFormController::onTextHeightMapBorderHeightValueChanged(double arg1)
{
    updateHeightMapBorderDrawer();
    updateHeightMapGrid(arg1);
}

void HeightMapFormController::onCheckBoxHeightMapGridShowToggled(bool checked)
{
    /*
    Q_UNUSED(checked)
    updateControlsState();
    */
}

void HeightMapFormController::onTextHeightMapGridXValueChanged(double arg1)
{
    updateHeightMapGrid(arg1);
}

void HeightMapFormController::onTextHeightMapGridYValueChanged(double arg1)
{
    updateHeightMapGrid(arg1);
}

void HeightMapFormController::onTextHeightMapGridZBottomValueChanged(double arg1)
{
    updateHeightMapGrid(arg1);
}

void HeightMapFormController::onTextHeightMapGridZTopValueChanged(double arg1)
{
    updateHeightMapGrid(arg1);
}

void HeightMapFormController::onCmdHeightMapModeToggled(bool checked)
{
    /*
    // Update flag
    m_heightMapMode = checked;

    // Reset file progress
    m_fileCommandIndex = 0;
    m_fileProcessedCommandIndex = 0;
    m_lastDrawnLineIndex = 0;

    // Reset/restore g-code program modification on edit mode enter/exit
    if (mUi->chkHeightMapUse->isChecked()) {
        onCheckBoxHeightMapUseClicked(!checked); // Update gcode program parser
//        m_codeDrawer->updateData(); // Force update data to properly shadowing
    }

    if (checked) {
        mUi->tblProgram->setModel(&m_probeModel);
        resizeTableHeightMapSections();
        m_currentModel = &m_probeModel;
        mCurrentDrawer = mProbeDrawer;
        updateParser();  // Update probe program parser
    } else {
        m_probeParser.reset();
        if (!mUi->chkHeightMapUse->isChecked()) {
            mUi->tblProgram->setModel(&m_programModel);
            connect(mUi->tblProgram->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex)));
            mUi->tblProgram->selectRow(0);

            resizeTableHeightMapSections();
            m_currentModel = &m_programModel;
            mCurrentDrawer = mCodeDrawer;

            if (!mUi->chkHeightMapUse->isChecked()) {
                mUi->glwVisualizer->updateExtremes(mCodeDrawer);
                updateProgramEstimatedTime(mCurrentDrawer->viewParser()->getLineSegmentList());
            }
        }
    }

    // Shadow toolpath
    QList<LineSegment*> list = m_viewParser.getLineSegmentList();
    QList<int> indexes;
    for (int i = m_lastDrawnLineIndex; i < list.count(); i++) {
        list[i]->setDrawn(checked);
        list[i]->setIsHightlight(false);
        indexes.append(i);
    }
    // Update only vertex color.
    // If chkHeightMapUse was checked codeDrawer updated via updateParser
    if (!mUi->chkHeightMapUse->isChecked()) mCodeDrawer->update(indexes);

    updateRecentFilesMenu();
    updateControlsState();
    */
}


void HeightMapFormController::onCheckBoxHeightMapInterpolationShowToggled(bool checked)
{
    /*
    Q_UNUSED(checked)
    updateControlsState();
    */
}

void HeightMapFormController::onCmdHeightMapLoadClicked()
{
    /*
    if (!saveChanges(true)) {
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), mLastFolder, tr("Heightmap files (*.map)"));

    if (fileName != "") {
        addRecentHeightmap(fileName);
        loadHeightMap(fileName);

        // If using heightmap
        if (mUi->chkHeightMapUse->isChecked() && !m_heightMapMode) {
            // Restore original file
            onCheckBoxHeightMapUseClicked(false);
            // Apply heightmap
            onCheckBoxHeightMapUseClicked(true);
        }

        updateRecentFilesMenu();
        updateControlsState(); // Enable 'cmdHeightMapMode' button
    }
    */
}

void HeightMapFormController::onTextHeightMapInterpolationStepXValueChanged(double arg1)
{
    /*
    Q_UNUSED(arg1)
    updateHeightMapInterpolationDrawer();
    */
}

void HeightMapFormController::onTextHeightMapInterpolationStepYValueChanged(double arg1)
{
    /*
    Q_UNUSED(arg1)
    updateHeightMapInterpolationDrawer();
    */
}

void HeightMapFormController::onCheckBoxHeightMapUseClicked(bool checked)
{
    /*
//    static bool fileChanged;

    // Reset table view
    QByteArray headerState = mUi->tblProgram->horizontalHeader()->saveState();
    mUi->tblProgram->setModel(NULL);

    CancelException cancel;

    if (checked) try {

        // Prepare progress dialog
        QProgressDialog progress(tr("Applying heightmap..."), tr("Abort"), 0, 0, this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setFixedHeight(progress.sizeHint().height());
        progress.show();
        progress.setStyleSheet("QProgressBar {text-align: center; qproperty-format: \"\"}");

        // Performance test
        QTime time;

        // Store fileChanged state
//        fileChanged = m_fileChanged;

        // Set current model to prevent reseting heightmap cache
        m_currentModel = &m_programHeightmapModel;

        // Update heightmap-modificated program if not cached
        if (m_programHeightmapModel.rowCount() == 0) {

            // Modifying linesegments
            QList<LineSegment*> *list = m_viewParser.getLines();
            QRectF borderRect = borderRectFromTextboxes();
            double x, y, z;
            QVector3D point;

            progress.setLabelText(tr("Subdividing segments..."));
            progress.setMaximum(list->count() - 1);
            time.start();

            for (int i = 0; i < list->count(); i++) {
                if (!list->at(i)->isZMovement()) {
                    QList<LineSegment*> subSegments = subdivideSegment(list->at(i));

                    if (subSegments.count() > 0) {
                        delete list->at(i);
                        list->removeAt(i);
                        foreach (LineSegment* subSegment, subSegments) list->insert(i++, subSegment);
                        i--;
                    }
                }

                if (progress.isVisible() && (i % PROGRESS_STEP == 0)) {
                    progress.setMaximum(list->count() - 1);
                    progress.setValue(i);
                    qApp->processEvents();
                    if (progress.wasCanceled()) throw cancel;
                }
            }

            qDebug() << "Subdivide time: " << time.elapsed();

            progress.setLabelText(tr("Updating Z-coordinates..."));
            progress.setMaximum(list->count() - 1);
            time.start();

            for (int i = 0; i < list->count(); i++) {
                if (i == 0) {
                    x = list->at(i)->getStart().x();
                    y = list->at(i)->getStart().y();
                    z = list->at(i)->getStart().z() + Interpolation::bicubicInterpolate(borderRect, &m_heightMapModel, x, y);
                    list->at(i)->setStart(QVector3D(x, y, z));
                } else list->at(i)->setStart(list->at(i - 1)->getEnd());

                x = list->at(i)->getEnd().x();
                y = list->at(i)->getEnd().y();
                z = list->at(i)->getEnd().z() + Interpolation::bicubicInterpolate(borderRect, &m_heightMapModel, x, y);
                list->at(i)->setEnd(QVector3D(x, y, z));

                if (progress.isVisible() && (i % PROGRESS_STEP == 0)) {
                    progress.setValue(i);
                    qApp->processEvents();
                    if (progress.wasCanceled()) throw cancel;
                }
            }

            qDebug() << "Z update time (interpolation): " << time.elapsed();

            progress.setLabelText(tr("Modifying G-code program..."));
            progress.setMaximum(m_programModel.rowCount() - 2);
            time.start();

            // Modifying g-code program
            QString command;
            QStringList args;
            int line;
            QString newCommand;
            GCodeItem item;
            int lastSegmentIndex = 0;
            int lastCommandIndex = -1;

            // Search strings
            QString coords("XxYyZzIiJjKkRr");
            QString g("Gg");
            QString m("Mm");

            char codeChar;          // Single code char G1 -> G
            float codeNum;          // Code number      G1 -> 1

            QString lastCode;
            bool isLinearMove;
            bool hasCommand;

            m_programLoading = true;
            for (int i = 0; i < m_programModel.rowCount() - 1; i++) {
                command = m_programModel.data().at(i).command;
                line = m_programModel.data().at(i).line;
                isLinearMove = false;
                hasCommand = false;

                if (line < 0 || line == lastCommandIndex || lastSegmentIndex == list->count() - 1) {
                    item.command = command;
                    m_programHeightmapModel.data().append(item);
                } else {
                    // Get commands args
                    args = m_programModel.data().at(i).args;
                    newCommand.clear();

                    // Parse command args
                    foreach (QString arg, args) {                   // arg examples: G1, G2, M3, X100...
                        codeChar = arg.at(0).toLatin1();            // codeChar: G, M, X...
                        if (!coords.contains(codeChar)) {           // Not parameter
                            codeNum = arg.mid(1).toDouble();
                            if (g.contains(codeChar)) {             // 'G'-command
                                // Store 'G0' & 'G1'
                                if (codeNum == 0.0f || codeNum == 1.0f) {
                                    lastCode = arg;
                                    isLinearMove = true;            // Store linear move
                                }

                                // Replace 'G2' & 'G3' with 'G1'
                                if (codeNum == 2.0f || codeNum == 3.0f) {
                                    newCommand.append("G1");
                                    isLinearMove = true;
                                // Drop plane command for arcs
                                } else if (codeNum != 17.0f && codeNum != 18.0f && codeNum != 19.0f) {
                                    newCommand.append(arg);
                                }

                                hasCommand = true;                  // Command has 'G'
                            } else {
                                if (m.contains(codeChar))
                                    hasCommand = true;              // Command has 'M'
                                newCommand.append(arg);       // Other commands
                            }
                        }
                    }

                    // Find first linesegment by command index
                    for (int j = lastSegmentIndex; j < list->count(); j++) {
                        if (list->at(j)->getLineNumber() == line) {
                            if (!qIsNaN(list->at(j)->getEnd().length()) && (isLinearMove || (!hasCommand && !lastCode.isEmpty()))) {
                                // Create new commands for each linesegment with given command index
                                while ((j < list->count()) && (list->at(j)->getLineNumber() == line)) {

                                    point = list->at(j)->getEnd();
                                    if (!list->at(j)->isAbsolute()) point -= list->at(j)->getStart();
                                    if (!list->at(j)->isMetric()) point /= 25.4;

                                    item.command = newCommand + QString("X%1Y%2Z%3")
                                            .arg(point.x(), 0, 'f', 3).arg(point.y(), 0, 'f', 3).arg(point.z(), 0, 'f', 3);
                                    m_programHeightmapModel.data().append(item);

                                    if (!newCommand.isEmpty()) newCommand.clear();
                                    j++;
                                }
                            // Copy original command if not G0 or G1
                            } else {
                                item.command = command;
                                m_programHeightmapModel.data().append(item);
                            }

                            lastSegmentIndex = j;
                            break;
                        }
                    }
                }
                lastCommandIndex = line;

                if (progress.isVisible() && (i % PROGRESS_STEP == 0)) {
                    progress.setValue(i);
                    qApp->processEvents();
                    if (progress.wasCanceled()) throw cancel;
                }
            }
            m_programHeightmapModel.insertRow(m_programHeightmapModel.rowCount());
        }
        progress.close();

        qDebug() << "Program modification time: " << time.elapsed();

        mUi->tblProgram->setModel(&m_programHeightmapModel);
        mUi->tblProgram->horizontalHeader()->restoreState(headerState);

        connect(mUi->tblProgram->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex)));

        m_programLoading = false;

        // Update parser
        mCurrentDrawer = mCodeDrawer;
        updateParser();

        // Select first row
        mUi->tblProgram->selectRow(0);
    }
    catch (CancelException) {                       // Cancel modification
        m_programHeightmapModel.clear();
        m_currentModel = &m_programModel;

        mUi->tblProgram->setModel(&m_programModel);
        mUi->tblProgram->horizontalHeader()->restoreState(headerState);

        connect(mUi->tblProgram->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex)));
        mUi->tblProgram->selectRow(0);

        mUi->chkHeightMapUse->setChecked(false);

        return;
    } else {                                        // Restore original program
        m_currentModel = &m_programModel;

        mUi->tblProgram->setModel(&m_programModel);
        mUi->tblProgram->horizontalHeader()->restoreState(headerState);

        connect(mUi->tblProgram->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex)));

        // Store changes flag
        bool fileChanged = m_fileChanged;

        // Update parser
        updateParser();

        // Select first row
        mUi->tblProgram->selectRow(0);

        // Restore changes flag
        m_fileChanged = fileChanged;
    }

    // Update groupbox title
    mUi->grpHeightMap->setProperty("overrided", checked);
    style()->unpolish(mUi->grpHeightMap);
    mUi->grpHeightMap->ensurePolished();

    // Update menu
    mUi->actFileSaveTransformedAs->setVisible(checked);
    */
}

void HeightMapFormController::onGripHeightMapToggled(bool arg1)
{
    //mUi->widgetHeightMap->setVisible(arg1);
}

void HeightMapFormController::updateHeightMapBorderDrawer()
{
    /*
    if (m_settingsLoading) return;

    qDebug() << "updating border drawer";

    m_heightMapBorderDrawer.setBorderRect(borderRectFromTextboxes());
    */
}

void HeightMapFormController::updateHeightMapGrid(double arg1)
{
    /*
    if (sender()->property("previousValue").toDouble() != arg1 && !updateHeightMapGrid())
        static_cast<QDoubleSpinBox*>(sender())->setValue(sender()->property("previousValue").toDouble());
    else sender()->setProperty("previousValue", arg1);
    */
}

bool HeightMapFormController::updateHeightMapGrid()
{
    /*
    if (m_settingsLoading) return true;

    qDebug() << "updating heightmap grid drawer";

    // Grid map changing warning
    bool nan = true;
    for (int i = 0; i < m_heightMapModel.rowCount(); i++)
        for (int j = 0; j < m_heightMapModel.columnCount(); j++)
            if (!qIsNaN(m_heightMapModel.data(m_heightMapModel.index(i, j), Qt::UserRole).toDouble())) {
                nan = false;
                break;
            }
    if (!nan && QMessageBox::warning(this, this->windowTitle(), tr("Changing grid settings will reset probe data. Continue?"),
                                                           QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) return false;

    // Update grid drawer
    QRectF borderRect = borderRectFromTextboxes();
    m_heightMapGridDrawer.setBorderRect(borderRect);
    m_heightMapGridDrawer.setGridSize(QPointF(mUi->txtHeightMapGridX->value(), mUi->txtHeightMapGridY->value()));
    m_heightMapGridDrawer.setZBottom(mUi->txtHeightMapGridZBottom->value());
    m_heightMapGridDrawer.setZTop(mUi->txtHeightMapGridZTop->value());

    // Reset model
//    int gridPointsX = trunc(borderRect.width() / mUi->txtHeightMapGridX->value()) + 1;
//    int gridPointsY = trunc(borderRect.height() / mUi->txtHeightMapGridY->value()) + 1;
    int gridPointsX = mUi->txtHeightMapGridX->value();
    int gridPointsY = mUi->txtHeightMapGridY->value();

    m_heightMapModel.resize(gridPointsX, gridPointsY);
    mUi->tblHeightMap->setModel(NULL);
    mUi->tblHeightMap->setModel(&m_heightMapModel);
    resizeTableHeightMapSections();

    // Update interpolation
    updateHeightMapInterpolationDrawer(true);

    // Generate probe program
    double gridStepX = gridPointsX > 1 ? borderRect.width() / (gridPointsX - 1) : 0;
    double gridStepY = gridPointsY > 1 ? borderRect.height() / (gridPointsY - 1) : 0;

    qDebug() << "generating probe program";

    m_programLoading = true;
    m_probeModel.clear();
    m_probeModel.insertRow(0);

    m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G21G90F%1G0Z%2").
                         arg(mSettingsForm->heightmapProbingFeed()).arg(mUi->txtHeightMapGridZTop->value()));
    m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G0X0Y0"));
//                         .arg(mUi->txtHeightMapGridZTop->value()));
    m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G38.2Z%1")
                         .arg(mUi->txtHeightMapGridZBottom->value()));
    m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G0Z%1")
                         .arg(mUi->txtHeightMapGridZTop->value()));

    double x, y;

    for (int i = 0; i < gridPointsY; i++) {
        y = borderRect.top() + gridStepY * i;
        for (int j = 0; j < gridPointsX; j++) {
            x = borderRect.left() + gridStepX * (i % 2 ? gridPointsX - 1 - j : j);
            m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G0X%1Y%2")
                                 .arg(x, 0, 'f', 3).arg(y, 0, 'f', 3));
            m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G38.2Z%1")
                                 .arg(mUi->txtHeightMapGridZBottom->value()));
            m_probeModel.setData(m_probeModel.index(m_probeModel.rowCount() - 1, 1), QString("G0Z%1")
                                 .arg(mUi->txtHeightMapGridZTop->value()));
        }
    }

    m_programLoading = false;

    if (mCurrentDrawer == mProbeDrawer) updateParser();

    m_heightMapChanged = true;
    return true;
    */
}

void HeightMapFormController::updateHeightMapInterpolationDrawer(bool reset)
{
    /*
    if (m_settingsLoading) return;

    qDebug() << "Updating interpolation";

    QRectF borderRect = borderRectFromTextboxes();
    m_heightMapInterpolationDrawer.setBorderRect(borderRect);

    QVector<QVector<double>> *interpolationData = new QVector<QVector<double>>;

    int interpolationPointsX = mUi->txtHeightMapInterpolationStepX->value();// * (mUi->txtHeightMapGridX->value() - 1) + 1;
    int interpolationPointsY = mUi->txtHeightMapInterpolationStepY->value();// * (mUi->txtHeightMapGridY->value() - 1) + 1;

    double interpolationStepX = interpolationPointsX > 1 ? borderRect.width() / (interpolationPointsX - 1) : 0;
    double interpolationStepY = interpolationPointsY > 1 ? borderRect.height() / (interpolationPointsY - 1) : 0;

    for (int i = 0; i < interpolationPointsY; i++) {
        QVector<double> row;
        for (int j = 0; j < interpolationPointsX; j++) {

            double x = interpolationStepX * j + borderRect.x();
            double y = interpolationStepY * i + borderRect.y();

            row.append(reset ? qQNaN() : Interpolation::bicubicInterpolate(borderRect, &m_heightMapModel, x, y));
        }
        interpolationData->append(row);
    }

    if (m_heightMapInterpolationDrawer.data() != NULL) {
        delete m_heightMapInterpolationDrawer.data();
    }
    m_heightMapInterpolationDrawer.setData(interpolationData);

    // Update grid drawer
    m_heightMapGridDrawer.update();

    // Heightmap changed by table user input
    if (sender() == &m_heightMapModel) m_heightMapChanged = true;

    // Reset heightmapped program model
    m_programHeightmapModel.clear();
    */
}


void HeightMapFormController::onCmdHeightMapCreateClicked()
{
    /*
    mUi->cmdHeightMapMode->setChecked(true);
    on_actFileNew_triggered();
    */
}

void HeightMapFormController::onCmdHeightMapBorderAutoClicked()
{
    /*
    QRectF rect = borderRectFromExtremes();

    if (!qIsNaN(rect.width()) && !qIsNaN(rect.height())) {
        mUi->txtHeightMapBorderX->setValue(rect.x());
        mUi->txtHeightMapBorderY->setValue(rect.y());
        mUi->txtHeightMapBorderWidth->setValue(rect.width());
        mUi->txtHeightMapBorderHeight->setValue(rect.height());
    }
    */
}

void HeightMapFormController::resizeTableHeightMapSections()
{
    /*
    if (mUi->tblHeightMap->horizontalHeader()->defaultSectionSize()
            * mUi->tblHeightMap->horizontalHeader()->count() < mUi->glwVisualizer->width())
        mUi->tblHeightMap->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); else {
        mUi->tblHeightMap->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }
    */
}

void HeightMapFormController::setHeightMapMode(bool)
{

}

void HeightMapFormController::resetHeightmap()
{
    /*
    delete m_heightMapInterpolationDrawer.data();
    m_heightMapInterpolationDrawer.setData(NULL);
//    updateHeightMapInterpolationDrawer();

    mUi->tblHeightMap->setModel(NULL);
    m_heightMapModel.resize(1, 1);

    mUi->txtHeightMap->clear();
    mHeightMapFileName.clear();
    m_heightMapChanged = false;
    */
}

void HeightMapFormController::setupSignalSlots()
{

    qDebug() << "HeightMapFormController: Setup Signals/Slots";
}

QList<LineSegment*> HeightMapFormController::subdivideSegment(LineSegment* segment)
{
    /*
    QList<LineSegment*> list;

    QRectF borderRect = borderRectFromTextboxes();

    double interpolationStepX = borderRect.width() / (mUi->txtHeightMapInterpolationStepX->value() - 1);
    double interpolationStepY = borderRect.height() / (mUi->txtHeightMapInterpolationStepY->value() - 1);

    double length;

    QVector3D vec = segment->getEnd() - segment->getStart();

    if (qIsNaN(vec.length())) return QList<LineSegment*>();

    if (fabs(vec.x()) / fabs(vec.y()) < interpolationStepX / interpolationStepY) length = interpolationStepY / (vec.y() / vec.length());
    else length = interpolationStepX / (vec.x() / vec.length());

    length = fabs(length);

    if (qIsNaN(length)) {
        qDebug() << "ERROR length:" << segment->getStart() << segment->getEnd();
        return QList<LineSegment*>();
    }

    QVector3D seg = vec.normalized() * length;
    int count = trunc(vec.length() / length);

    if (count == 0) return QList<LineSegment*>();

    for (int i = 0; i < count; i++) {
        LineSegment* line = new LineSegment(segment);
        line->setStart(i == 0 ? segment->getStart() : list[i - 1]->getEnd());
        line->setEnd(line->getStart() + seg);
        list.append(line);
    }

    if (list.count() > 0 && list.last()->getEnd() != segment->getEnd()) {
        LineSegment* line = new LineSegment(segment);
        line->setStart(list.last()->getEnd());
        line->setEnd(segment->getEnd());
        list.append(line);
    }

    return list;
    */
}

QRectF HeightMapFormController::borderRectFromTextboxes()
{
    QRectF rect;

    rect.setX(mUi.txtHeightMapBorderX->value());
    rect.setY(mUi.txtHeightMapBorderY->value());
    rect.setWidth(mUi.txtHeightMapBorderWidth->value());
    rect.setHeight(mUi.txtHeightMapBorderHeight->value());

    return rect;
}

QRectF HeightMapFormController::borderRectFromExtremes()
{
    /*
    QRectF rect;

    rect.setX(mCodeDrawer.getMinimumExtremes().x());
    rect.setY(mCodeDrawer.getMinimumExtremes().y());
    rect.setWidth(mCodeDrawer.getSizes().x());
    rect.setHeight(mCodeDrawer.getSizes().y());

    return rect;
    */
}


void HeightMapFormController::resizeEvent(QResizeEvent *re)
{
    Q_UNUSED(re)
    resizeTableHeightMapSections();
}
