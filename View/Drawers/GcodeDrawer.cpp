// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GcodeDrawer.h"

GcodeDrawer::GcodeDrawer()
    : QObject(),
      mIgnoreZ(false),
      mColorNormal(QColor("Blue")),
      mColorDrawn(QColor("Black")),
      mColorHighlight(QColor("Red")),
      mColorZMovement(QColor("Yellow")),
      mColorStart(QColor("Green")),
      mColorEnd(QColor("Red")),
      mColorArc(QColor("Red")),
      mGeometryUpdated(false)
{
    mPosition = QVector3D(0,0,0);
    mPointSize = 10;
    mLineWidth = 3;
    mViewParser = QSharedPointer<GcodeViewParser>::create();
}

GcodeDrawer::GcodeDrawer(const GcodeDrawer& other)
    : QObject(other.parent())
{
    mViewParser = other.mViewParser;
    mIgnoreZ = other.mIgnoreZ;
    mColorNormal = other.mColorNormal;
    mColorDrawn = other.mColorDrawn;
    mColorHighlight = other.mColorHighlight;
    mColorZMovement = other.mColorZMovement;
    mColorStart = other.mColorStart;
    mColorEnd = other.mColorEnd;
    mImage = other.mImage;
    mIndexes = other.mIndexes;
    mGeometryUpdated = other.mGeometryUpdated;

    if (mViewParser == nullptr)
    {
        mViewParser.create();
    }
}

void GcodeDrawer::update()
{
    mIndexes.clear();
    mGeometryUpdated = false;
    ShaderDrawable::update();
}

void GcodeDrawer::update(const QList<int>& indexes)
{
    // Store segments to update
    mIndexes += indexes;
}

bool GcodeDrawer::updateData()
{
    if (mIndexes.isEmpty())
    {
        return prepareVectors();
    }
    else
    {
        return updateVectors();
    }
}

bool GcodeDrawer::prepareVectors()
{
    qDebug() << "GcodeDrawer: preparing vectors" << this;

    if (mViewParser == nullptr)
    {
        qDebug() << "GcodeDrawer: Viewparser is null";
        return true;
    }

    QList<LineSegment> list = mViewParser->getLines();
    VertexData vertex;

    qDebug() << "GcodeDrawer: lines count" << list.count();

    // Clear all vertex data
    mLines.clear();
    mPoints.clear();
    mTriangles.clear();

    // Delete texture on mode change
    if (mTexture)
    {
        mTexture->destroy();
        delete mTexture;
        mTexture = nullptr;
    }

    bool drawFirstPoint = true;
    for (int i = 0; i < list.count(); i++)
    {

        if (qIsNaN(list.at(i).getEnd().z()))
        {
            continue;
        }

        // Find first point of toolpath
        if (drawFirstPoint)
        {

            if (qIsNaN(list.at(i).getEnd().x()) || qIsNaN(list.at(i).getEnd().y()))
            {
                continue;
            }

            // Draw first toolpath point
            vertex.color = Util::colorToVector(mColorStart);
            vertex.position = list.at(i).getEnd() + mPosition;
            if (mIgnoreZ)
            {
                vertex.position.setZ(0);
            }
            vertex.start = QVector3D(sNan, sNan, mPointSize);
            mPoints.append(vertex);

            drawFirstPoint = false;
            continue;
        }

        // Prepare vertices
        if (list.at(i).isRapidMovement())
        {
            vertex.start = list.at(i).getStart();
        }
        else
        {
            vertex.start = QVector3D(sNan, sNan, sNan);
        }

        list[i].setVertexIndex(mLines.count());

        // Set color
        vertex.color = getSegmentColorVector(list.at(i));

        // Line start
        vertex.position = list.at(i).getStart() + mPosition;
        if (mIgnoreZ) vertex.position.setZ(0);
        mLines.append(vertex);

        // Line end
        vertex.position = list.at(i).getEnd() + mPosition;
        if (mIgnoreZ) vertex.position.setZ(0);
        mLines.append(vertex);

        // Draw last toolpath point
        if (i == list.count() - 1)
        {
            vertex.color = Util::colorToVector(mColorEnd);
            vertex.position = list.at(i).getEnd() + mPosition;
            if (mIgnoreZ) vertex.position.setZ(0);
            vertex.start = QVector3D(sNan, sNan, mPointSize);
            mPoints.append(vertex);
        }
    }
    mGeometryUpdated = true;
    mIndexes.clear();
    return true;
}

