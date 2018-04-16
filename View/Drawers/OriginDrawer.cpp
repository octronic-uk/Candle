#include "OriginDrawer.h"

OriginDrawer::OriginDrawer()
{
    mLineWidth = 10;
}

OriginDrawer::~OriginDrawer()
{

}

bool OriginDrawer::updateData()
{
    QVector4D red = QVector4D(1.0, 0.0, 0.0, 1.0);
    QVector4D green = QVector4D(0.0, 1.0, 0.0, 1.0);
    QVector4D blue = QVector4D(0.0, 0.0, 1.0, 1.0);
    QVector4D white = QVector4D(1.0,1.0,1.0,1.0);

    mLines = {
        // X-axis
        {QVector3D(0, 0, 0), red, QVector3D(sNan, sNan, sNan)},
        {QVector3D(9, 0, 0), red, QVector3D(sNan, sNan, sNan)},
        {QVector3D(10, 0, 0), red, QVector3D(sNan, sNan, sNan)},
        {QVector3D(8, 0.5, 0), red, QVector3D(sNan, sNan, sNan)},
        {QVector3D(8, 0.5, 0), red, QVector3D(sNan, sNan, sNan)},
        {QVector3D(8, -0.5, 0), red, QVector3D(sNan, sNan, sNan)},
        {QVector3D(8, -0.5, 0), red, QVector3D(sNan, sNan, sNan)},
        {QVector3D(10, 0, 0), red, QVector3D(sNan, sNan, sNan)},

        // Y-axis
        {QVector3D(0, 0, 0),    green, QVector3D(sNan, sNan, sNan)},
        {QVector3D(0, 9, 0),    green, QVector3D(sNan, sNan, sNan)},
        {QVector3D(0, 10, 0),   green, QVector3D(sNan, sNan, sNan)},
        {QVector3D(0.5, 8, 0),  green, QVector3D(sNan, sNan, sNan)},
        {QVector3D(0.5, 8, 0),  green, QVector3D(sNan, sNan, sNan)},
        {QVector3D(-0.5, 8, 0), green, QVector3D(sNan, sNan, sNan)},
        {QVector3D(-0.5, 8, 0), green, QVector3D(sNan, sNan, sNan)},
        {QVector3D(0, 10, 0),   green, QVector3D(sNan, sNan, sNan)},

        // Z-axis
        {QVector3D(0, 0, 0),    blue, QVector3D(sNan, sNan, sNan)},
        {QVector3D(0, 0, 9),    blue, QVector3D(sNan, sNan, sNan)},
        {QVector3D(0, 0, 10),   blue, QVector3D(sNan, sNan, sNan)},
        {QVector3D(0.5, 0, 8),  blue, QVector3D(sNan, sNan, sNan)},
        {QVector3D(0.5, 0, 8),  blue, QVector3D(sNan, sNan, sNan)},
        {QVector3D(-0.5, 0, 8), blue, QVector3D(sNan, sNan, sNan)},
        {QVector3D(-0.5, 0, 8), blue, QVector3D(sNan, sNan, sNan)},
        {QVector3D(0, 0, 10),   blue, QVector3D(sNan, sNan, sNan)},

        // 2x2 rect
        {QVector3D(1, 1, 0),   white, QVector3D(sNan, sNan, sNan)},
        {QVector3D(-1, 1, 0),  white, QVector3D(sNan, sNan, sNan)},
        {QVector3D(-1, 1, 0),  white, QVector3D(sNan, sNan, sNan)},
        {QVector3D(-1, -1, 0), white, QVector3D(sNan, sNan, sNan)},
        {QVector3D(-1, -1, 0), white, QVector3D(sNan, sNan, sNan)},
        {QVector3D(1, -1, 0),  white, QVector3D(sNan, sNan, sNan)},
        {QVector3D(1, -1, 0),  white, QVector3D(sNan, sNan, sNan)},
        {QVector3D(1, 1, 0),   white, QVector3D(sNan, sNan, sNan)},
    };
    return true;
}

QVector3D OriginDrawer::getSizes()
{
   return QVector3D();
}

QVector3D OriginDrawer::getMinimumExtremes()
{
   return QVector3D();
}

QVector3D OriginDrawer::getMaximumExtremes()
{
   return QVector3D();
}

int OriginDrawer::getVertexCount()
{
   return mLines.size();
}
