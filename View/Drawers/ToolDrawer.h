// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#ifndef TOOLDRAWER_H
#define TOOLDRAWER_H

#include <QVector3D>
#include <QTimer>
#include <QColor>
#include <cmath>
#include "ShaderDrawable.h"

class ToolDrawer : public ShaderDrawable
{
public:
    explicit ToolDrawer();
    virtual ~ToolDrawer();

    double toolDiameter() const;
    void setToolDiameter(double toolDiameter);

    double toolLength() const;
    void setToolLength(double toolLength);

    QVector3D toolPosition() const;
    void setToolPosition(const QVector3D &toolPosition);

    double rotationAngle() const;
    void setRotationAngle(double rotationAngle);
    void rotate(double angle);

    double toolAngle() const;
    void setToolAngle(double toolAngle);

    QColor color() const;
    void setColor(const QColor &color);

protected:
    QVector<VertexData> createCircle(QVector3D center, double radius, int arcs, QVector4D color);
    double normalizeAngle(double angle);
    bool updateData() override;

private:
    double mToolDiameter;
    double mToolLength;
    double mEndLength;
    QVector3D mToolPosition;
    double mRotationAngle;
    double mToolAngle;
    QColor mColor;

};

#endif // TOOLDRAWER_H