bool GcodeDrawer::updateVectors()
{
    // Update vertices
    QList<LineSegment> list = mViewParser->getLines();

    // Map buffer
    VertexData *data = static_cast<VertexData*>(mVBO.map(QOpenGLBuffer::WriteOnly));

    // Update vertices for each line segment
    int vertexIndex;
    foreach (int i, mIndexes)
    {
        // Update vertex pair
        if (i < 0 || i > list.count() - 1)
            continue;
        vertexIndex = list.at(i).vertexIndex();
        if (vertexIndex >= 0)
        {
            // Update vertex array
            if (data)
            {
                data[vertexIndex].color = getSegmentColorVector(list.at(i));
                data[vertexIndex + 1].color = data[vertexIndex].color;
            }
            else
            {
                mLines[vertexIndex].color = getSegmentColorVector(list.at(i));
                mLines[vertexIndex + 1].color = mLines.at(vertexIndex).color;
            }
        }
    }

    mIndexes.clear();
    if (data) mVBO.unmap();
    return !data;
}



QVector4D GcodeDrawer::getSegmentColorVector(const LineSegment & segment)
{
    return Util::colorToVector(getSegmentColor(segment));
}

QColor GcodeDrawer::getSegmentColor(const LineSegment& segment)
{

    if (segment.isZMovement())
    {
        return mColorZMovement;
    }
    else if (segment.isArc())
    {
        return mColorArc;
    }
    else if (segment.drawn())
    {
        return mColorDrawn;
    }
    else if (segment.isHightlight())
    {
        return mColorHighlight;
    }
    else if (segment.isRapidMovement())
    {
        return mColorNormal;
    }

    return mColorNormal;
}

QVector3D GcodeDrawer::getSizes() const
{
    QVector3D min = mViewParser->getMinimumExtremes();
    QVector3D max = mViewParser->getMaximumExtremes();

    return QVector3D(max.x() - min.x(), max.y() - min.y(), max.z() - min.z());
}

QVector3D GcodeDrawer::getMinimumExtremes() const
{
    QVector3D v = mViewParser->getMinimumExtremes();
    if (mIgnoreZ)
        v.setZ(0);

    return v;
}

QVector3D GcodeDrawer::getMaximumExtremes() const
{
    QVector3D v = mViewParser->getMaximumExtremes();
    if (mIgnoreZ) v.setZ(0);

    return v;
}

void GcodeDrawer::initialise()
{
    mViewParser = QSharedPointer<GcodeViewParser>::create();
    mIgnoreZ = false;
    mColorNormal = QColor("Blue");
    mColorDrawn = QColor("Black");
    mColorHighlight = QColor("Red");
    mColorZMovement = QColor("Yellow");
    mColorStart = QColor("Green");
    mColorEnd = QColor("Red");
    mGeometryUpdated = false;
    mPointSize = 10;
    mLineWidth = 4;
    update();
}

void GcodeDrawer::setViewParser(const QSharedPointer<GcodeViewParser>& viewParser)
{
    mViewParser = viewParser;
}

QSharedPointer<GcodeViewParser> GcodeDrawer::viewParser()
{
    return mViewParser;
}

bool GcodeDrawer::geometryUpdated()
{
    return mGeometryUpdated;
}

QColor GcodeDrawer::colorNormal() const
{
    return mColorNormal;
}

void GcodeDrawer::setColorNormal(const QColor &colorNormal)
{
    mColorNormal = colorNormal;
}

QColor GcodeDrawer::colorHighlight() const
{
    return mColorHighlight;
}

void GcodeDrawer::setColorHighlight(const QColor &colorHighlight)
{
    mColorHighlight = colorHighlight;
}

QColor GcodeDrawer::colorZMovement() const
{
    return mColorZMovement;
}

void GcodeDrawer::setColorZMovement(const QColor &colorZMovement)
{
    mColorZMovement = colorZMovement;
}

QColor GcodeDrawer::colorDrawn() const
{
    return mColorDrawn;
}

void GcodeDrawer::setColorDrawn(const QColor &colorDrawn)
{
    mColorDrawn = colorDrawn;
}

QColor GcodeDrawer::colorStart() const
{
    return mColorStart;
}

void GcodeDrawer::setColorStart(const QColor &colorStart)
{
    mColorStart = colorStart;
}

QColor GcodeDrawer::colorEnd() const
{
    return mColorEnd;
}

void GcodeDrawer::setColorEnd(const QColor &colorEnd)
{
    mColorEnd = colorEnd;
}

bool GcodeDrawer::getIgnoreZ() const
{
    return mIgnoreZ;
}

void GcodeDrawer::setIgnoreZ(bool ignoreZ)
{
    mIgnoreZ = ignoreZ;
}

QVector3D GcodeDrawer::getPosition() const
{
    return mPosition;
}

void GcodeDrawer::setPosition(QVector3D position)
{
    mPosition = position;
    update();
}
