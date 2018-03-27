/*
 * FileModel.cpp
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
#include <QTime>
#include <QFile>
#include <QDir>

#include <QMessageBox> // TODO - Move these to view/controller
#include <QProgressDialog>

#include "GCodeFileModel.h"
#include "Parser/GCodeParser.h"
#include "Model/Tables/GCodeTableModel.h"

GCodeFileModel::GCodeFileModel(QObject *parent)
    : QObject(parent)
{

}

GCodeFileModel::~GCodeFileModel()
{

}

void GCodeFileModel::load(QList<QString> data)
{
    /*
    QTime time;
    time.start();

    // Reset tables
    clearTable();
    mProbeModel.clear();
    mProgramHeightmapModel.clear();
    mCurrentModel = &mProgramModel;

    // Reset parsers
    mViewParser.reset();
    mProbeParser.reset();

    // Reset code drawer
    mCurrentDrawer = mCodeDrawer;
    mCodeDrawer->update();
    mUi->glwVisualizer->fitDrawable(mCodeDrawer);
    updateProgramEstimatedTime(QList<LineSegment*>());

    // Update interface
    mUi->chkHeightMapUse->setChecked(false);
    mUi->grpHeightMap->setProperty("overrided", false);
    style()->unpolish(mUi->grpHeightMap);
    mUi->grpHeightMap->ensurePolished();

    // Reset tableview
    QByteArray headerState = mUi->tblProgram->horizontalHeader()->saveState();
    mUi->tblProgram->setModel(NULL);

    // Prepare parser
    GcodeParser gp;
    gp.setTraverseSpeed(mSettingsForm->rapidSpeed());
    if (mCodeDrawer->getIgnoreZ()) gp.reset(QVector3D(qQNaN(), qQNaN(), 0));

    qDebug() << "Prepared to load:" << time.elapsed();
    time.start();

    // Block parser updates on table changes
    mProgramLoading = true;

    QString command;
    QString stripped;
    QString trimmed;
    QList<QString> args;
    GCodeItem item;

    // Prepare model
    mProgramModel.data().clear();
    mProgramModel.data().reserve(data.count());

    QProgressDialog progress(tr("Opening file..."), tr("Abort"), 0, data.count(), this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setFixedSize(progress.sizeHint());
    if (data.count() > PROGRESS_MIN_LINES) {
        progress.show();
        progress.setStyleSheet("QProgressBar {text-align: center; qproperty-format: \"\"}");
    }

    while (!data.isEmpty())
    {
        command = data.takeFirst();

        // Trim command
        trimmed = command.trimmed();

        if (!trimmed.isEmpty()) {
            // Split command
            stripped = GcodePreprocessorUtils::removeComment(command);
            args = GcodePreprocessorUtils::splitCommand(stripped);

            PointSegment *ps = gp.addCommand(args);

    //        if (ps && (qIsNaN(ps->point()->x()) || qIsNaN(ps->point()->y()) || qIsNaN(ps->point()->z())))
    //                   qDebug() << "nan point segment added:" << *ps->point();

            item.command = trimmed;
            item.state = GCodeItem::InQueue;
            item.line = gp.getCommandNumber();
            item.args = args;

            mProgramModel.data().append(item);
        }

        if (progress.isVisible() && (data.count() % PROGRESS_STEP == 0)) {
            progress.setValue(progress.maximum() - data.count());
            qApp->processEvents();
            if (progress.wasCanceled()) break;
        }
    }
    progress.close();

    mProgramModel.insertRow(mProgramModel.rowCount());

    qDebug() << "model filled:" << time.elapsed();
    time.start();

    updateProgramEstimatedTime(mViewParser.getLinesFromParser(&gp, mSettingsForm->arcPrecision(), mSettingsForm->arcDegreeMode()));
    qDebug() << "view parser filled:" << time.elapsed();

    mProgramLoading = false;

    // Set table model
    mUi->tblProgram->setModel(&mProgramModel);
    mUi->tblProgram->horizontalHeader()->restoreState(headerState);

    // Update tableview
    connect(mUi->tblProgram->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableCurrentChanged(QModelIndex,QModelIndex)));
    mUi->tblProgram->selectRow(0);

    //  Update code drawer
    mCodeDrawer->update();
    mUi->glwVisualizer->fitDrawable(mCodeDrawer);

    resetHeightmap();
    updateControlsState();
    */
}

void GCodeFileModel::load(QString fileName)
{
    /*
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, this->windowTitle(), tr("Can't open file:\n") + fileName);
        return;
    }

    // Set filename
    mProgramFileName = fileName;

    // Prepare text stream
    QTextStream textStream(&file);

    // Read lines
    QList<QString> data;
    while (!textStream.atEnd()) data.append(textStream.readLine());

    // Load lines
    loadFile(data);
    */
}

QTime GCodeFileModel::updateProgramEstimatedTime(QList<LineSegment*> lines)
{
    /*
    double time = 0;

    for (int i = 0; i < lines.count(); i++) {
        LineSegment *ls = lines[i];
    //    foreach (LineSegment *ls, lines) {
        double length = (ls->getEnd() - ls->getStart()).length();

        if (!qIsNaN(length) && !qIsNaN(ls->getSpeed()) && ls->getSpeed() != 0) time +=
                length / ((mUi->chkFeedOverride->isChecked() && !ls->isFastTraverse())
                          ? (ls->getSpeed() * mUi->txtFeed->value() / 100) : ls->getSpeed());

//        qDebug() << "length/time:" << length << ((mUi->chkFeedOverride->isChecked() && !ls->isFastTraverse())
//                                                 ? (ls->getSpeed() * mUi->txtFeed->value() / 100) : ls->getSpeed())
//                 << time;

//        if (qIsNaN(length)) qDebug() << "length nan:" << i << ls->getLineNumber() << ls->getStart() << ls->getEnd();
//        if (qIsNaN(ls->getSpeed())) qDebug() << "speed nan:" << ls->getSpeed();
    }

    time *= 60;

    QTime t;

    t.setHMS(0, 0, 0);
    t = t.addSecs(time);

    mUi->glwVisualizer->setSpendTime(QTime(0, 0, 0));
    mUi->glwVisualizer->setEstimatedTime(t);

    return t;
    */
}


bool GCodeFileModel::save(QString fileName, GCodeTableModel *model)
{
    /*
    QFile file(fileName);
    QDir dir;

    qDebug() << "Saving program";

    if (file.exists()) dir.remove(file.fileName());
    if (!file.open(QIODevice::WriteOnly)) return false;

    QTextStream textStream(&file);

    for (int i = 0; i < model->rowCount() - 1; i++) {
        textStream << model->data(model->index(i, 1)).toString() << "\r\n";
    }

    file.close();

    return true;
    */
}


bool GCodeFileModel::isGCodeFile(QString fileName)
{
    /*
    return fileName.endsWith(".txt", Qt::CaseInsensitive)
            || fileName.endsWith(".nc", Qt::CaseInsensitive)
            || fileName.endsWith(".ncc", Qt::CaseInsensitive)
            || fileName.endsWith(".ngc", Qt::CaseInsensitive)
            || fileName.endsWith(".tap", Qt::CaseInsensitive);
            */
    return false;
}


bool GCodeFileModel::hasFileChanged()
{
    return false;
}

