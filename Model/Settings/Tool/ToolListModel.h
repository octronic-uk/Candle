/*
 * ToolModelListModel.h
 *
 * Created: 06 2018 by Ashley
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
#include "Model/Settings/Tool/Tool.h"
#include <QSharedPointer>

class ToolListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ToolListModel(QObject *parent = nullptr);
    void initialise(QList<QSharedPointer<Tool>> data);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void insert(QSharedPointer<Tool> newItem);
    Tool* getData(int);
    void remove(Tool* item);
    QList<QSharedPointer<Tool>>& getAllData();
    void clear();
    QModelIndex indexOf(Tool* holder);

signals:
    void toolCreatedSignal(Tool* tool);
    void toolUpdatedSignal(Tool* tool);
    void toolDeletedSignal(Tool* tool);

private:
    QList<QSharedPointer<Tool>> mData;

    // QAbstractItemModel interface
public:
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
};
