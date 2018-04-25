// This file is a part of "CoconutCNC" application.
// This file was originally ported from "PointSegment.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QSharedPointer>
#include <QVector3D>

#include "ArcProperties.h"

class GcodeCommand;

class PointSegment
{
public:
    enum planes
    {
        XY,
        ZX,
        YZ
    };

    PointSegment(GcodeCommand* parent);
    PointSegment(const PointSegment &ps);
    PointSegment(GcodeCommand* parent, const QVector3D &b, int num);
    PointSegment(GcodeCommand* parent, const QVector3D &getPointHandle, int num, const QVector3D &center, double radius, bool clockwise);
    ~PointSegment();

    QVector3D* getPointHandle();

    QVector<double> points() const;

    void setToolHead(int head);
    int getToolhead() const;

    void setLineNumber(int num);
    int getLineNumber() const;

    void setSpeed(double s);
    double getSpeed() const;

    void setIsZMovement(bool isZ);
    bool isZMovement() const;

    void setIsMetric(bool mIsMetric);
    bool isMetric() const;

    void setIsArc(bool isA);
    bool isArc() const;

    void setIsRapidMovement(bool isF);
    bool isRapidMovement() const;

    void setArcCenter(const QVector3D &center);

    QVector<double> centerPoints() const;
    QVector3D center() const;

    void setIsClockwise(bool clockwise);
    bool isClockwise() const;

    void setRadius(double rad);
    double getRadius() const;

    void convertToMetric();

    bool isAbsolute() const;
    void setIsAbsolute(bool isAbsolute);

    planes plane() const;
    void setPlane(const planes &plane);

    double getSpindleSpeed() const;
    void setSpindleSpeed(double spindleSpeed);

    double getDwell() const;
    void setDwell(double dwell);

    GcodeCommand* getParent() const;
    void setParent(GcodeCommand* parent);

private:
    GcodeCommand* mParent;
    ArcProperties mArcProperties;
    int mToolhead;
    double mSpeed;
    double mSpindleSpeed;
    double mDwell;
    QSharedPointer<QVector3D> mPoint;
    bool mIsMetric;
    bool mIsZMovement;
    bool mIsArc;
    bool mIsRapidMovement;
    bool mIsAbsolute;
    int mLineNumber;
    planes mPlane;
};
