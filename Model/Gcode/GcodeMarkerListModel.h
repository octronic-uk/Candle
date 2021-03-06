/*
 * GcodeMarkerListModel.h
 *
 * Created: 14 2018 by Ashley
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

#include <QAbstractListModel>

class GcodeCommand;

class GcodeMarkerListModel : public QAbstractListModel
{
public:
    GcodeMarkerListModel(QObject* parent = nullptr);
    ~GcodeMarkerListModel() override;

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    void setMarkers(QList<GcodeCommand*> data);
    GcodeCommand* at(int);
    void clear();

private:
    QList<GcodeCommand*> mData;
    QString removeCommentChars(QString marker) const;
};
