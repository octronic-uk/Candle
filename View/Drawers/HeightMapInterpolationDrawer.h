// This file is a part of "Cocoanut" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#ifndef HEIGHTMAPINTERPOLATIONDRAWER_H
#define HEIGHTMAPINTERPOLATIONDRAWER_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QColor>
#include "ShaderDrawable.h"
#include "Utils/Util.h"

class HeightMapInterpolationDrawer : public ShaderDrawable
{
public:
    explicit HeightMapInterpolationDrawer();

    QVector<QVector<double> > *data() const;
    void setData(QVector<QVector<double> > *data);

    QRectF borderRect() const;
    void setBorderRect(const QRectF &borderRect);

protected:
    bool updateData();

private:
    QRectF m_borderRect;
    double m_gridSize;
    QVector<QVector<double>> *m_data;
    double Min(double v1, double v2);
    double Max(double v1, double v2);
};

#endif // HEIGHTMAPINTERPOLATIONDRAWER_H
