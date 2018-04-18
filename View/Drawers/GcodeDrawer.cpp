// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GcodeDrawer.h"

GcodeDrawer::GcodeDrawer()
    : QObject(),
      mDrawMode(GcodeDrawer::Vectors),
      mSimplify(false),
      mIgnoreZ(false),
      mGrayscaleSegments(false),
      mGrayscaleCode(GcodeDrawer::S),
      mGrayscaleMin(0),
      mGrayscaleMax(255),
      mColorNormal(QColor("Blue")),
      mColorDrawn(QColor("Black")),
      mColorHighlight(QColor("Red")),
      mColorZMovement(QColor("Yellow")),
      mColorStart(QColor("Green")),
      mColorEnd(QColor("Red")),
      mGeometryUpdated(false)
{
    mPointSize = 4;
    mLineWidth = 3;
    mViewParser = QSharedPointer<GcodeViewParser>::create();
}

GcodeDrawer::GcodeDrawer(const GcodeDrawer& other)
    : QObject(other.parent())
{
    mViewParser = other.mViewParser;
    mDrawMode = other.mDrawMode;
    mSimplify = other.mSimplify;
    mSimplifyPrecision = other.mSimplifyPrecision;
    mIgnoreZ = other.mIgnoreZ;
    mGrayscaleSegments = other.mGrayscaleSegments;
    mGrayscaleCode = other.mGrayscaleCode;
    mGrayscaleMin = other.mGrayscaleMin;
    mGrayscaleMax = other.mGrayscaleMax;
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
    switch (mDrawMode)
    {
        case GcodeDrawer::Vectors:
            if (mIndexes.isEmpty())
            {
                return prepareVectors();
            }
            else
            {
                return updateVectors();
            }
        case GcodeDrawer::Raster:
            if (mIndexes.isEmpty())
            {
                return prepareRaster();
            }
            else
            {
                return updateRaster();
            }
    }
    return false;
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
            vertex.position = list.at(i).getEnd();
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
        if (list.at(i).isFastTraverse())
        {
            vertex.start = list.at(i).getStart();
        }
        else
        {
            vertex.start = QVector3D(sNan, sNan, sNan);
        }

        // Simplify geometry
        int j = i;
        if (mSimplify && i < list.count() - 1)
        {
            QVector3D start = list.at(i).getEnd() - list.at(i).getStart();
            QVector3D next;
            double length = start.length();
            bool straight = false;

            do
            {
                list[i].setVertexIndex(mLines.count()); // Store vertex index
                i++;
                if (i < list.count() - 1)
                {
                    next = list.at(i).getEnd() - list.at(i).getStart();
                    length += next.length();
                    //                    straight = start.crossProduct(start.normalized(), next.normalized()).length() < 0.025;
                }
                // Split short & straight lines
            }
            while
                    (
                     (length < mSimplifyPrecision || straight) &&
                     i < list.count() &&
                     getSegmentType(list.at(i)) == getSegmentType(list.at(j))
                     );
            i--;
        }
        else
        {
            list[i].setVertexIndex(mLines.count()); // Store vertex index
        }

        // Set color
        vertex.color = getSegmentColorVector(list.at(i));

        // Line start
        vertex.position = list.at(j).getStart();
        if (mIgnoreZ) vertex.position.setZ(0);
        mLines.append(vertex);

        // Line end
        vertex.position = list.at(i).getEnd();
        if (mIgnoreZ) vertex.position.setZ(0);
        mLines.append(vertex);

        // Draw last toolpath point
        if (i == list.count() - 1)
        {
            vertex.color = Util::colorToVector(mColorEnd);
            vertex.position = list.at(i).getEnd();
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

bool GcodeDrawer::prepareRaster()
{
    const int maxImageSize = 8192;

    qDebug() << "GcodeDrawer: preparing raster" << this;

    // Generate image
    QImage image;
    qDebug() << "GcodeDrawer: image info" << mViewParser->getResolution() << mViewParser->getMinLength();

    if (mViewParser->getResolution().width() <= maxImageSize && mViewParser->getResolution().height() <= maxImageSize)
    {
        image = QImage(mViewParser->getResolution(), QImage::Format_RGB888);
        image.fill(Qt::white);

        QList<LineSegment> list = mViewParser->getLines();
        qDebug() << "GcodeDrawer: lines count" << list.count();

        double pixelSize = mViewParser->getMinLength();
        QVector3D origin = mViewParser->getMinimumExtremes();

        for (int i = 0; i < list.count(); i++)
        {
            if (!qIsNaN(list.at(i).getEnd().length()))
            {
                setImagePixelColor
                        (
                            image,
                            (list.at(i).getEnd().x() - origin.x()) / pixelSize,
                            (list.at(i).getEnd().y() - origin.y()) / pixelSize,
                            getSegmentColor(list.at(i)).rgb()
                            );
            }
        }
    }

    // Create vertices array
    // Clear all vertex data
    mLines.clear();
    mPoints.clear();
    mTriangles.clear();

    if (mTexture)
    {
        mTexture->destroy();
        delete mTexture;
        mTexture = nullptr;
    }

    QVector<VertexData> vertices;
    VertexData vertex;

    // Set color
    vertex.color = Util::colorToVector(Qt::red);

    // Rect
    vertex.start = QVector3D(sNan, 0, 0);
    vertex.position = QVector3D(getMinimumExtremes().x(), getMinimumExtremes().y(), 0);
    vertices.append(vertex);

    vertex.start = QVector3D(sNan, 1, 1);
    vertex.position = QVector3D(getMaximumExtremes().x(), getMaximumExtremes().y(), 0);
    vertices.append(vertex);

    vertex.start = QVector3D(sNan, 0, 1);
    vertex.position = QVector3D(getMinimumExtremes().x(), getMaximumExtremes().y(), 0);
    vertices.append(vertex);

    vertex.start = QVector3D(sNan, 0, 0);
    vertex.position = QVector3D(getMinimumExtremes().x(), getMinimumExtremes().y(), 0);
    vertices.append(vertex);

    vertex.start = QVector3D(sNan, 1, 0);
    vertex.position = QVector3D(getMaximumExtremes().x(), getMinimumExtremes().y(), 0);
    vertices.append(vertex);

    vertex.start = QVector3D(sNan, 1, 1);
    vertex.position = QVector3D(getMaximumExtremes().x(), getMaximumExtremes().y(), 0);
    vertices.append(vertex);

    if (!image.isNull())
    {
        mTexture = new QOpenGLTexture(image);
        mTriangles += vertices;
        mImage = image;
    }
    else
    {
        for (int i = 0; i < vertices.count(); i++)
        {
            vertices[i].start = QVector3D(sNan, sNan, sNan);
        }
        mLines += vertices;
        mImage = QImage();
    }

    mGeometryUpdated = true;
    mIndexes.clear();
    return true;
}

bool GcodeDrawer::updateRaster()
{
    if (!mImage.isNull())
    {
        QList<LineSegment> list = mViewParser->getLines();

        double pixelSize = mViewParser->getMinLength();
        QVector3D origin = mViewParser->getMinimumExtremes();

        foreach (int i, mIndexes)
            setImagePixelColor
                    (
                        mImage,
                        (list.at(i).getEnd().x() - origin.x()) / pixelSize,
                        (list.at(i).getEnd().y() - origin.y()) / pixelSize,
                        getSegmentColor(list.at(i)).rgb()
                        );

        if (mTexture)
            mTexture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, mImage.bits());
    }

    mIndexes.clear();
    return false;
}

void GcodeDrawer::setImagePixelColor(QImage &image, double x, double y, QRgb color) const
{
    if (qIsNaN(x) || qIsNaN(y))
    {
        qDebug() << "GcodeDrawer: Error updating pixel" << x << y;
        return;
    };

    uchar *pixel = image.scanLine((int)y);

    *(pixel + (int)x * 3) = qRed(color);
    *(pixel + (int)x * 3 + 1) = qGreen(color);
    *(pixel + (int)x * 3 + 2) = qBlue(color);
}

QVector4D GcodeDrawer::getSegmentColorVector(const LineSegment & segment)
{
    return Util::colorToVector(getSegmentColor(segment));
}

QColor GcodeDrawer::getSegmentColor(const LineSegment& segment)
{
    if (segment.drawn())
        return mColorDrawn;//QVector3D(0.85, 0.85, 0.85);
    else if (segment.isHightlight())
        return mColorHighlight;//QVector3D(0.57, 0.51, 0.9);
    else if (segment.isFastTraverse())
        return mColorNormal;// QVector3D(0.0, 0.0, 0.0);
    else if (segment.isZMovement())
        return mColorZMovement;//QVector3D(1.0, 0.0, 0.0);
    else if (mGrayscaleSegments)
        switch (mGrayscaleCode)
        {
            case GrayscaleCode::S:
                return QColor::fromHsl
                        (
                            0, 0,
                            qBound<int>
                            (
                                0,
                                255 - 255.0 / (mGrayscaleMax - mGrayscaleMin) *
                                segment.getSpindleSpeed(),
                                255
                                )
                            );
            case GrayscaleCode::Z:
                return QColor::fromHsl
                        (
                            0, 0,
                            qBound<int>
                            (
                                0, 255 - 255.0 / (mGrayscaleMax - mGrayscaleMin) *
                                segment.getStart().z(),
                                255
                                )
                            );
        }
    return mColorNormal;//QVector3D(0.0, 0.0, 0.0);
}

int GcodeDrawer::getSegmentType(const LineSegment& segment)
{
    return segment.isFastTraverse() + segment.isZMovement() * 2;
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
    mDrawMode = DrawMode::Vectors;
    mSimplify = false;
    mSimplifyPrecision = 1.0;
    mIgnoreZ = false;
    mGrayscaleSegments = false;
    mGrayscaleCode = GcodeDrawer::S;
    mGrayscaleMin = 0;
    mGrayscaleMax = 255;
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

bool GcodeDrawer::simplify() const
{
    return mSimplify;
}

void GcodeDrawer::setSimplify(bool simplify)
{
    mSimplify = simplify;
}

double GcodeDrawer::simplifyPrecision() const
{
    return mSimplifyPrecision;
}

void GcodeDrawer::setSimplifyPrecision(double simplifyPrecision)
{
    mSimplifyPrecision = simplifyPrecision;
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

GcodeDrawer::DrawMode GcodeDrawer::drawMode() const
{
    return mDrawMode;
}

void GcodeDrawer::setDrawMode(const DrawMode &drawMode)
{
    mDrawMode = drawMode;
}

int GcodeDrawer::grayscaleMax() const
{
    return mGrayscaleMax;
}

void GcodeDrawer::setGrayscaleMax(int grayscaleMax)
{
    mGrayscaleMax = grayscaleMax;
}

int GcodeDrawer::grayscaleMin() const
{
    return mGrayscaleMin;
}

void GcodeDrawer::setGrayscaleMin(int grayscaleMin)
{
    mGrayscaleMin = grayscaleMin;
}

GcodeDrawer::GrayscaleCode GcodeDrawer::grayscaleCode() const
{
    return mGrayscaleCode;
}

void GcodeDrawer::setGrayscaleCode(const GrayscaleCode &grayscaleCode)
{
    mGrayscaleCode = grayscaleCode;
}

bool GcodeDrawer::getGrayscaleSegments() const
{
    return mGrayscaleSegments;
}

void GcodeDrawer::setGrayscaleSegments(bool grayscaleSegments)
{
    mGrayscaleSegments = grayscaleSegments;
}









