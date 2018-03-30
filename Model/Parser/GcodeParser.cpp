// This file is a part of "CocoanutCNC" application.
// This file was originally ported from "GcodeParser.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include <QListIterator>
#include <QDebug>
#include "GcodeParser.h"

GcodeParser::GcodeParser(QObject *parent)
    : QObject(parent),
      mIsMetric(true),
      mInAbsoluteMode(true),
      mInAbsoluteIJKMode(false),
      mLastGcodeCommand(-1),
      mCommandNumber(0),
      mSpeedOverride(-1),
      mTruncateDecimalLength(40),
      mRemoveAllWhitespace(true),
      mConvertArcsToLines(false),
      mSmallArcThreshold(1.0),
      mSmallArcSegmentLength(0.3),
      mLastSpeed(0),
      mTraverseSpeed(300),
      mLastSpindleSpeed(0)
{
    reset();
}

GcodeParser::~GcodeParser()
{
    foreach (PointSegment *ps, mPoints) delete ps;
}

bool GcodeParser::getConvertArcsToLines()
{
    return mConvertArcsToLines;
}

void GcodeParser::setConvertArcsToLines(bool convertArcsToLines)
{
    mConvertArcsToLines = convertArcsToLines;
}

bool GcodeParser::getRemoveAllWhitespace()
{
    return mRemoveAllWhitespace;
}

void GcodeParser::setRemoveAllWhitespace(bool removeAllWhitespace)
{
    mRemoveAllWhitespace = removeAllWhitespace;
}

double GcodeParser::getSmallArcSegmentLength()
{
    return mSmallArcSegmentLength;
}

void GcodeParser::setSmallArcSegmentLength(double smallArcSegmentLength)
{
    mSmallArcSegmentLength = smallArcSegmentLength;
}

double GcodeParser::getSmallArcThreshold()
{
    return mSmallArcThreshold;
}

void GcodeParser::setSmallArcThreshold(double smallArcThreshold)
{
    mSmallArcThreshold = smallArcThreshold;
}

double GcodeParser::getSpeedOverride()
{
    return mSpeedOverride;
}

void GcodeParser::setSpeedOverride(double speedOverride)
{
    mSpeedOverride = speedOverride;
}

int GcodeParser::getTruncateDecimalLength()
{
    return mTruncateDecimalLength;
}

void GcodeParser::setTruncateDecimalLength(int truncateDecimalLength)
{
    mTruncateDecimalLength = truncateDecimalLength;
}

// Resets the current state.
void GcodeParser::reset(const QVector3D &initialPoint)
{
    qDebug() << "GcodeParser: Reseting parser with initial point" << initialPoint;
    foreach (PointSegment *ps, mPoints)
    {
        delete ps;
    }
    mPoints.clear();
    // The unspoken home location.
    mCurrentPoint = initialPoint;
    mCurrentPlane = PointSegment::XY;
    mPoints.append(new PointSegment(&mCurrentPoint, -1));
}

/**
* Add a command to be processed.
*/
PointSegment* GcodeParser::addCommand(QString command)
{
    QStringList stripped = GcodePreprocessorUtils::removeComment(command);
    QStringList args = GcodePreprocessorUtils::splitCommand(stripped.at(0));
    return addCommand(args);
}

/**
* Add a command which has already been broken up into its arguments.
*/
PointSegment* GcodeParser::addCommand(const QStringList &args)
{
    if (args.isEmpty())
    {
        return nullptr;
    }
    return processCommand(args);
}

/**
* Warning, this should only be used when modifying live gcode, such as when
* expanding an arc or canned cycle into line segments.
*/
void GcodeParser::setLastGcodeCommand(float num)
{
    mLastGcodeCommand = num;
}

/**
* Gets the point at the end of the list.
*/
QVector3D *GcodeParser::getCurrentPoint()
{
    return &mCurrentPoint;
}

