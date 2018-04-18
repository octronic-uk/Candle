/*
 * GrblConfigurationModel.h
 *
 * Created: 18 2018 by Ashley
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

#include <QAbstractTableModel>
#include <QMap>
#include <QList>

class GrblConfigurationModel : public QAbstractTableModel
{
public:
    GrblConfigurationModel(QObject* parent = nullptr);
    ~GrblConfigurationModel() override;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QMap<int,QString> getData();

public slots:
    void onFirmwareConfigurationRead(int param, QString value);
private:
    void setupHeader();
    void setupFields();

private:
    QList<QString> mHeaders;
    QMap<int,QString> mFields;
    QMap<int,QString> mData;

};
