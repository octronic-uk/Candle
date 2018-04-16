// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QVector3D>
#include <QTimer>
#include <QColor>
#include <cmath>
#include "ShaderDrawable.h"

class Tool;
class SqlSettingsModel;

class ToolDrawer : public ShaderDrawable
{
public:
    explicit ToolDrawer();
    virtual ~ToolDrawer() override;

    float toolDiameter() const;
    void setToolDiameter(float toolDiameter);

    float toolLength() const;
    void setToolLength(float toolLength);

    QVector3D toolPosition() const;
    void setToolPosition(const QVector3D &toolPosition);

    float rotationAngle() const;
    void setRotationAngle(float rotationAngle);
    void rotate(float angle);

    float toolAngle() const;
    void setToolAngle(float toolAngle);

    QColor color() const;
    void setColor(const QColor &color);

    Tool* getToolHandle() const;
    void setToolHandle(Tool* toolHandle);

    SqlSettingsModel* getSettingsModelHandle() const;
    void setSettingsModelHandle(SqlSettingsModel* settingsModelHandle);

protected:
    void doItTheOldWay();
    void doItTheNewWay();
    QVector<VertexData> createCircle(QVector3D center, float radius, int arcs, QVector4D color);
    float normalizeAngle(float angle);
    bool updateData() override;

private:
    float mToolDiameter;
    float mToolLength;
    float mEndLength;
    QVector3D mToolPosition;
    float mRotationAngle;
    float mToolAngle;
    QColor mColor;
    SqlSettingsModel* mSettingsModelHandle;
    Tool* mToolHandle;

    void generateToolGeometry();
    void generateToolHolderGeometry();
};