/**
* Expands the last point in the list if it is an arc according to the
* the parsers settings.
*/
QList<PointSegment*> GcodeParser::expandArc()
{
    PointSegment *startSegment = mPoints[mPoints.size() - 2];
    PointSegment *lastSegment = mPoints[mPoints.size() - 1];

    QList<PointSegment*> empty;

    // Can only expand arcs.
    if (!lastSegment->isArc())
    {
        return empty;
    }

    // Get precalculated stuff.
    QVector3D *start = startSegment->point();
    QVector3D *end = lastSegment->point();
    QVector3D *center = lastSegment->center();
    double radius = lastSegment->getRadius();
    bool clockwise = lastSegment->isClockwise();
    PointSegment::planes plane = startSegment->plane();

    //
    // Start expansion.
    //

    QList<QVector3D> expandedPoints =
            GcodePreprocessorUtils::generatePointsAlongArcBDring(
                plane, *start, *end, *center, clockwise, radius,
                mSmallArcThreshold, mSmallArcSegmentLength, false
                );

    // Validate output of expansion.
    if (expandedPoints.length() == 0)
    {
        return empty;
    }

    // Remove the last point now that we're about to expand it.
    mPoints.removeLast();
    mCommandNumber--;

    // Initialize return value
    QList<PointSegment*> psl;

    // Create line segments from points.
    PointSegment *temp;

    QListIterator<QVector3D> psi(expandedPoints);
    // skip first element.
    if (psi.hasNext()) psi.next();

    while (psi.hasNext())
    {
        temp = new PointSegment(&psi.next(), mCommandNumber++);
        temp->setIsMetric(lastSegment->isMetric());
        mPoints.append(temp);
        psl.append(temp);
    }

    // Update the new endpoint.
    mCurrentPoint.setX(mPoints.last()->point()->x());
    mCurrentPoint.setY(mPoints.last()->point()->y());
    mCurrentPoint.setZ(mPoints.last()->point()->z());

    return psl;
}

QList<PointSegment*> GcodeParser::getPointSegmentList()
{
    return mPoints;
}

double GcodeParser::getTraverseSpeed() const
{
    return mTraverseSpeed;
}

void GcodeParser::setTraverseSpeed(double traverseSpeed)
{
    mTraverseSpeed = traverseSpeed;
}

int GcodeParser::getCommandNumber() const
{
    return mCommandNumber - 1;
}

PointSegment *GcodeParser::processCommand(const QStringList &args)
{
    QList<float> gCodes;
    PointSegment *ps = nullptr;

    // Handle F code
    double speed = GcodePreprocessorUtils::parseCoord(args, 'F');
    if (!qIsNaN(speed))
    {
        mLastSpeed = mIsMetric ? speed : speed * 25.4;
    }

    // Handle S code
    double spindleSpeed = GcodePreprocessorUtils::parseCoord(args, 'S');
    if (!qIsNaN(spindleSpeed))
    {
        mLastSpindleSpeed = spindleSpeed;
    }

    // Handle P code
    double dwell = GcodePreprocessorUtils::parseCoord(args, 'P');
    if (!qIsNaN(dwell))
    {
        mPoints.last()->setDwell(dwell);
    }

    // handle G codes.
    gCodes = GcodePreprocessorUtils::parseCodes(args, 'G');

    // If there was no command, add the implicit one to the party.
    if (gCodes.isEmpty() && mLastGcodeCommand != -1)
    {
        gCodes.append(mLastGcodeCommand);
    }

    foreach (float code, gCodes)
    {
        ps = handleGCode(code, args);
    }

    return ps;
}

PointSegment *GcodeParser::addLinearPointSegment(const QVector3D &nextPoint, bool fastTraverse)
{
    PointSegment *ps = new PointSegment(&nextPoint, mCommandNumber++);

    bool zOnly = false;

    // Check for z-only
    if ((mCurrentPoint.x() == nextPoint.x()) &&
        (mCurrentPoint.y() == nextPoint.y()) &&
        (mCurrentPoint.z() != nextPoint.z()))
    {
        zOnly = true;
    }

    ps->setIsMetric(mIsMetric);
    ps->setIsZMovement(zOnly);
    ps->setIsFastTraverse(fastTraverse);
    ps->setIsAbsolute(mInAbsoluteMode);
    ps->setSpeed(fastTraverse ? mTraverseSpeed : mLastSpeed);
    ps->setSpindleSpeed(mLastSpindleSpeed);
    mPoints.append(ps);

    // Save off the endpoint.
    mCurrentPoint = nextPoint;

    return ps;
}

PointSegment *GcodeParser::addArcPointSegment(const QVector3D &nextPoint, bool clockwise, const QStringList &args)
{
    PointSegment *ps = new PointSegment(&nextPoint, mCommandNumber++);

    QVector3D center = GcodePreprocessorUtils::updateCenterWithCommand(args, mCurrentPoint, nextPoint, mInAbsoluteIJKMode, clockwise);
    double radius = GcodePreprocessorUtils::parseCoord(args, 'R');

    // Calculate radius if necessary.
    if (qIsNaN(radius))
    {

        QMatrix4x4 m;
        m.setToIdentity();
        switch (mCurrentPlane)
        {
            case PointSegment::XY:
                break;
            case PointSegment::ZX:
                m.rotate(90, 1.0, 0.0, 0.0);
                break;
            case PointSegment::YZ:
                m.rotate(-90, 0.0, 1.0, 0.0);
                break;
        }

        radius = sqrt(pow((double)((m * mCurrentPoint).x() - (m * center).x()), 2.0)
                      + pow((double)((m * mCurrentPoint).y() - (m * center).y()), 2.0));
    }

    ps->setIsMetric(mIsMetric);
    ps->setArcCenter(&center);
    ps->setIsArc(true);
    ps->setRadius(radius);
    ps->setIsClockwise(clockwise);
    ps->setIsAbsolute(mInAbsoluteMode);
    ps->setSpeed(mLastSpeed);
    ps->setSpindleSpeed(mLastSpindleSpeed);
    ps->setPlane(mCurrentPlane);
    mPoints.append(ps);

    // Save off the endpoint.
    mCurrentPoint = nextPoint;
    return ps;
}

