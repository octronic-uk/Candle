/*
 * ToolModelTableModel.h
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
#include <QAbstractTableModel>
#include <QStringList>
#include <QSharedPointer>
#include "ToolGeometry.h"

class Tool;

class ToolGeometryTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ToolGeometryTableModel(Tool* parentTool, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int parent_id, int row, int count, const QModelIndex& parent);
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    ToolGeometry* getToolGeometryHandleAtRow(int row);

    ToolGeometry* insertNew();
    void insertItem(QSharedPointer<ToolGeometry> item);
    void deleteItem(ToolGeometry* geom);

    void removeSelectedGeometryRow();
    void setSelectedGeometryRow(int row);
    ToolGeometry*getSelectedGeometryHandle();
    QList<ToolGeometry*> getDataHandles();

signals:
    void geometryUpdatedSignal(ToolGeometry*);

private:
    QStringList mTableHeaders;
    ToolGeometry* mSelectedGeometryHandle;
    Tool* mParentHandle;
    QList<QSharedPointer<ToolGeometry>> mData;
};
