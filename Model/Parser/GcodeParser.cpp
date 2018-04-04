// This file is a part of "CocoanutCNC" application.
// This file was originally ported from "GcodeParser.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include <QListIterator>
#include <QDebug>
#include "GcodeParser.h"
#include <QMatrix4x4>

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
    QStringList stripped = removeComment(command);
    QStringList args = splitCommand(stripped.at(0));
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
            generatePointsAlongArcBDring(
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
    double speed = parseCoord(args, 'F');
    if (!qIsNaN(speed))
    {
        mLastSpeed = mIsMetric ? speed : speed * 25.4;
    }

    // Handle S code
    double spindleSpeed = parseCoord(args, 'S');
    if (!qIsNaN(spindleSpeed))
    {
        mLastSpindleSpeed = spindleSpeed;
    }

    // Handle P code
    double dwell = parseCoord(args, 'P');
    if (!qIsNaN(dwell))
    {
        mPoints.last()->setDwell(dwell);
    }

    // handle G codes.
    gCodes = parseCodes(args, 'G');

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

    QVector3D center = updateCenterWithCommand(args, mCurrentPoint, nextPoint, mInAbsoluteIJKMode, clockwise);
    double radius = parseCoord(args, 'R');

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
    double spindleSpeed = parseCoord(args, 'S');
    if (!qIsNaN(spindleSpeed))
    {
        mLastSpindleSpeed = spindleSpeed;
    }
}

