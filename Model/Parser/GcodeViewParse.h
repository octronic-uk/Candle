// This file is a part of "CocoanutCNC" application.
// This file was originally ported from "GcodeViewParse.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QObject>
#include <QVector3D>
#include <QVector2D>
#include "LineSegment.h"
#include "GcodeParser.h"
#include "Utils/Util.h"

class GcodeViewParse : public QObject
{
    Q_OBJECT
public:
    explicit GcodeViewParse(QObject *parent = 0);
    ~GcodeViewParse();

    QVector3D &getMinimumExtremes();
    QVector3D &getMaximumExtremes();
    double getMinLength() const;
    QSize getResolution() const;
    QList<LineSegment*> toObjRedux(QList<QString> gcode, double arcPrecision, bool arcDegreeMode);
    QList<LineSegment*> getLineSegmentList();
    QList<LineSegment*> getLinesFromParser(GcodeParser *gp, double arcPrecision, bool arcDegreeMode);

    QList<LineSegment*> *getLines();
    QVector<QList<int>> &getLinesIndexes();

    void reset();

signals:

public slots:

private:
    bool absoluteMode;
    bool absoluteIJK;

    // Parsed object
    QVector3D m_min, m_max;
    double m_minLength;
    QList<LineSegment*> m_lines;
    QVector<QList<int>> m_lineIndexes;

    // Parsing state.
    QVector3D lastPoint;
    int currentLine; // for assigning line numbers to segments.

    // Debug
    bool debug;
    void testExtremes(QVector3D p3d);
    void testExtremes(double x, double y, double z);
    void testLength(const QVector3D &start, const QVector3D &end);
};
