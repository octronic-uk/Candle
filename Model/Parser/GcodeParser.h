// This file is a part of "CocoanutCNC" application.
// This file was originally ported from "GcodeParser.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QObject>
#include <QVector3D>
#include <cmath>
#include "PointSegment.h"
#include "GcodePreprocessorUtils.h"

class GcodeParser : public QObject
{
    Q_OBJECT
public:
    explicit GcodeParser(QObject *parent = nullptr);
    ~GcodeParser();

    bool getConvertArcsToLines();
    void setConvertArcsToLines(bool convertArcsToLines);
    bool getRemoveAllWhitespace();
    void setRemoveAllWhitespace(bool removeAllWhitespace);
    double getSmallArcSegmentLength();
    void setSmallArcSegmentLength(double smallArcSegmentLength);
    double getSmallArcThreshold();
    void setSmallArcThreshold(double smallArcThreshold);
    double getSpeedOverride();
    void setSpeedOverride(double speedOverride);
    int getTruncateDecimalLength();
    void setTruncateDecimalLength(int truncateDecimalLength);
    void reset(const QVector3D &initialPoint = QVector3D(qQNaN(), qQNaN(), qQNaN()));
    PointSegment *addCommand(QString command);
    PointSegment *addCommand(const QStringList &args);
    QVector3D* getCurrentPoint();
    QList<PointSegment *> expandArc();
    QStringList preprocessCommands(QStringList commands);
    QStringList preprocessCommand(QString command);
    QStringList convertArcsToLines(QString command);
    QList<PointSegment *> getPointSegmentList();
    double getTraverseSpeed() const;
    void setTraverseSpeed(double traverseSpeed);
    int getCommandNumber() const;

signals:

public slots:

private:

    // Current state
    bool mIsMetric;
    bool mInAbsoluteMode;
    bool mInAbsoluteIJKMode;
    float mLastGcodeCommand;
    QVector3D mCurrentPoint;
    int mCommandNumber;
    PointSegment::planes mCurrentPlane;

    // Settings
    double mSpeedOverride;
    int mTruncateDecimalLength;
    bool mRemoveAllWhitespace;
    bool mConvertArcsToLines;
    double mSmallArcThreshold;
    // Not configurable outside, but maybe it should be.
    double mSmallArcSegmentLength;

    double mLastSpeed;
    double mTraverseSpeed;
    double mLastSpindleSpeed;

    // The gcode.
    QList<PointSegment*> mPoints;
    PointSegment *processCommand(const QStringList &args);
    void handleMCode(float code, const QStringList &args);
    PointSegment *handleGCode(float code, const QStringList &args);
    PointSegment *addLinearPointSegment(const QVector3D &nextPoint, bool fastTraverse);
    PointSegment *addArcPointSegment(const QVector3D &nextPoint, bool clockwise, const QStringList &args);
    void setLastGcodeCommand(float num);
};

