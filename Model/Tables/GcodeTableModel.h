// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QAbstractTableModel>
#include <QString>
#include "../GcodeCommand.h"

class GcodeTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit GcodeTableModel(QObject *parent = nullptr);
    ~GcodeTableModel() override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QList<GcodeCommand>& data();
    void setCommandData(QList<GcodeCommand> &items);
    void clear();
signals:

public slots:

private:
    QList<GcodeCommand> mData;
    QStringList mHeaders;
};
