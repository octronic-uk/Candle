/*
 * HeightMapFileModel.cpp
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
#include "HeightMapFileModel.h"
#include <QtDebug>

HeightMapFileModel::HeightMapFileModel(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Constructing HeightMapFileModel";
}

HeightMapFileModel::~HeightMapFileModel()
{
    qDebug() << "Destructing HeightMapFileModel";
}

bool HeightMapFileModel::isHeightMapFile(QString fileName)
{
    return fileName.endsWith(".map", Qt::CaseInsensitive);
}

bool HeightMapFileModel::hasFileChanged()
{
    return mHeightMapChanged;
}

void HeightMapFileModel::setFileChanged(bool changed)
{
   mHeightMapChanged = changed;
}

bool HeightMapFileModel::save(QString fileName)
{
    /*
    QFile file(fileName);
    QDir dir;

    if (file.exists()) dir.remove(file.fileName());
    if (!file.open(QIODevice::WriteOnly)) return false;

    QTextStream textStream(&file);
    textStream << mUi->txtHeightMapBorderX->text() << ";"
               << mUi->txtHeightMapBorderY->text() << ";"
               << mUi->txtHeightMapBorderWidth->text() << ";"
               << mUi->txtHeightMapBorderHeight->text() << "\r\n";
    textStream << mUi->txtHeightMapGridX->text() << ";"
               << mUi->txtHeightMapGridY->text() << ";"
               << mUi->txtHeightMapGridZBottom->text() << ";"
               << mUi->txtHeightMapGridZTop->text() << "\r\n";
    textStream << mUi->cboHeightMapInterpolationType->currentIndex() << ";"
               << mUi->txtHeightMapInterpolationStepX->text() << ";"
                << mUi->txtHeightMapInterpolationStepY->text() << "\r\n";

    for (int i = 0; i < m_heightMapModel.rowCount(); i++) {
        for (int j = 0; j < m_heightMapModel.columnCount(); j++) {
            textStream << m_heightMapModel.data(m_heightMapModel.index(i, j), Qt::UserRole).toString() << ((j == m_heightMapModel.columnCount() - 1) ? "" : ";");
        }
        textStream << "\r\n";
    }

    file.close();

    m_heightMapChanged = false;

    return true;
    */
}

void HeightMapFileModel::load(QString fileName)
{
    emit heightMapFileLoadStartedSignal();

    mFile.setFileName(fileName);
    if (!mFile.open(QIODevice::ReadOnly))
    {
        emit statusUpdateSignal(QString(tr("Can't open file:\n") + fileName));
        return;
    }
    QTextStream textStream(&mFile);
    /*

    m_settingsLoading = true;

    // Storing previous values
    mUi->txtHeightMapBorderX->setValue(qQNaN());
    mUi->txtHeightMapBorderY->setValue(qQNaN());
    mUi->txtHeightMapBorderWidth->setValue(qQNaN());
    mUi->txtHeightMapBorderHeight->setValue(qQNaN());

    mUi->txtHeightMapGridX->setValue(qQNaN());
    mUi->txtHeightMapGridY->setValue(qQNaN());
    mUi->txtHeightMapGridZBottom->setValue(qQNaN());
    mUi->txtHeightMapGridZTop->setValue(qQNaN());

    QList<QString> list = textStream.readLine().split(";");
    mUi->txtHeightMapBorderX->setValue(list[0].toDouble());
    mUi->txtHeightMapBorderY->setValue(list[1].toDouble());
    mUi->txtHeightMapBorderWidth->setValue(list[2].toDouble());
    mUi->txtHeightMapBorderHeight->setValue(list[3].toDouble());

    list = textStream.readLine().split(";");
    mUi->txtHeightMapGridX->setValue(list[0].toDouble());
    mUi->txtHeightMapGridY->setValue(list[1].toDouble());
    mUi->txtHeightMapGridZBottom->setValue(list[2].toDouble());
    mUi->txtHeightMapGridZTop->setValue(list[3].toDouble());

    m_settingsLoading = false;

    updateHeightMapBorderDrawer();

    m_heightMapModel.clear();   // To avoid probe data wipe message
    updateHeightMapGrid();

    list = textStream.readLine().split(";");

//    mUi->chkHeightMapBorderAuto->setChecked(false);
//    mUi->chkHeightMapBorderAuto->setEnabled(false);
//    mUi->txtHeightMapBorderX->setEnabled(false);
//    mUi->txtHeightMapBorderY->setEnabled(false);
//    mUi->txtHeightMapBorderWidth->setEnabled(false);
//    mUi->txtHeightMapBorderHeight->setEnabled(false);

//    mUi->txtHeightMapGridX->setEnabled(false);
//    mUi->txtHeightMapGridY->setEnabled(false);
//    mUi->txtHeightMapGridZBottom->setEnabled(false);
//    mUi->txtHeightMapGridZTop->setEnabled(false);

    for (int i = 0; i < m_heightMapModel.rowCount(); i++) {
        QList<QString> row = textStream.readLine().split(";");
        for (int j = 0; j < m_heightMapModel.columnCount(); j++) {
            m_heightMapModel.setData(m_heightMapModel.index(i, j), row[j].toDouble(), Qt::UserRole);
        }
    }

    file.close();

    mUi->txtHeightMap->setText(fileName.mid(fileName.lastIndexOf("/") + 1));
    mHeightMapFileName = fileName;
    m_heightMapChanged = false;

    mUi->cboHeightMapInterpolationType->setCurrentIndex(list[0].toInt());
    mUi->txtHeightMapInterpolationStepX->setValue(list[1].toDouble());
    mUi->txtHeightMapInterpolationStepY->setValue(list[2].toDouble());

    updateHeightMapInterpolationDrawer();
    */
    qDebug() << "HeightMapFileModel: Loaded %s",fileName.toStdString().c_str();
    emit heightMapFileLoadFinishedSignal();
}

QString HeightMapFileModel::getCurrentFileName()
{
   return mFile.fileName();
}