PointSegment * GcodeParser::handleGCode(float code, const QStringList &args)
{
    PointSegment *ps = nullptr;

    QVector3D nextPoint = updatePointWithCommand(args, mCurrentPoint, mInAbsoluteMode);

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
    QStringList commandAndComment = removeComment(command);
    QString newCommand = commandAndComment.at(0);
    QString rawCommand = newCommand;
    hasComment = (newCommand.length() != command.length());

    if (mRemoveAllWhitespace)
    {
        newCommand = removeAllWhitespace(newCommand);
    }

    if (newCommand.length() > 0)
    {

        // Override feed speed
        if (mSpeedOverride > 0)
        {
            newCommand = overrideSpeed(newCommand, mSpeedOverride);
        }

        if (mTruncateDecimalLength > 0)
        {
            newCommand = truncateDecimals(mTruncateDecimalLength, newCommand);
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
        result.append(generateG1FromPoints(start, end, mInAbsoluteMode, mTruncateDecimalLength));
        start = *segment->point();
    }

    return result;

}

/**
* Searches the command string for an 'f' and replaces the speed value
* between the 'f' and the next space with a percentage of that speed.
* In that way all speed values become a ratio of the provided speed
* and don't get overridden with just a fixed speed.
*/
QString GcodeParser::overrideSpeed(QString command, double speed)
{
    static QRegExp re("[Ff]([0-9.]+)");

    if (re.indexIn(command) != -1)
    {
        double speedMagnitude = (speed/100); // from percentage to magnitude
        command.replace(re, QString("F%1").arg(re.cap(1).toDouble() * speedMagnitude));
    }
    return command;
}

/**
* Searches the command string for an 'f' and replaces the speed value
* between the 'f' and the next space with a percentage of that speed.
* In that way all speed values become a ratio of the provided speed
* and don't get overridden with just a fixed speed.
*/
GcodeCommand* GcodeParser::overrideSpeed(const GcodeCommand *command, double speed)
{
    QString cmd = command->getCommand();
    static QRegExp re("[Ff]([0-9.]+)");

    if (re.indexIn(cmd) != -1)
    {
        double speedMagnitude = (speed/100); // from percentage to magnitude
        cmd.replace(re, QString("F%1").arg(re.cap(1).toDouble() * speedMagnitude));
    }
    GcodeCommand* overriddenFR = new GcodeCommand(*command);
    overriddenFR->setCommand(cmd);
    return overriddenFR;
}

/**
* Removes any comments within parentheses or beginning with a semi-colon.
*/
QStringList GcodeParser::removeComment(QString command)
{
    QString command_trimmed, comment;

    static QRegExp rx1("\\(+[^\\(]*\\)+");
    static QRegExp rx2(";.*");

    // Remove any comments within ( parentheses ) using regex "\([^\(]*\)"
    if (command.contains('('))
    {
        comment = command.remove(rx1);
    }

    // Remove any comment beginning with ';' using regex ";.*"
    if (command.contains(';'))
    {
        comment = command.remove(rx2);
    }

    return QStringList() << command.trimmed() << comment;
}

/**
* Searches for a comment in the input string and returns the first match.
*/
QString GcodeParser::parseComment(QString command)
{
    // REGEX: Find any comment, includes the comment characters:
    // "(?<=\()[^\(\)]*|(?<=\;)[^;]*"
    // "(?<=\\()[^\\(\\)]*|(?<=\\;)[^;]*"

    static QRegExp re("(\\([^\\(\\)]*\\)|;[^;].*)");

    if (re.indexIn(command) != -1) {
        return re.cap(1);
    }
    return "";
}

QString GcodeParser::truncateDecimals(int length, QString command)
{
    static QRegExp re("(\\d*\\.\\d*)");
    int pos = 0;

    while ((pos = re.indexIn(command, pos)) != -1)
    {
        QString newNum = QString::number(re.cap(1).toDouble(), 'f', length);
        command = command.left(pos) + newNum + command.mid(pos + re.matchedLength());
        pos += newNum.length() + 1;
    }

    return command;
}

QString GcodeParser::removeAllWhitespace(QString command)
{
    static QRegExp rx("\\s");

    return command.remove(rx);
}

QList<float> GcodeParser::parseCodes(const QStringList &args, char code)
{
    QList<float> l;

    foreach (QString s, args) {
        if (s.length() > 0 && s[0].toUpper() == code) l.append(s.mid(1).toDouble());
    }

    return l;
}

QList<int> GcodeParser::parseGCodes(QString command)
{
    static QRegExp re("[Gg]0*(\\d+)");

    QList<int> codes;
    int pos = 0;

    while ((pos = re.indexIn(command, pos)) != -1) {
        codes.append(re.cap(1).toInt());
        pos += re.matchedLength();
    }

    return codes;
}

QList<int> GcodeParser::parseMCodes(QString command)
{
    static QRegExp re("[Mm]0*(\\d+)");

    QList<int> codes;
    int pos = 0;

    while ((pos = re.indexIn(command, pos)) != -1) {
        codes.append(re.cap(1).toInt());
        pos += re.matchedLength();
    }

    return codes;
}

/**
* Update a point given the arguments of a command.
*/
QVector3D GcodeParser::updatePointWithCommand(const QString &command, const QVector3D &initial, bool absoluteMode)
{
    QStringList l = splitCommand(command);
    return updatePointWithCommand(l, initial, absoluteMode);
}

/**
* Update a point given the arguments of a command, using a pre-parsed list.
*/
QVector3D GcodeParser::updatePointWithCommand(const QStringList &commandArgs, const QVector3D &initial,
                                                         bool absoluteMode)
{
    double x = qQNaN();
    double y = qQNaN();
    double z = qQNaN();
    char c;

    for (int i = 0; i < commandArgs.length(); i++)
    {
        if (commandArgs.at(i).length() > 0)
        {
            c = commandArgs.at(i).at(0).toUpper().toLatin1();
            switch (c)
            {
            case 'X':
                x = commandArgs.at(i).mid(1).toDouble();;
                break;
            case 'Y':
                y = commandArgs.at(i).mid(1).toDouble();;
                break;
            case 'Z':
                z = commandArgs.at(i).mid(1).toDouble();;
                break;
            }
        }
    }

    return updatePointWithCommand(initial, x, y, z, absoluteMode);
}

/**
* Update a point given the new coordinates.
*/
QVector3D GcodeParser::updatePointWithCommand(const QVector3D &initial, double x, double y, double z, bool absoluteMode)
{
    QVector3D newPoint(initial.x(), initial.y(), initial.z());

    if (absoluteMode)
    {
        if (!qIsNaN(x)) newPoint.setX(x);
        if (!qIsNaN(y)) newPoint.setY(y);
        if (!qIsNaN(z)) newPoint.setZ(z);
    }
    else
    {
        if (!qIsNaN(x)) newPoint.setX(newPoint.x() + x);
        if (!qIsNaN(y)) newPoint.setY(newPoint.y() + y);
        if (!qIsNaN(z)) newPoint.setZ(newPoint.z() + z);
    }

    return newPoint;
}

QVector3D GcodeParser::updateCenterWithCommand(QStringList commandArgs, QVector3D initial, QVector3D nextPoint, bool absoluteIJKMode, bool clockwise)
{
    double i = qQNaN();
    double j = qQNaN();
    double k = qQNaN();
    double r = qQNaN();
    char c;

    foreach (QString t, commandArgs)
    {
        if (t.length() > 0)
        {
            c = t[0].toUpper().toLatin1();
            switch (c)
            {
            case 'I':
                i = t.mid(1).toDouble();
                break;
            case 'J':
                j = t.mid(1).toDouble();
                break;
            case 'K':
                k = t.mid(1).toDouble();
                break;
            case 'R':
                r = t.mid(1).toDouble();
                break;
            }
        }
    }

    if (qIsNaN(i) && qIsNaN(j) && qIsNaN(k)) {
        return convertRToCenter(initial, nextPoint, r, absoluteIJKMode, clockwise);
    }

    return updatePointWithCommand(initial, i, j, k, absoluteIJKMode);
}

QString GcodeParser::generateG1FromPoints(QVector3D start, QVector3D end, bool absoluteMode, int precision)
{
    QString sb("G1");

    if (absoluteMode)
    {
        if (!qIsNaN(end.x())) sb.append("X" + QString::number(end.x(), 'f', precision));
        if (!qIsNaN(end.y())) sb.append("Y" + QString::number(end.y(), 'f', precision));
        if (!qIsNaN(end.z())) sb.append("Z" + QString::number(end.z(), 'f', precision));
    }
    else
    {
        if (!qIsNaN(end.x())) sb.append("X" + QString::number(end.x() - start.x(), 'f', precision));
        if (!qIsNaN(end.y())) sb.append("Y" + QString::number(end.y() - start.y(), 'f', precision));
        if (!qIsNaN(end.z())) sb.append("Z" + QString::number(end.z() - start.z(), 'f', precision));
    }

    return sb;
}

/**
* Splits a gcode command by each word/argument, doesn't care about spaces.
* This command is about the same speed as the string.split(" ") command,
* but might be a little faster using precompiled regex.
*/
QStringList GcodeParser::splitCommand(const QString &command)
{
    QStringList l;
    bool readNumeric = false;
    QString sb;

    QByteArray ba(command.toLatin1());
    const char *cmd = ba.constData(); // Direct access to string data
    char c;

    for (int i = 0; i < command.length(); i++)
    {
        c = cmd[i];

        if (readNumeric && !isDigit(c) && c != '.')
        {
            readNumeric = false;
            l.append(sb);
            sb.clear();
            if (isLetter(c)) sb.append(c);
        }
        else if (isDigit(c) || c == '.' || c == '-')
        {
            sb.append(c);
            readNumeric = true;
        }
        else if (isLetter(c))
        {
            sb.append(c);
        }
    }

    if (sb.length() > 0)
    {
        l.append(sb);
    }

//    QChar c;

//    for (int i = 0; i < command.length(); i++) {
//        c = command[i];

//        if (readNumeric && !c.isDigit() && c != '.') {
//            readNumeric = false;
//            l.append(sb);
//            sb = "";
//            if (c.isLetter()) sb.append(c);
//        } else if (c.isDigit() || c == '.' || c == '-') {
//            sb.append(c);
//            readNumeric = true;
//        } else if (c.isLetter()) sb.append(c);
//    }

//    if (sb.length() > 0) l.append(sb);

    return l;
}

// TODO: Replace everything that uses this with a loop that loops through
// the string and creates a hash with all the values.
double GcodeParser::parseCoord(QStringList argList, char c)
{
//    int n = argList.length();

//    for (int i = 0; i < n; i++) {
//        if (argList[i].length() > 0 && argList[i][0].toUpper() == c) return argList[i].mid(1).toDouble();
//    }

    foreach (QString t, argList)
    {
        if (t.length() > 0 && t[0].toUpper() == c) return t.mid(1).toDouble();
    }
    return qQNaN();
}

//static public List<String> convertArcsToLines(Point3d start, Point3d end) {
//    List<String> l = new ArrayList<String>();

//    return l;
//}

QVector3D GcodeParser::convertRToCenter(QVector3D start, QVector3D end, double rad, bool absoluteIJK, bool clockwise)
{
    double radius = rad;
    QVector3D center;

    double x = end.x() - start.x();
    double y = end.y() - start.y();

    double height_x2_div_diameter = 4 * radius * radius - x * x - y * y;
    if (height_x2_div_diameter < 0)
    {
        qDebug() << "Error computing arc radius.";
    }
    height_x2_div_diameter = (-sqrt(height_x2_div_diameter)) / hypot(x, y);

    if (!clockwise)
    {
        height_x2_div_diameter = -height_x2_div_diameter;
    }

    // Special message from gcoder to software for which radius
    // should be used.
    if (radius < 0)
    {
        height_x2_div_diameter = -height_x2_div_diameter;
        // TODO: Places that use this need to run ABS on radius.
        radius = -radius;
    }

    double offsetX = 0.5 * (x - (y * height_x2_div_diameter));
    double offsetY = 0.5 * (y + (x * height_x2_div_diameter));

    if (!absoluteIJK)
    {
        center.setX(start.x() + offsetX);
        center.setY(start.y() + offsetY);
    }
    else
    {
        center.setX(offsetX);
        center.setY(offsetY);
    }

    return center;
}

/**
* Return the angle in radians when going from start to end.
*/
double GcodeParser::getAngle(QVector3D start, QVector3D end)
{
    double deltaX = end.x() - start.x();
    double deltaY = end.y() - start.y();

    double angle = 0.0;

    if (deltaX != 0)
    { // prevent div by 0
        // it helps to know what quadrant you are in
        if (deltaX > 0 && deltaY >= 0)
        { // 0 - 90
            angle = atan(deltaY / deltaX);
        }
        else if (deltaX < 0 && deltaY >= 0)
        { // 90 to 180
            angle = M_PI - fabs(atan(deltaY / deltaX));
        }
        else if (deltaX < 0 && deltaY < 0)
        { // 180 - 270
            angle = M_PI + fabs(atan(deltaY / deltaX));
        }
        else if (deltaX > 0 && deltaY < 0)
        { // 270 - 360
            angle = M_PI * 2 - fabs(atan(deltaY / deltaX));
        }
    }
    else
    {
        // 90 deg
        if (deltaY > 0)
        {
            angle = M_PI / 2.0;
        }
        // 270 deg
        else
        {
            angle = M_PI * 3.0 / 2.0;
        }
    }

    return angle;
}

double GcodeParser::calculateSweep(double startAngle, double endAngle, bool isCw)
{
    double sweep;

    // Full circle
    if (startAngle == endAngle)
    {
        sweep = (M_PI * 2);
        // Arcs
    }
    else
    {
        // Account for full circles and end angles of 0/360
        if (endAngle == 0)
        {
            endAngle = M_PI * 2;
        }
        // Calculate distance along arc.
        if (!isCw && endAngle < startAngle)
        {
            sweep = ((M_PI * 2 - startAngle) + endAngle);
        }
        else if (isCw && endAngle > startAngle)
        {
            sweep = ((M_PI * 2 - endAngle) + startAngle);
        }
        else
        {
            sweep = fabs(endAngle - startAngle);
        }
    }

    return sweep;
}

/**
* Generates the points along an arc including the start and end points.
*/
QList<QVector3D>
GcodeParser::generatePointsAlongArcBDring
(
        PointSegment::planes plane, QVector3D start, QVector3D end,
        QVector3D center, bool clockwise, double rad, double minArcLength,
        double arcPrecision, bool arcDegreeMode
){
    double radius = rad;
    // Rotate vectors according to plane
    QMatrix4x4 m;
    m.setToIdentity();
    switch (plane)
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
    start = m * start;
    end = m * end;
    center = m * center;

    // Check center
    if (qIsNaN(center.length()))
    {
        return QList<QVector3D>();
    }

    // Calculate radius if necessary.
    if (radius == 0)
    {
        radius = sqrt(pow((double)(start.x() - center.x()), 2.0) + pow((double)(end.y() - center.y()), 2.0));
    }

    double startAngle = getAngle(center, start);
    double endAngle = getAngle(center, end);
    double sweep = calculateSweep(startAngle, endAngle, clockwise);

    // Convert units.
    double arcLength = sweep * radius;

    // If this arc doesn't meet the minimum threshold, don't expand.
//    if (minArcLength > 0 && arcLength < minArcLength) {
//        QList<QVector3D> empty;
//        return empty;
//    }

    int numPoints;

    if (arcDegreeMode && arcPrecision > 0)
    {
        numPoints = qMax(1.0, sweep / (M_PI * arcPrecision / 180));
    }
    else
    {
        if (arcPrecision <= 0 && minArcLength > 0)
        {
            arcPrecision = minArcLength;
        }
        numPoints = (int)ceil(arcLength/arcPrecision);
    }

    return generatePointsAlongArcBDring(
        plane, start, end, center, clockwise,
        radius, startAngle, sweep, numPoints
    );
}

/**
* Generates the points along an arc including the start and end points.
*/
QList<QVector3D>
GcodeParser::generatePointsAlongArcBDring
(
    PointSegment::planes plane, QVector3D p1, QVector3D p2,
    QVector3D center, bool isCw, double radius, double startAngle,
    double sweep, int numPoints
){
    // Prepare rotation matrix to restore plane
    QMatrix4x4 m;
    m.setToIdentity();
    switch (plane)
    {
    case PointSegment::XY:
        break;
    case PointSegment::ZX:
        m.rotate(-90, 1.0, 0.0, 0.0);
        break;
    case PointSegment::YZ:
        m.rotate(90, 0.0, 1.0, 0.0);
        break;
    }

    QVector3D lineEnd(p2.x(), p2.y(), p1.z());
    QList<QVector3D> segments;
    double angle;

    // Calculate radius if necessary.
    if (radius == 0)
    {
        radius = sqrt(pow((double)(p1.x() - center.x()), 2.0) + pow((double)(p1.y() - center.y()), 2.0));
    }

    double zIncrement = (p2.z() - p1.z()) / numPoints;
    for (int i = 1; i < numPoints; i++)
    {
        if (isCw)
        {
            angle = (startAngle - i * sweep / numPoints);
        }
        else
        {
            angle = (startAngle + i * sweep / numPoints);
        }

        if (angle >= M_PI * 2)
        {
            angle = angle - M_PI * 2;
        }

        lineEnd.setX(cos(angle) * radius + center.x());
        lineEnd.setY(sin(angle) * radius + center.y());
        lineEnd.setZ(lineEnd.z() + zIncrement);

        segments.append(m * lineEnd);
    }

    segments.append(m * p2);

    return segments;
}

bool GcodeParser::isDigit(char c)
{
    return c > 47 && c < 58;
}

bool GcodeParser::isLetter(char c)
{
    return (c > 64 && c < 91) || (c > 96 && c < 123);
}

char GcodeParser::toUpper(char c)
{
    return (c > 96 && c < 123) ? c - 32 : c;
}
