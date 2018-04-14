// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "ToolDrawer.h"

ToolDrawer::ToolDrawer()
{
    qDebug() << "ToolDrawer: Constructing";
    mToolDiameter = 3.0f;
    mToolLength = 15.0f;
    mToolPosition = QVector3D(0, 0, 0);
    mRotationAngle = 0.0f;
    setColor(QColor("DarkGray"));
    mLineWidth = 3.0f;
}

ToolDrawer::~ToolDrawer()
{
    qDebug() << "ToolDrawer: Destructing";
}

bool ToolDrawer::updateData()
{
    const int arcs = 8;

    // Clear data
    mLines.clear();
    mPoints.clear();

    // Prepare vertex
    VertexData vertex;
    vertex.color = Util::colorToVector(mColor);
    vertex.start = QVector3D(sNan, sNan, sNan);

    // Draw lines
    for (int i = 0; i < arcs; i++)
    {
        float x = mToolPosition.x() + mToolDiameter / 2.0f * cos(mRotationAngle / 180.0f * M_PI + (2.0f * M_PI / arcs) * i);
        float y = mToolPosition.y() + mToolDiameter / 2.0f * sin(mRotationAngle / 180.0f * M_PI + (2.0f * M_PI / arcs) * i);

        // Side lines
        vertex.position = QVector3D(x, y, mToolPosition.z() + mEndLength);
        mLines.append(vertex);
        vertex.position = QVector3D(x, y, mToolPosition.z() + mToolLength);
        mLines.append(vertex);

        // Bottom lines
        vertex.position = QVector3D(
            mToolPosition.x(), mToolPosition.y(), mToolPosition.z()
        );

        mLines.append(vertex);
        vertex.position = QVector3D(x, y, mToolPosition.z() + mEndLength);
        mLines.append(vertex);

        // Top lines
        vertex.position = QVector3D(
            mToolPosition.x(),
            mToolPosition.y(),
            mToolPosition.z() + mToolLength
        );

        mLines.append(vertex);
        vertex.position = QVector3D(x, y, mToolPosition.z() + mToolLength);
        mLines.append(vertex);

        // Zero Z lines
        vertex.position = QVector3D(mToolPosition.x(), mToolPosition.y(), 0);
        mLines.append(vertex);
        vertex.position = QVector3D(x, y, 0);
        mLines.append(vertex);
    }

    // Draw circles
    // Bottom
    mLines += createCircle(
        QVector3D(
            mToolPosition.x(), mToolPosition.y(),
            mToolPosition.z() + mEndLength
        ),
        mToolDiameter / 2, 20, vertex.color
    );

    // Top
    mLines += createCircle(
        QVector3D(
            mToolPosition.x(), mToolPosition.y(),
            mToolPosition.z() + mToolLength
        ),
        mToolDiameter / 2, 20, vertex.color
    );

    // Zero Z circle
    if (mEndLength == 0)
    {
        mLines += createCircle(
            QVector3D(mToolPosition.x(), mToolPosition.y(), 0),
            mToolDiameter / 2, 20, vertex.color
        );
    }

    return true;
}

QColor ToolDrawer::color() const
{
    return mColor;
}

void ToolDrawer::setColor(const QColor &color)
{
    mColor = color;
}


QVector<VertexData> ToolDrawer::createCircle(QVector3D center, float radius, int arcs, QVector3D color)
{
    // Vertices
    QVector<VertexData> circle;

    // Prepare vertex
    VertexData vertex;
    vertex.color = color;
    vertex.start = QVector3D(sNan, sNan, sNan);

    // Create line loop
    for (int i = 0; i <= arcs; i++)
    {
        float angle = 2 * M_PI * i / arcs;
        float x = center.x() + radius * cos(angle);
        float y = center.y() + radius * sin(angle);

        if (i > 1)
        {
            circle.append(circle.last());
        }
        else if (i == arcs)
        {
            circle.append(circle.first());
        }

        vertex.position = QVector3D(x, y, center.z());
        circle.append(vertex);
    }

    return circle;
}

float ToolDrawer::toolDiameter() const
{
    return mToolDiameter;
}

void ToolDrawer::setToolDiameter(float toolDiameter)
{
    if (mToolDiameter != toolDiameter)
    {
        mToolDiameter = toolDiameter;
        update();
    }
}

float ToolDrawer::toolLength() const
{
    return mToolLength;
}

void ToolDrawer::setToolLength(float toolLength)
{
    if (mToolLength != toolLength)
    {
        mToolLength = toolLength;
        update();
    }
}
QVector3D ToolDrawer::toolPosition() const
{
    return mToolPosition;
}

void ToolDrawer::setToolPosition(const QVector3D &toolPosition)
{
    if (mToolPosition != toolPosition)
    {
        mToolPosition = toolPosition;
        update();
    }
}

float ToolDrawer::rotationAngle() const
{
    return mRotationAngle;
}

void ToolDrawer::setRotationAngle(float rotationAngle)
{
    if (mRotationAngle != rotationAngle)
    {
        mRotationAngle = rotationAngle;
        update();
    }
}

void ToolDrawer::rotate(float angle)
{
    setRotationAngle(normalizeAngle(mRotationAngle + angle));
}

float ToolDrawer::toolAngle() const
{
    return mToolAngle;
}

void ToolDrawer::setToolAngle(float toolAngle)
{
    if (mToolAngle != toolAngle)
    {
        mToolAngle = toolAngle;
        mEndLength = mToolAngle > 0 && mToolAngle < 180 ? mToolDiameter / 2 / tan(mToolAngle / 180 * M_PI / 2) : 0;
        if (mToolLength < mEndLength)
        {
            mToolLength = mEndLength;
        }
        update();
    }
}

float ToolDrawer::normalizeAngle(float angle)
{
    while (angle < 0)
    {
        angle += 360;
    }

    while (angle > 360)
    {
        angle -= 360;
    }

    return angle;
}
