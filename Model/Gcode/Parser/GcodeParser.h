// This file is a part of "CoconutCNC" application.
// This file was originally ported from "GcodeParser.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QObject>
#include <QVector3D>
#include <cmath>
#include "Model/Gcode/Parser/PointSegment.h"
#include "Model/Gcode/GcodeCommand.h"

class GcodeParser : public QObject
{
    Q_OBJECT
public:
    explicit GcodeParser(QObject *parent = nullptr);
    ~GcodeParser();

    void initialise();

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
    PointSegment addCommand(QString command);
    PointSegment addCommand(const QStringList &args);
    QVector3D getCurrentPoint();
    QList<PointSegment> expandArc();
    QStringList preprocessCommands(QStringList commands);
    QStringList preprocessCommand(QString command);
    QStringList convertArcsToLines(QString command);
    QList<PointSegment>& getPointSegmentList();
    double getTraverseSpeed() const;
    void setTraverseSpeed(double traverseSpeed);
    int getCommandNumber() const;

    static QString overrideSpeed(const QString command, double speed);
    static QString removeComment(QString command);
    static QString parseComment(const QString command);
    static QString truncateDecimals(int length, QString command);
    static QString removeAllWhitespace(const QString command);
    static QList<float> parseCodes(const QStringList &args, char code);
    static QList<int> parseGCodes(const QString command);
    static QList<int> parseMCodes(const QString command);
    static QStringList splitCommand(const QString &command);
    static double parseCoord(QStringList argList, char c);
    static QVector3D updatePointWithCommand(const QVector3D &initial, double x, double y, double z, bool absoluteMode);
    static QVector3D updatePointWithCommand(const QStringList &commandArgs, const QVector3D &initial, bool absoluteMode);
    static QVector3D updatePointWithCommand(const QString &command, const QVector3D &initial, bool absoluteMode);
    static QVector3D convertRToCenter(QVector3D start, QVector3D end, double radius, bool absoluteIJK, bool clockwise);
    static QVector3D updateCenterWithCommand(QStringList commandArgs, QVector3D initial, QVector3D nextPoint, bool absoluteIJKMode, bool clockwise);
    static QString generateG1FromPoints(QVector3D start, QVector3D end, bool absoluteMode, int precision);
    static double getAngle(QVector3D start, QVector3D end);
    static double calculateSweep(double startAngle, double endAngle, bool isCw);
    static QList<QVector3D> generatePointsAlongArcBDring(PointSegment::planes plane, QVector3D start, QVector3D end, QVector3D center, bool clockwise, double R, double minArcLength, double arcPrecision, bool arcDegreeMode);
    static QList<QVector3D> generatePointsAlongArcBDring(PointSegment::planes plane, QVector3D p1, QVector3D p2, QVector3D center, bool isCw, double radius, double startAngle, double sweep, int numPoints);
    static bool isDigit(char c);
    static bool isLetter(char c);
    static char toUpper(char c);
    static GcodeCommand* overrideSpeed(const GcodeCommand *command, double speed);

    const float GCODE_RAPID = 0.0f;
    const float GCODE_LINEAR_INTERPOLATION = 1.0;
    const float GCODE_STRAIGHT_PROBE = 38.2f;
    const float GCODE_ARC_MOVE_IJK = 2.0f;
    const float GCODE_ARC_MOVE_RP = 3.0f;
    const float GCODE_PLANE_XY = 17.0f;
    const float GCODE_PLANE_ZX = 18.0f;
    const float GCODE_PLANE_YZ = 19.0f;
    const float GCODE_UNITS_INCHES = 20.0f;
    const float GCODE_UNITS_MM = 21.0f;
    const float GCODE_DISTANCE_ABSOLUTE = 90.0f;
    const float GCODE_DISTANCE_ABSOLUTE_IJK = 90.1f;
    const float GCODE_DISTANCE_INCREMENTAL = 91.0f;
    const float GCODE_DISTANCE_INCREMENTAL_IJK = 91.1f;
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
    QList<PointSegment> mPoints;
    PointSegment processCommand(const QStringList &args);
    void handleMCode(float code, const QStringList &args);
    PointSegment handleGCode(float code, const QStringList &args);
    PointSegment addLinearPointSegment(const QVector3D &nextPoint, bool fastTraverse);
    PointSegment addArcPointSegment(const QVector3D &nextPoint, bool clockwise, const QStringList &args);
    void setLastGcodeCommand(float num);
};

