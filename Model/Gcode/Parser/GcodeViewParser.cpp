// This file is a part of "CoconutCNC" application.
// This file was originally ported from "GcodeViewParse.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include <QDebug>
#include "GcodeViewParser.h"

GcodeViewParser::GcodeViewParser(QObject *parent) :
    QObject(parent)
{
    mAbsoluteMode = true;
    mAbsoluteIJK = false;
    mCurrentLine = 0;
    debug = true;

    mMin = QVector3D(qQNaN(), qQNaN(), qQNaN());
    mMax = QVector3D(qQNaN(), qQNaN(), qQNaN());

    mMinLength = qQNaN();
}

GcodeViewParser::~GcodeViewParser()
{
    //foreach (LineSegment *ls, mLines) delete ls;
}

QVector3D GcodeViewParser::getMinimumExtremes() const
{
    return mMin;
}

QVector3D GcodeViewParser::getMaximumExtremes() const
{
    return mMax;
}

void GcodeViewParser::testExtremes(const QVector3D &p3d)
{
    testExtremes(p3d.x(), p3d.y(), p3d.z());
}

void GcodeViewParser::testExtremes(double x, double y, double z)
{
    mMin.setX(Util::nMin(mMin.x(), x));
    mMin.setY(Util::nMin(mMin.y(), y));
    mMin.setZ(Util::nMin(mMin.z(), z));

    mMax.setX(Util::nMax(mMax.x(), x));
    mMax.setY(Util::nMax(mMax.y(), y));
    mMax.setZ(Util::nMax(mMax.z(), z));
}

void GcodeViewParser::testLength(const QVector3D &start, const QVector3D &end)
{
    double length = (start - end).length();
    if (!qIsNaN(length) && length != 0)
    {
        mMinLength = qIsNaN(mMinLength) ?
        length :
        qMin<double>(mMinLength, length);
    }
}

QList<LineSegment> GcodeViewParser::toObjRedux
(
    const QList<QString> &gcode,
    double arcPrecision,
    bool arcDegreeMode
)
{
    QSharedPointer<GcodeParser> gp;
    gp.create();

    foreach (QString s, gcode)
    {
        gp->addCommand(s);
    }

    return setLinesFromParser(gp.data(), arcPrecision, arcDegreeMode);
}

QList<LineSegment> GcodeViewParser::getLineSegmentList() const
{
    return mLines;
}

void GcodeViewParser::reset()
{
    //foreach (LineSegment *ls, mLines) delete ls;
    mLines.clear();
    mLineIndexes.clear();
    mCurrentLine = 0;
    mMin = QVector3D(qQNaN(), qQNaN(), qQNaN());
    mMax = QVector3D(qQNaN(), qQNaN(), qQNaN());
    mMinLength = qQNaN();
}

double GcodeViewParser::getMinLength() const
{
    return mMinLength;
}

QSize GcodeViewParser::getResolution() const
{
    return QSize
    (
        ((mMax.x() - mMin.x()) / mMinLength) + 1,
        ((mMax.y() - mMin.y()) / mMinLength) + 1
    );
}

QList<LineSegment> GcodeViewParser::setLinesFromParser
(
        GcodeParser* gp,
        double arcPrecision,
        bool arcDegreeMode
)
{
    qDebug() << "GcodeViewParse::getLinesFromParser";
    QList<PointSegment> segmentList = gp->getPointSegmentList();
    // For a line segment list ALL arcs must be converted to lines.
    double minArcLength = 0.1;
    double length;

    qDebug() << "GcodeViewParse: Point Segments" << segmentList.length();
    Q_UNUSED(length)

    QVector3D start, end;
    LineSegment ls;

    // Prepare segments indexes
    mLineIndexes.resize(segmentList.count());

    int lineIndex = 0;
    bool first = true;
    for(int i=0; i<segmentList.count(); i++)
    {
        PointSegment segment = segmentList[i];
        bool isMetric = segment.isMetric();
        segment.convertToMetric();
        end = segment.point();

        // start is null for the first iteration.
        if (!first)
        {
            // Expand arc for graphics.
            if (segment.isArc())
            {
                /*
                qDebug() << "GcodeViewParse: PS" << segment.getLineNumber() << "is arc"
                         << "Start" << start
                         << "End" << end;
                         */

                QList<QVector3D> points =
                    GcodeParser::generatePointsAlongArcBDring
                    (
                        segment.plane(),
                        start, end,
                        segment.center(), segment.isClockwise(),
                        segment.getRadius(),
                        minArcLength, arcPrecision, arcDegreeMode
                    );
                // Create line segments from points.
                if (points.length() > 0)
                {
                    QVector3D startPoint = start;
                    foreach (QVector3D nextPoint, points)
                    {
                        if (nextPoint == startPoint) continue;
                        ls = LineSegment(startPoint, nextPoint, lineIndex);
                        ls.setIsArc(segment.isArc());
                        ls.setIsClockwise(segment.isClockwise());
                        ls.setPlane(segment.plane());
                        ls.setIsRapidMovement(segment.isRapidMovement());
                        ls.setIsZMovement(segment.isZMovement());
                        ls.setIsMetric(isMetric);
                        ls.setIsAbsolute(segment.isAbsolute());
                        ls.setSpeed(segment.getSpeed());
                        ls.setSpindleSpeed(segment.getSpindleSpeed());
                        ls.setDwell(segment.getDwell());
                        testExtremes(nextPoint);
                        mLines.append(ls);
                        mLineIndexes[segment.getLineNumber()].append(mLines.count() - 1);
                        startPoint = nextPoint;
                    }
                    lineIndex++;
                }
            // Line
            }
            else
            {
                //qDebug() << "GcodeViewParse: PS" << segment.getLineNumber() << "is NOT arc";
                ls = LineSegment(start, end, lineIndex++);
                ls.setIsArc(segment.isArc());
                ls.setIsRapidMovement(segment.isRapidMovement());
                ls.setIsZMovement(segment.isZMovement());
                ls.setIsMetric(isMetric);
                ls.setIsAbsolute(segment.isAbsolute());
                ls.setSpeed(segment.getSpeed());
                ls.setSpindleSpeed(segment.getSpindleSpeed());
                ls.setDwell(segment.getDwell());
                testExtremes(end);
                testLength(start, end);
                mLines.append(ls);
                mLineIndexes[segment.getLineNumber()].append(mLines.count() - 1);
            }
        }
        else
        {
            //qDebug() << "GcodeViewParse: First line"
             //        << segment.getLineNumber();
            first = false;
        }

        start = end;
        //qDebug() << "GcodeViewParse: after the fact Start" << start << "end" << end;
    }

    return mLines;
}

QList<LineSegment>& GcodeViewParser::getLines()
{
    return mLines;
}

QVector<QList<int>> GcodeViewParser::getLinesIndexes() const
{
    return mLineIndexes;
}
