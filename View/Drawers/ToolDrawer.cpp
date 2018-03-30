// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "ToolDrawer.h"

ToolDrawer::ToolDrawer()
{
    qDebug() << "ToolDrawer: Constructing";
    mToolDiameter = 3;
    mToolLength = 15;
    mToolPosition = QVector3D(0, 0, 0);
    mRotationAngle = 0;
    setColor(QColor("Orange"));
    m_lineWidth = 3;
}

ToolDrawer::~ToolDrawer()
{
    qDebug() << "ToolDrawer: Destructing";
}

bool ToolDrawer::updateData()
{
    const int arcs = 4;

    // Clear data
    m_lines.clear();
    m_points.clear();

    // Prepare vertex
    VertexData vertex;
    vertex.color = Util::colorToVector(mColor);
    vertex.start = QVector3D(sNan, sNan, sNan);

    // Draw lines
    for (int i = 0; i < arcs; i++)
    {
        double x = mToolPosition.x() + mToolDiameter / 2 *
                cos(mRotationAngle / 180 * M_PI + (2 * M_PI / arcs) * i);
        double y = mToolPosition.y() + mToolDiameter / 2 *
                sin(mRotationAngle / 180 * M_PI + (2 * M_PI / arcs) * i);

        // Side lines
        vertex.position = QVector3D(x, y, mToolPosition.z() + mEndLength);
        m_lines.append(vertex);
        vertex.position = QVector3D(x, y, mToolPosition.z() + mToolLength);
        m_lines.append(vertex);

        // Bottom lines
        vertex.position = QVector3D(
            mToolPosition.x(), mToolPosition.y(), mToolPosition.z()
        );

        m_lines.append(vertex);
        vertex.position = QVector3D(x, y, mToolPosition.z() + mEndLength);
        m_lines.append(vertex);

        // Top lines
        vertex.position = QVector3D(
            mToolPosition.x(),
            mToolPosition.y(),
            mToolPosition.z() + mToolLength
        );

        m_lines.append(vertex);
        vertex.position = QVector3D(x, y, mToolPosition.z() + mToolLength);
        m_lines.append(vertex);

        // Zero Z lines
        vertex.position = QVector3D(mToolPosition.x(), mToolPosition.y(), 0);
        m_lines.append(vertex);
        vertex.position = QVector3D(x, y, 0);
        m_lines.append(vertex);
    }

    // Draw circles
    // Bottom
    m_lines += createCircle(
        QVector3D(
            mToolPosition.x(), mToolPosition.y(),
            mToolPosition.z() + mEndLength
        ),
        mToolDiameter / 2, 20, vertex.color
    );

    // Top
    m_lines += createCircle(
        QVector3D(
            mToolPosition.x(), mToolPosition.y(),
            mToolPosition.z() + mToolLength
        ),
        mToolDiameter / 2, 20, vertex.color
    );

    // Zero Z circle
    if (mEndLength == 0)
    {
        m_lines += createCircle(
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


QVector<VertexData> ToolDrawer::createCircle(QVector3D center, double radius, int arcs, QVector3D color)
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
        double angle = 2 * M_PI * i / arcs;
        double x = center.x() + radius * cos(angle);
        double y = center.y() + radius * sin(angle);

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

double ToolDrawer::toolDiameter() const
{
    return mToolDiameter;
}

void ToolDrawer::setToolDiameter(double toolDiameter)
{
    if (mToolDiameter != toolDiameter)
    {
        mToolDiameter = toolDiameter;
        update();
    }
}
double ToolDrawer::toolLength() const
{
    return mToolLength;
}

void ToolDrawer::setToolLength(double toolLength)
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
double ToolDrawer::rotationAngle() const
{
    return mRotationAngle;
}

void ToolDrawer::setRotationAngle(double rotationAngle)
{
    if (mRotationAngle != rotationAngle)
    {
        mRotationAngle = rotationAngle;
        update();
    }
}

void ToolDrawer::rotate(double angle)
{
    setRotationAngle(normalizeAngle(mRotationAngle + angle));
}

double ToolDrawer::toolAngle() const
{
    return mToolAngle;
}

void ToolDrawer::setToolAngle(double toolAngle)
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

double ToolDrawer::normalizeAngle(double angle)
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
