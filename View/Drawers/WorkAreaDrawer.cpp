#include "WorkAreaDrawer.h"

WorkAreaDrawer::WorkAreaDrawer(int width, int height, QColor color)
    : ShaderDrawable(),
      mWidth(width),
      mHeight(height),
      mColor(color)
{
    mColor.setAlphaF(0.5);
    mVisible = true;
    mLineWidth = 1;
}

WorkAreaDrawer::~WorkAreaDrawer()
{

}

bool WorkAreaDrawer::updateData()
{
    VertexData topLeft, topRight, bottomLeft, bottomRight;

    topLeft.color = Util::colorToVector(mColor);
    topRight.color = topLeft.color;
    bottomLeft.color = topLeft.color;
    bottomRight.color = topLeft.color;

    topLeft.start = QVector3D(0.0,0.0,0.0);
    topRight.start = QVector3D(0.0,0.0,0.0);
    bottomLeft.start = QVector3D(0.0,0.0,0.0);
    bottomRight.start = QVector3D(0.0,0.0,0.0);

    int halfWidth = mWidth/2;
    int halfHeight = mHeight/2;

    topLeft.position     = QVector3D(-halfWidth, halfHeight , 0.0);
    topRight.position    = QVector3D( halfWidth, halfHeight , 0.0);
    bottomLeft.position  = QVector3D(-halfWidth, -halfHeight, 0.0);
    bottomRight.position = QVector3D( halfWidth, -halfHeight, 0.0);

    // Triangle 1
    mTriangles.append(topLeft);
    mTriangles.append(topRight);
    mTriangles.append(bottomRight);
    // Triangle 2
    mTriangles.append(bottomRight);
    mTriangles.append(bottomLeft);
    mTriangles.append(topLeft);

    return true;
}

QVector3D WorkAreaDrawer::getSizes()
{
   return QVector3D(mWidth,mHeight,0.0f);
}

QVector3D WorkAreaDrawer::getMinimumExtremes()
{
   return QVector3D(-(mWidth/2),-(mHeight/2),0.0f);
}

QVector3D WorkAreaDrawer::getMaximumExtremes()
{
   return QVector3D(mWidth/2,mHeight/2,0.0f);
}

int WorkAreaDrawer::getVertexCount()
{
   return mLines.size();
}

int WorkAreaDrawer::getHeight() const
{
    return mHeight;
}

void WorkAreaDrawer::setHeight(int height)
{
    mHeight = height;
}

int WorkAreaDrawer::getWidth() const
{
    return mWidth;
}

void WorkAreaDrawer::setWidth(int width)
{
    mWidth = width;
}

QColor WorkAreaDrawer::getColor() const
{
    return mColor;
}

void WorkAreaDrawer::setColor(const QColor& color)
{
    mColor = color;
}
