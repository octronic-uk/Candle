/*
 * GcodeMarkerListModel.cpp
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

#include "GcodeMarkerListModel.h"
#include "Model/Gcode/GcodeCommand.h"

GcodeMarkerListModel::GcodeMarkerListModel(QObject* parent)
    : QAbstractListModel(parent)
{

}

GcodeMarkerListModel::~GcodeMarkerListModel()
{

}

int GcodeMarkerListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return mData.count();
}

QVariant GcodeMarkerListModel::data(const QModelIndex& index, int role) const
{
   if (role == Qt::DisplayRole)
   {
       return removeCommentChars(mData.at(index.row())->getMarker());
   }
   return QVariant();
}

QString GcodeMarkerListModel::removeCommentChars(QString marker) const
{
   return marker.remove(")").remove("(").remove(";");
}

void GcodeMarkerListModel::setMarkers(QList<GcodeCommand*> data)
{
   mData = data;
   // TODO - Leak Spin
   GcodeCommand *start, *end;
   start = new GcodeCommand("",0);
   start->setMarker("Program Start");
   start->setState(GcodeCommandState::Marker);
   end = new GcodeCommand("",INT_MAX);
   end->setMarker("Program End");
   end->setState(GcodeCommandState::Marker);

   mData.push_front(start);
   mData.push_back(end);
   emit dataChanged(QModelIndex(),QModelIndex());
}

GcodeCommand* GcodeMarkerListModel::at(int index)
{
    return mData.at(index);
}

void GcodeMarkerListModel::clear()
{
   mData.clear();
   emit dataChanged(QModelIndex(),QModelIndex());
}
