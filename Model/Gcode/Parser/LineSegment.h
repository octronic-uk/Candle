// This file is a part of "CoconutCNC" application.
// This file was originally ported from "LineSegment.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QVector3D>
#include "PointSegment.h"

class LineSegment
{
public:
    LineSegment();
    LineSegment(const QVector3D &a, const QVector3D &b, int num);
    LineSegment(const LineSegment &initial);
    ~LineSegment();

    int getLineNumber() const;
    QList<QVector3D> getPointArray() const;
    QList<double> getPoints() const;

    QVector3D getStart() const;
    void setStart(const QVector3D &vector);

    QVector3D getEnd() const;
    void setEnd(const QVector3D &vector);

    void setToolHead(int head);
    int getToolhead() const;

    void setSpeed(double s);
    double getSpeed() const;

    void setIsZMovement(bool isZ);
    bool isZMovement() const;

    void setIsArc(bool isA);
    bool isArc() const;

    void setIsFastTraverse(bool isF);
    bool isFastTraverse() const;

    bool contains(const QVector3D &point) const;

    void setDrawn(bool drawn);
    bool drawn() const;

    void setIsMetric(bool isMetric);
    bool isMetric() const;

    void setIsAbsolute(bool isAbsolute);
    bool isAbsolute() const;

    void setIsHightlight(bool isHightlight);
    bool isHightlight() const;

    void setVertexIndex(int vertexIndex);
    int vertexIndex() const;

    void setSpindleSpeed(double spindleSpeed);
    double getSpindleSpeed() const;

    void setDwell(double dwell);
    double getDwell() const;

    void setIsClockwise(bool isClockwise);
    bool isClockwise() const;

    void setPlane(const PointSegment::planes &plane);
    PointSegment::planes plane() const;

private:
    int mToolhead;
    double mSpeed;
    double mSpindleSpeed;
    double mDwell;
    QVector3D mFirst;
    QVector3D mSecond;

    // Line properties
    bool mIsZMovement;
    bool mIsArc;
    bool mIsClockwise;
    bool mIsFastTraverse;
    int mLineNumber;
    bool mDrawn;
    bool mIsMetric;
    bool mIsAbsolute;
    bool mIsHightlight;
    int mVertexIndex;

    PointSegment::planes mPlane;
};
