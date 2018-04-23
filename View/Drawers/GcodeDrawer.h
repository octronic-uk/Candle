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

    explicit GcodeDrawer();
    GcodeDrawer(const GcodeDrawer& other);

    void update();
    void update(const QList<int>& indexes);
    bool updateData() override;

    QVector3D getSizes() const;
    QVector3D getMinimumExtremes() const override;
    QVector3D getMaximumExtremes() const override;

    void initialise();

    void setViewParserHandle(GcodeViewParser* viewParser);
    GcodeViewParser* getViewParserHandle();

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

    GcodeParser getParser() const;
    void setParser(const GcodeParser& parser);

    QVector3D getPosition() const;
    void setPosition(QVector3D position);

signals:

public slots:

private slots:

private:
    GcodeViewParser* mViewParserHandle;
    bool mIgnoreZ;
    QColor mColorNormal;
    QColor mColorDrawn;
    QColor mColorHighlight;
    QColor mColorZMovement;
    QColor mColorStart;
    QColor mColorEnd;
    QColor mColorArc;
    QList<int> mIndexes;
    bool mGeometryUpdated;
    QVector3D mPosition;

    bool prepareVectors();
    bool updateVectors();

    QVector4D getSegmentColorVector(LineSegment* segment);
    QColor getSegmentColor(LineSegment* segment);
};
