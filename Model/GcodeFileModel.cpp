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
#include <QFile>
#include <QDir>
#include <QTime>

#include <QtDebug>
#include <QMessageBox> // TODO - Move these to view/controller
#include <QProgressDialog>

#include "GcodeFileModel.h"
#include "Model/Tables/GcodeTableModel.h"

GcodeFileModel::GcodeFileModel(QObject *parent)
    : QObject(parent),
      mProgramLoading(false),
      mFileChanged(false)
{
    //qDebug() << "GcodeFileModel: Constructing";
}

GcodeFileModel::~GcodeFileModel()
{
    //qDebug() << "GcodeFileModel: Destructing";
}

void GcodeFileModel::load(QList<QString> data)
{
    //qDebug() << "GcodeFileModel: load(Qlist<QString>)";
    QTime time;
    time.start();

    // Reset tables
    emit gcodeFileLoadStartedSignal();

    //clearTable();
    //mProbeModel.clear();
    //mProgramHeightmapModel.clear();
    //mCurrentModel = &mProgramModel;

    // Reset parsers
    //mViewParser.reset();
    //mProbeParser.reset();

    // Reset code drawer
    //mCurrentDrawer = mCodeDrawer;
    //mCodeDrawer->update();
    //mUi->glwVisualizer->fitDrawable(mCodeDrawer);
    //updateProgramEstimatedTime(QList<LineSegment*>());

    // Update interface
    //mUi->chkHeightMapUse->setChecked(false);
    //mUi->grpHeightMap->setProperty("overrided", false);
    //style()->unpolish(mUi->grpHeightMap);
    //mUi->grpHeightMap->ensurePolished();



    // Prepare parser
    mGcodeParser.setTraverseSpeed(1);//mSettingsForm->rapidSpeed());
    //if (mCodeDrawer->getIgnoreZ()) gp.reset(QVector3D(qQNaN(), qQNaN(), 0));

    //qDebug() << "GcodeFileModel: Prepared to load at time: " << time.elapsed();
    time.start();

    // Block parser updates on table changes
    mProgramLoading = true;

    // Prepare model
    //qDebug() << "GcodeFileModel: Clearing data";
    mData.clear();
    emit clearExistingGcodeFileSignal();
    emit reserveGcodeRowsSignal(data.count());
    //mProgramModel.data().reserve(data.count());

//    QProgressDialog progress(tr("Opening file..."), tr("Abort"), 0, data.count(), nullptr);//parent);
//    progress.setWindowModality(Qt::WindowModal);
//    progress.setFixedSize(progress.sizeHint());
//    if (data.count() > PROGRESS_MIN_LINES)
//    {
//        progress.show();
//       progress.setStyleSheet("QProgressBar {text-align: center; qproperty-format: \"\"}");
//    }

    while (!data.isEmpty())
    {
        //qDebug() << "GcodeFileModel: Next Line";
        QString command;
        QString cmd_no_comment;
        QString trimmed;
        QList<QString> args;
        GcodeItem item;

        command = data.takeFirst();

        // Trim command
        trimmed = command.trimmed();

        if (!trimmed.isEmpty())
        {
            // Split command
            QStringList cmdAndComment =GcodePreprocessorUtils::removeComment(command);
            cmd_no_comment = cmdAndComment.at(0);
            args = GcodePreprocessorUtils::splitCommand(cmd_no_comment);

            PointSegment *ps = mGcodeParser.addCommand(args);

            if  (
                ps && (qIsNaN(ps->point()->x()) ||
                qIsNaN(ps->point()->y()) ||
                qIsNaN(ps->point()->z()))
            ) {
                //qDebug() << "GcodeFileModel: nan point segment added:" << *ps->point();
            }

            item.setCommand(trimmed);
            item.setState(GcodeItemState::GCODE_ITEM_STATE_IN_QUEUE);
            item.setLine(mGcodeParser.getCommandNumber());
            item.setArgs(args);

            //emit nextGcodeLineReadySignal(item);
            //qDebug() << "GcodeFileModel: Appending to mData";
            mData.append(item);

        }

//        if (progress.isVisible() && (data.count() % PROGRESS_STEP == 0)) {
//            progress.setValue(progress.maximum() - data.count());
//            qApp->processEvents();
//            if (progress.wasCanceled()) break;
//        }
    }
    //progress.close();


    //qDebug() << "GcodeFileModel: model filled at time " << time.elapsed();
    time.start();

    //updateProgramEstimatedTime(mViewParser.getLinesFromParser(&gp, mSettingsForm->arcPrecision(), mSettingsForm->arcDegreeMode()));
    //qDebug() << "GcodeFileModel: view parser filled at time" << time.elapsed();

    mProgramLoading = false;
    emit gcodeFileLoadFinishedSignal(mData);
    emit gcodeParserUpdatedSignal(&mGcodeParser);

    //  Update code drawer
    //mCodeDrawer->update();
    //mUi->glwVisualizer->fitDrawable(mCodeDrawer);

    //resetHeightmap();
    //updateControlsState();
}

