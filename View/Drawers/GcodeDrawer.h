// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QObject>
#include <QVector3D>
#include "Model/Gcode/Parser/LineSegment.h"
#include "Model/Gcode/Parser/GcodeViewParser.h"
#include "ShaderDrawable.h"

class GcodeDrawer : public QObject, public ShaderDrawable
{
    Q_OBJECT
public:
    enum GrayscaleCode
    {
        S,
        Z
    };
    enum DrawMode
    {
        Vectors,
        Raster
    };

    explicit GcodeDrawer();
    GcodeDrawer(const GcodeDrawer& other);

    void update();
    void update(const QList<int>& indexes);
    bool updateData() override;

    QVector3D getSizes() const;
    QVector3D getMinimumExtremes() const override;
    QVector3D getMaximumExtremes() const override;

    void initialise();

    void setViewParser(const QSharedPointer<GcodeViewParser>& viewParser);
    QSharedPointer<GcodeViewParser> viewParser();

    bool simplify() const;
    void setSimplify(bool simplify);

    double simplifyPrecision() const;
    void setSimplifyPrecision(double simplifyPrecision);

    bool geometryUpdated();

    QColor colorNormal() const;
    void setColorNormal(const QColor &colorNormal);

    QColor colorHighlight() const;
    void setColorHighlight(const QColor &colorHighlight);

    QColor colorZMovement() const;
    void setColorZMovement(const QColor &colorZMovement);

    QColor colorDrawn() const;
    void setColorDrawn(const QColor &colorDrawn);

    QColor colorStart() const;
    void setColorStart(const QColor &colorStart);

    QColor colorEnd() const;
    void setColorEnd(const QColor &colorEnd);

    bool getIgnoreZ() const;
    void setIgnoreZ(bool ignoreZ);

    bool getGrayscaleSegments() const;
    void setGrayscaleSegments(bool grayscaleSegments);

    GrayscaleCode grayscaleCode() const;
    void setGrayscaleCode(const GrayscaleCode &grayscaleCode);

    int grayscaleMin() const;
    void setGrayscaleMin(int grayscaleMin);

    int grayscaleMax() const;
    void setGrayscaleMax(int grayscaleMax);

    DrawMode drawMode() const;
    void setDrawMode(const DrawMode &drawMode);

    GcodeParser getParser() const;
    void setParser(const GcodeParser& parser);

signals:

public slots:

private slots:
    void onTimerVertexUpdate();

private:
    QSharedPointer<GcodeViewParser> mViewParser;
    DrawMode mDrawMode;
    bool mSimplify;
    double mSimplifyPrecision;
    bool mIgnoreZ;
    bool mGrayscaleSegments;
    GrayscaleCode mGrayscaleCode;
    int mGrayscaleMin;
    int mGrayscaleMax;
    QColor mColorNormal;
    QColor mColorDrawn;
    QColor mColorHighlight;
    QColor mColorZMovement;
    QColor mColorStart;
    QColor mColorEnd;
    QTimer mTimerVertexUpdate;
    QImage mImage;
    QList<int> mIndexes;
    bool mGeometryUpdated;

    bool prepareVectors();
    bool updateVectors();
    bool prepareRaster();
    bool updateRaster();

    int getSegmentType(const LineSegment& segment);
    QVector3D getSegmentColorVector(const LineSegment& segment);
    QColor getSegmentColor(const LineSegment& segment);
    void setImagePixelColor(QImage &image, double x, double y, const QRgb color) const;
};
