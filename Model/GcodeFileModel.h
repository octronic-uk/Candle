/*
 * FileModel.h
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

#pragma once

#include <QObject>
#include <QFile>
#include <QTime>

#include "Model/Tables/GcodeTableModel.h"
#include "Parser/GcodeParser.h"

class LineSegment;

class GcodeFileModel : public QObject
{
    Q_OBJECT
public:
    GcodeFileModel(QObject *parent = nullptr);
    ~GcodeFileModel();

    void load(QList<QString> data);
    void load(QString fileName);
    bool save(QString fileName, GcodeTableModel *model);
    bool isGcodeFile(QString fileName);
    bool hasFileChanged();
    void setFileChanged(bool changed);
    QTime updateProgramEstimatedTime(QList<LineSegment*> lines);
    QString getCurrentFileName();
    GcodeCommand getCommand(int index);
    int countCommands();

signals:
    void statusBarUpdateSignal(QString);
    void gcodeFileLoadStartedSignal();
    void gcodeFileLoadFinishedSignal(QList<GcodeCommand>);
    void nextGcodeLineReadySignal(GcodeCommand);
    void clearExistingGcodeFileSignal();
    void reserveGcodeRowsSignal(int count);
    void gcodeParserUpdatedSignal(GcodeParser*);

private:
    bool mProgramLoading;
    bool mFileChanged;
    QFile mFile;
    QList<GcodeCommand> mData;
    GcodeParser mGcodeParser;
};