void GcodeParser::handleMCode(float code, const QStringList &args)
{
    Q_UNUSED(code)
    double spindleSpeed = GcodePreprocessorUtils::parseCoord(args, 'S');
    if (!qIsNaN(spindleSpeed))
    {
        mLastSpindleSpeed = spindleSpeed;
    }
}

PointSegment * GcodeParser::handleGCode(float code, const QStringList &args)
{
    PointSegment *ps = nullptr;

    QVector3D nextPoint = GcodePreprocessorUtils::updatePointWithCommand(args, mCurrentPoint, mInAbsoluteMode);

    if (code == 0) ps = addLinearPointSegment(nextPoint, true);
    else if (code == 1.0f) ps = addLinearPointSegment(nextPoint, false);
    else if (code == 38.2f) ps = addLinearPointSegment(nextPoint, false);
    else if (code == 2.0f) ps = addArcPointSegment(nextPoint, true, args);
    else if (code == 3.0f) ps = addArcPointSegment(nextPoint, false, args);
    else if (code == 17.0f) mCurrentPlane = PointSegment::XY;
    else if (code == 18.0f) mCurrentPlane = PointSegment::ZX;
    else if (code == 19.0f) mCurrentPlane = PointSegment::YZ;
    else if (code == 20.0f) mIsMetric = false;
    else if (code == 21.0f) mIsMetric = true;
    else if (code == 90.0f) mInAbsoluteMode = true;
    else if (code == 90.1f) mInAbsoluteIJKMode = true;
    else if (code == 91.0f) mInAbsoluteMode = false;
    else if (code == 91.1f) mInAbsoluteIJKMode = false;

    if (code == 0 || code == 1 || code == 2 || code == 3 || code == 38.2)
    {
        mLastGcodeCommand = code;
    }

    return ps;
}

QStringList GcodeParser::preprocessCommands(QStringList commands)
{

    QStringList result;

    foreach (QString command, commands)
    {
        result.append(preprocessCommand(command));
    }

    return result;
}

QStringList GcodeParser::preprocessCommand(QString command)
{

    QStringList result;
    bool hasComment = false;

    // Remove comments from command.
    QStringList commandAndComment = GcodePreprocessorUtils::removeComment(command);
    QString newCommand = commandAndComment.at(0);
    QString rawCommand = newCommand;
    hasComment = (newCommand.length() != command.length());

    if (mRemoveAllWhitespace)
    {
        newCommand = GcodePreprocessorUtils::removeAllWhitespace(newCommand);
    }

    if (newCommand.length() > 0)
    {

        // Override feed speed
        if (mSpeedOverride > 0)
        {
            newCommand = GcodePreprocessorUtils::overrideSpeed(newCommand, mSpeedOverride);
        }

        if (mTruncateDecimalLength > 0)
        {
            newCommand = GcodePreprocessorUtils::truncateDecimals(mTruncateDecimalLength, newCommand);
        }

        // If this is enabled we need to parse the gcode as we go along.
        if (mConvertArcsToLines)
        { // || this.expandCannedCycles) {
            QStringList arcLines = convertArcsToLines(newCommand);
            if (arcLines.length() > 0)
            {
                result.append(arcLines);
            }
            else
            {
                result.append(newCommand);
            }
        }
        else if (hasComment)
        {
            // Maintain line level comment.
            result.append(command.replace(rawCommand, newCommand));
        }
        else
        {
            result.append(newCommand);
        }
    }
    else if (hasComment)
    {
        // Reinsert comment-only lines.
        result.append(command);
    }

    return result;
}

QStringList GcodeParser::convertArcsToLines(QString command)
{

    QStringList result;

    QVector3D start = mCurrentPoint;

    PointSegment *ps = addCommand(command);

    if (ps == nullptr || !ps->isArc())
    {
        return result;
    }

    QList<PointSegment*> psl = expandArc();

    if (psl.length() == 0)
    {
        return result;
    }

    // Create an array of new commands out of the of the segments in psl.
    // Don't add them to the gcode parser since it is who expanded them.
    foreach (PointSegment* segment, psl)
    {
        //Point3d end = segment.point();
        QVector3D end = *segment->point();
        result.append(GcodePreprocessorUtils::generateG1FromPoints(start, end, mInAbsoluteMode, mTruncateDecimalLength));
        start = *segment->point();
    }

    return result;

}
