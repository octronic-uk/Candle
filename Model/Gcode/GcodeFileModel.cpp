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

#include <QtDebug>

#include "GcodeFileModel.h"
#include "Model/Gcode/GcodeTableModel.h"
#include "Utils/GcodeCommandNotFoundException.h"

GcodeFileModel::GcodeFileModel(QObject *parent)
    : QObject(parent),
      mProgramLoading(false),
      mFileChanged(false),
      mFileOpen(false)
{
    qDebug() << "GcodeFileModel: Constructing";
    mGcodeParser = QSharedPointer<GcodeParser>::create(this);
}

GcodeFileModel::~GcodeFileModel()
{
    qDebug() << "GcodeFileModel: Destructing";
    for (GcodeCommand* item : mData)
    {
        delete item;
    }
    mData.clear();
    mProgramLoading = false;
    mFileChanged = false;
}

void GcodeFileModel::load(QList<QString> data)
{
    qDebug() << "GcodeFileModel: load(Qlist<QString>)";

    // Reset tables
    emit gcodeFileLoadStartedSignal();
    // Prepare parser
    mGcodeParser->setTraverseSpeed(1);//mSettingsForm->rapidSpeed());
    //if (mCodeDrawer->getIgnoreZ()) gp.reset(QVector3D(qQNaN(), qQNaN(), 0));

    // Block parser updates on table changes
    mProgramLoading = true;

    // Prepare model
    //qDebug() << "GcodeFileModel: Clearing data";
    mData.clear();
    emit clearExistingGcodeFileSignal();
    emit reserveGcodeRowsSignal(data.count());


    // TODO - This should probably be moved to GcodeParser

    int index = 0;
    while (!data.isEmpty())
    {
        QString command;
        QString trimmed;
        QList<QString> args;
        GcodeCommand* item = new GcodeCommand();
        command = data.takeFirst();
        trimmed = command.trimmed();
        item->setCommand(command);

        //qDebug() << "GcodeFileModel: Next Line" << command;
        if (!trimmed.isEmpty())
        {
            item->setLine(mGcodeParser->getCommandNumber());
            item->setTableIndex(index);

            mGcodeParser->addCommand(item);

            if (item->getCommand() == "%")
            {
                qDebug() << "GcodeFileModel: Skipping % at index " << index;
                continue;
            }

            if (item->getArgs().isEmpty())
            {
                QString marker = GcodeParser::parseComment(command);
                qDebug() << "GcodeFileModel: marker " << marker;
                item->setMarker(marker);
                item->setState(GcodeCommandState::Marker);
                if (!item->getMarker().isEmpty())
                {
                    mMarkers.append(item);
                }
            }
            else
            {
                item->setCommand(trimmed + '\r');
                item->setState(GcodeCommandState::InQueue);
            }

            mData.append(item);
            index++;
        }
    }
    mProgramLoading = false;
    printMarkers();
    emit gcodeFileLoadFinishedSignal(this);
    emit gcodeParserUpdatedSignal(mGcodeParser.data());
    mFileOpen = true;
}

void GcodeFileModel::load(QString fileName)
{
    qDebug() << "GcodeFileModel: load(QString fileName)";
    mFile.setFileName(fileName);

    if (!mFile.open(QIODevice::ReadOnly))
    {
        emit statusBarUpdateSignal(QString(tr("Can't open file") + fileName));
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
    load(data);
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
   qDebug() << "GcodeFileModel: getCurrentFileName()";
   return mFile.fileName();
}

GcodeCommand* GcodeFileModel::getCommandByID(long id) const
{
   for (GcodeCommand* next : mData)
   {
       if (next->hasID(id))
       {
           return next;
       }
   }
   throw GcodeCommandNotFoundException(id);
}

GcodeCommand* GcodeFileModel::getCommand(int index) const
{
    if (index < 0 || index > mData.count() -1)
    {
        throw GcodeCommandNotFoundException(index);
    }

    return mData.at(index);
}

int GcodeFileModel::countCommands()
{
    return mData.count();
}

QList<GcodeCommand*> GcodeFileModel::getData() const
{
    return mData;
}

QList<GcodeCommand*> GcodeFileModel::getMarkers() const
{
    return mMarkers;
}

bool GcodeFileModel::isOpen()
{
   return mFileOpen;
}

void GcodeFileModel::printMarkers()
{
   qDebug() << "GcodeFileModel: Markers";
   for (GcodeCommand* marker : mMarkers)
   {
       qDebug() << "GcodeFileModel: " << marker->getTableIndex()
                << ":" << marker->getMarker();
   }
}


bool GcodeFileModel::save(QString fileName, GcodeTableModel *model)
{
    Q_UNUSED(fileName)
    Q_UNUSED(model)
    qDebug() << "GcodeFileModel:save(QString, GcodeTableModel)";

    QFile file(fileName);
    QDir dir;

    qDebug() << "GcodeFileModel: Saving program";

    if (file.exists())
    {
        dir.remove(file.fileName());
    }

    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QTextStream textStream(&file);

    for (int i = 0; i < model->rowCount() - 1; i++)
    {
        textStream << model->data(model->index(i, 1)).toString() << "\r\n";
    }

    file.close();
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
    qDebug() << "GcodeTableModel: hasFileChanged";
    return mFileChanged;
}

