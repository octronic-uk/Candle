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
#include <QSharedPointer>

class Profile;
class Tool;

class ToolListModel : public QAbstractListModel
{
    Q_OBJECT

    enum DataIndex {
        TOOL_NAME_INDEX = 0,
        TOOL_HOLDER_ID_INDEX = 3,
        TOOL_NUMBER_INDEX = 4,
    };

public:
    explicit ToolListModel(Profile* profile, QObject *parent = nullptr);
    void initialise(QList<QSharedPointer<Tool>> data);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Tool* getData(int);
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QList<QSharedPointer<Tool>>& getAllData();
    void clear();

    void insertItem(QSharedPointer<Tool> newItem);
    void deleteItem(Tool* item);

    QModelIndex indexOf(Tool* holder);
    QModelIndex getSelectedToolIndex();
    QModelIndex getToolIndex(Tool* holder);
    QModelIndex getToolNumberIndex(Tool* holder);

    Tool* getSelected() const;
    Tool* getToolByNumber(int num);
    void setSelectedToolHandle(Tool* selectedToolHandle);

signals:

private:
    QList<QSharedPointer<Tool>> mData;
    Profile* mProfileHandle;
    Tool* mSelectedToolHandle;
};