void GcodeFileModel::load(QString fileName)
{
    //qDebug() << "GcodeFileModel: load(QString fileName)";
    mFile.setFileName(fileName);

    if (!mFile.open(QIODevice::ReadOnly))
    {
        emit statusUpdateSignal(QString(tr("Can't open file") + fileName));
        return;
    }

    // Prepare text stream
    QTextStream textStream(&mFile);

    // Read lines
    QList<QString> data;
    while (!textStream.atEnd())
    {
        data.append(textStream.readLine());
    }

    mFile.close();

    // Load lines
    load(data);
    //qDebug() << "GcodeFileModel: Loaded Gcode File "
             //<< fileName;
}

QTime GcodeFileModel::updateProgramEstimatedTime(QList<LineSegment*> lines)
{

    Q_UNUSED(lines)
    //qDebug() << "GcodeFileModel: updateProgramEstimatedTime(QList<LineSegment*> lines)";
    /*
    double time = 0;

    for (int i = 0; i < lines.count(); i++) {
        LineSegment *ls = lines[i];
    //    foreach (LineSegment *ls, lines) {
        double length = (ls->getEnd() - ls->getStart()).length();

        if (!qIsNaN(length) && !qIsNaN(ls->getSpeed()) && ls->getSpeed() != 0) time +=
                length / ((mUi->chkFeedOverride->isChecked() && !ls->isFastTraverse())
                          ? (ls->getSpeed() * mUi->txtFeed->value() / 100) : ls->getSpeed());

//        //qDebug() << "GcodeFileModel: length/time:" << length << ((mUi->chkFeedOverride->isChecked() && !ls->isFastTraverse())
//                                                 ? (ls->getSpeed() * mUi->txtFeed->value() / 100) : ls->getSpeed())
//                 << time;

//        if (qIsNaN(length)) //qDebug() << "GcodeFileModel: length nan:" << i << ls->getLineNumber() << ls->getStart() << ls->getEnd();
//        if (qIsNaN(ls->getSpeed())) //qDebug() << "GcodeFileModel: speed nan:" << ls->getSpeed();
    }

    time *= 60;

    QTime t;

    t.setHMS(0, 0, 0);
    t = t.addSecs(time);

    mUi->glwVisualizer->setSpendTime(QTime(0, 0, 0));
    mUi->glwVisualizer->setEstimatedTime(t);

    return t;
    */
    return QTime();
}

QString GcodeFileModel::getCurrentFileName()
{
   //qDebug() << "GcodeFileModel: getCurrentFileName()";
   return mFile.fileName();
}


bool GcodeFileModel::save(QString fileName, GcodeTableModel *model)
{
    Q_UNUSED(fileName)
    Q_UNUSED(model)
   //qDebug() << "GcodeFileModel:save(QString, GcodeTableModel)";
    /*
    QFile file(fileName);
    QDir dir;

    //qDebug() << "GcodeFileModel: Saving program";

    if (file.exists()) dir.remove(file.fileName());
    if (!file.open(QIODevice::WriteOnly)) return false;

    QTextStream textStream(&file);

    for (int i = 0; i < model->rowCount() - 1; i++) {
        textStream << model->data(model->index(i, 1)).toString() << "\r\n";
    }

    file.close();

    return true;
    */
   return true;
}


bool GcodeFileModel::isGcodeFile(QString fileName)
{
    //qDebug() << "GcodeTableModel: isGcodeFile" << fileName;
    return fileName.endsWith(".txt", Qt::CaseInsensitive)
            || fileName.endsWith(".nc", Qt::CaseInsensitive)
            || fileName.endsWith(".ncc", Qt::CaseInsensitive)
            || fileName.endsWith(".ngc", Qt::CaseInsensitive)
            || fileName.endsWith(".tap", Qt::CaseInsensitive);
}


bool GcodeFileModel::hasFileChanged()
{
    //qDebug() << "GcodeTableModel: hasFileChanged";
    return mFileChanged;
}

