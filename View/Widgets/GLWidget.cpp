// This file is a part of "CocoanutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GLWidget.h"
#include "View/Drawers/ToolDrawer.h"
#include <QDebug>
#include <QtWidgets>
#include <QPainter>
#include <QEasingCurve>
#include <QtDebug>

#ifdef GLES
#include <GLES/gl.h>
#endif

#define ZOOMSTEP 1.1

#ifdef GLES
GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      mShaderProgram(nullptr),
#else
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent),
      mShaderProgram(nullptr),
#endif
     mColorText(QColor("White")),
     mColorBackground(QColor("DarkGray")),
     mProjectionMode(ProjectionMode::PERSPECTIVE)
{
    mAnimateView = false;
    mUpdatesEnabled = false;

    mXRot = 90;
    mYRot = 0;

    mXRotTarget = 90;
    mYRotTarget = 0;

    mZoom = 1;

    mXPan = 0;
    mYPan = 0;
    m_distance = 100;

    mXLookAt = 0;
    mYLookAt = 0;
    mZLookAt = 0;

    mXMin = 0;
    mXMax = 0;
    mYMin = 0;
    mYMax = 0;
    mZMin = 0;
    mZMax = 0;
    mXSize = 0;
    mYSize = 0;
    mZSize = 0;

    updateProjection();
    updateView();

    mSpendTime.setHMS(0, 0, 0);
    mEstimatedTime.setHMS(0, 0, 0);

    mVsync = false;
    mTargetFps = 60;

    QTimer::singleShot(1000, this, SLOT(onFramesTimer()));
}

GLWidget::~GLWidget()
{
    if (mShaderProgram)
    {
        delete mShaderProgram;
    }
}

double GLWidget::calculateVolume(QVector3D size)
{
    return size.x() * size.y() * size.z();
}

void GLWidget::addDrawable(ShaderDrawable *drawable)
{
    mShaderDrawables.append(drawable);
}

void GLWidget::fitDrawable(ShaderDrawable *drawable)
{
    stopViewAnimation();

    if (drawable != NULL)
    {
        updateExtremes(drawable);

        double a = mYSize / 2 / 0.25 * 1.3
                + (mZMax - mZMin) / 2;
        double b = mXSize / 2 / 0.25 * 1.3
                / ((double)width() / height())
                + (mZMax - mZMin) / 2;
        m_distance = qMax(a, b);

        if (m_distance == 0) m_distance = 200;

        mXLookAt = (mXMax - mXMin) / 2 + mXMin;
        mZLookAt = -((mYMax - mYMin) / 2 + mYMin);
        mYLookAt = (mZMax - mZMin) / 2 + mZMin;
    }
    else
    {
        m_distance = 200;
        mXLookAt = 0;
        mYLookAt = 0;
        mZLookAt = 0;

        mXMin = 0;
        mXMax = 0;
        mYMin = 0;
        mYMax = 0;
        mZMin = 0;
        mZMax = 0;

        mXSize = 0;
        mYSize = 0;
        mZSize = 0;
    }

    mXPan = 0;
    mYPan = 0;
    mZoom = 1;

    updateProjection();
    updateView();
}

void GLWidget::updateExtremes(ShaderDrawable *drawable)
{
    if (!qIsNaN(drawable->getMinimumExtremes().x()))
    {
        mXMin = drawable->getMinimumExtremes().x();
    }
    else
    {
        mXMin = 0;
    }


    if (!qIsNaN(drawable->getMaximumExtremes().x()))
    {
        mXMax = drawable->getMaximumExtremes().x();
    }
    else
    {
        mXMax = 0;
    }

    if (!qIsNaN(drawable->getMinimumExtremes().y()))
    {
        mYMin = drawable->getMinimumExtremes().y();
    }
    else
    {
        mYMin = 0;
    }

    if (!qIsNaN(drawable->getMaximumExtremes().y()))
    {
        mYMax = drawable->getMaximumExtremes().y();
    }
    else
    {
        mYMax = 0;
    }

    if (!qIsNaN(drawable->getMinimumExtremes().z()))
    {
        mZMin = drawable->getMinimumExtremes().z();
    }
    else
    {
        mZMin = 0;
    }

    if (!qIsNaN(drawable->getMaximumExtremes().z()))
    {
        mZMax = drawable->getMaximumExtremes().z();
    }
    else
    {
        mZMax = 0;
    }

    mXSize = mXMax - mXMin;
    mYSize = mYMax - mYMin;
    mZSize = mZMax - mZMin;
}

bool GLWidget::antialiasing() const
{
    return mAntialiasing;
}

void GLWidget::setAntialiasing(bool antialiasing)
{
    mAntialiasing = antialiasing;
}

void GLWidget::onFramesTimer()
{
    mFps = mFrames;
    mFrames = 0;

    QTimer::singleShot(1000, this, SLOT(onFramesTimer()));
}

void GLWidget::viewAnimation()
{
    double t = (double)mAnimationFrame++ / (mFps * 0.2);

    if (t >= 1) stopViewAnimation();

    QEasingCurve ec(QEasingCurve::OutExpo);
    double val = ec.valueForProgress(t);

    mXRot = mXRotStored + double(mXRotTarget - mXRotStored) * val;
    mYRot = mYRotStored + double(mYRotTarget - mYRotStored) * val;

    updateView();
}

bool GLWidget::vsync() const
{
    return mVsync;
}

void GLWidget::setVsync(bool vsync)
{
    mVsync = vsync;
}

bool GLWidget::msaa() const
{
    return mMsaa;
}

void GLWidget::setMsaa(bool msaa)
{
    mMsaa = msaa;
}

bool GLWidget::updatesEnabled() const
{
    return mUpdatesEnabled;
}

void GLWidget::setUpdatesEnabled(bool updatesEnabled)
{
    mUpdatesEnabled = updatesEnabled;
}

bool GLWidget::zBuffer() const
{
    return mZBuffer;
}

void GLWidget::setZBuffer(bool zBuffer)
{
    mZBuffer = zBuffer;
}

QString GLWidget::bufferState() const
{
    return mBufferState;
}

void GLWidget::setBufferState(const QString &bufferState)
{
    mBufferState = bufferState;
}

QString GLWidget::parserStatus() const
{
    return mParserStatus;
}

void GLWidget::setParserStatus(const QString &parserStatus)
{
    mParserStatus = parserStatus;
}


double GLWidget::lineWidth() const
{
    return mLineWidth;
}

void GLWidget::setLineWidth(double lineWidth)
{
    mLineWidth = lineWidth;
}

void GLWidget::setTopView()
{
    mXRotTarget = 90;
    mYRotTarget = mYRot > 180 ? 360 : 0;
    beginViewAnimation();
}

void GLWidget::setFrontView()
{
    mXRotTarget = 0;
    mYRotTarget = mYRot > 180 ? 360 : 0;
    beginViewAnimation();
}

void GLWidget::setLeftView()
{
    mXRotTarget = 0;
    mYRotTarget = mYRot > 270 ? 450 : 90;
    beginViewAnimation();
}

int GLWidget::fps()
{
    return mTargetFps;
}

void GLWidget::setIsometricView()
{
    mXRotTarget = 45;
    mYRotTarget = mYRot > 180 ? 405 : 45;
    beginViewAnimation();
}

void GLWidget::beginViewAnimation()
{
    mXRotStored = mXRot;
    mYRotStored = mYRot;
    mAnimationFrame = 0;
    mAnimateView = true;
}

void GLWidget::stopViewAnimation()
{
    mAnimateView = false;
}
QColor GLWidget::colorText() const
{
    return mColorText;
}

void GLWidget::setColorText(const QColor &colorText)
{
    mColorText = colorText;
}

QColor GLWidget::colorBackground() const
{
    return mColorBackground;
}

void GLWidget::setColorBackground(const QColor &colorBackground)
{
    mColorBackground = colorBackground;
}


void GLWidget::setFps(int fps)
{
    if (fps <= 0) return;
    mTargetFps = fps;
    mTimerPaint.stop();
    mTimerPaint.start(mVsync ? 0 : 1000 / fps, Qt::PreciseTimer, this);
}

QTime GLWidget::estimatedTime() const
{
    return mEstimatedTime;
}

void GLWidget::setEstimatedTime(const QTime &estimatedTime)
{
    mEstimatedTime = estimatedTime;
}

QTime GLWidget::spendTime() const
{
    return mSpendTime;
}

void GLWidget::setSpendTime(const QTime &spendTime)
{
    mSpendTime = spendTime;
}

void GLWidget::initializeGL()
{
#ifndef GLES
    // Initialize functions
    initializeOpenGLFunctions();
#endif

    // Create shader program
    mShaderProgram = new QOpenGLShaderProgram();

    if (mShaderProgram)
    {
        // Compile vertex shader
        mShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vshader.glsl");
        // Compile fragment shader
        mShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/fshader.glsl");
        // Link shader pipeline
        mShaderProgram->link();
        qDebug() << "GLWidget: shader program created";
    }
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    updateProjection();
    emit resized();
}

void GLWidget::updateProjection()
{
    // Reset projection
    mProjectionMatrix.setToIdentity();

    double asp = (double)width() / height();
    ;

    switch (mProjectionMode)
    {
       case ProjectionMode::ORTHO:
            mProjectionMatrix.ortho(
                QRect(
                    QPoint((-0.5 + mXPan) * asp, (0.5 + mXPan) * asp),
                    QPoint(-0.5 + mYPan, 0.5 + mYPan)
                )
            );
            break;
        case ProjectionMode::PERSPECTIVE:
            mProjectionMatrix.frustum(
                (-0.5 + mXPan) * asp,
                (0.5 + mXPan) * asp,
                -0.5 + mYPan,
                0.5 + mYPan,
                2,
                m_distance * 2
            );
            break;
    }
}

void GLWidget::updateView()
{
    // Set view matrix
    mViewMatrix.setToIdentity();

    double r = m_distance;
    double angY = M_PI / 180 * mYRot;
    double angX = M_PI / 180 * mXRot;

    QVector3D eye(
        r * cos(angX) * sin(angY) + mXLookAt,
        r * sin(angX) + mYLookAt,
        r * cos(angX) * cos(angY) + mZLookAt
    );
    QVector3D center(mXLookAt, mYLookAt, mZLookAt);
    QVector3D up(
        fabs(mXRot) == 90 ?
        -sin(angY + (mXRot < 0 ? M_PI : 0)) :
        0,
        cos(angX), fabs(mXRot) == 90 ?
            -cos(angY + (mXRot < 0 ? M_PI :  0)) : 0
    );

    mViewMatrix.lookAt(eye, center, up.normalized());

    mViewMatrix.translate(mXLookAt, mYLookAt, mZLookAt);
    mViewMatrix.scale(mZoom, mZoom, mZoom);
    mViewMatrix.translate(-mXLookAt, -mYLookAt, -mZLookAt);

    mViewMatrix.rotate(-90, 1.0, 0.0, 0.0);
}

#ifdef GLES
void GLWidget::paintGL()
{
#else
void GLWidget::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe)
#endif
    QPainter painter(this);

    // Segment counter
    int vertices = 0;

    painter.beginNativePainting();

    // Clear viewport
    glClearColor(mColorBackground.redF(),
                 mColorBackground.greenF(),
                 mColorBackground.blueF(),
                 1.0
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Shader drawable points
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Update settings
    if (mAntialiasing)
    {
        if (mMsaa)
        {
            glEnable(GL_MULTISAMPLE);
        }
        else
        {
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_LINE_SMOOTH);
            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_POINT_SMOOTH);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
        }
    }
    if (mZBuffer)
    {
        glEnable(GL_DEPTH_TEST);
    }

    if (mShaderProgram)
    {
        // Draw 3d
        mShaderProgram->bind();

        // Set modelview-projection matrix
        mShaderProgram->setUniformValue("mvp_matrix", mProjectionMatrix * mViewMatrix);
        mShaderProgram->setUniformValue("mv_matrix", mViewMatrix);

        // Update geometries in current opengl context
        foreach (ShaderDrawable *drawable, mShaderDrawables)
            if (drawable->needsUpdateGeometry()) drawable->updateGeometry(mShaderProgram);

        // Draw geometries
        foreach (ShaderDrawable *drawable, mShaderDrawables)
        {
            drawable->draw(mShaderProgram);
            if (drawable->visible()) vertices += drawable->getVertexCount();
        }

        mShaderProgram->release();
    }
    else
    {
        qDebug() << "GLWidget: ShaderProgram is false??";
    }

    // Draw 2D
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);

    painter.endNativePainting();

    double x = 10;
    double y = height() - 60;

    QRect backBox(0,y-20, width(), height() - y + 20 );
    QColor backBoxColor = QColor("Black");
    backBoxColor.setAlphaF(0.5);
    painter.fillRect(backBox,backBoxColor) ;

    QPen pen(mColorText);
    painter.setPen(pen);

    painter.drawText(QPoint(x, y), QString("X: %1 ... %2").arg(mXMin, 0, 'f', 3).arg(mXMax, 0, 'f', 3));
    painter.drawText(QPoint(x, y + 15), QString("Y: %1 ... %2").arg(mYMin, 0, 'f', 3).arg(mYMax, 0, 'f', 3));
    painter.drawText(QPoint(x, y + 30), QString("Z: %1 ... %2").arg(mZMin, 0, 'f', 3).arg(mZMax, 0, 'f', 3));
    painter.drawText(QPoint(x, y + 45), QString("%1 / %2 / %3").arg(mXSize, 0, 'f', 3).arg(mYSize, 0, 'f', 3).arg(mZSize, 0, 'f', 3));

    QFontMetrics fm(painter.font());

    painter.drawText(QPoint(x, fm.height() + 10), mParserStatus);

    QString str = QString(tr("Vertices: %1")).arg(vertices);
    painter.drawText(QPoint(width() - fm.width(str) - 10, y + 30), str);
    str = QString("FPS: %1").arg(mFps);
    painter.drawText(QPoint(width() - fm.width(str) - 10, y + 45), str);

    str = mSpendTime.toString("hh:mm:ss") + " / " + mEstimatedTime.toString("hh:mm:ss");
    painter.drawText(QPoint(width() - fm.width(str) - 10, y), str);

    str = mBufferState;
    painter.drawText(QPoint(width() - fm.width(str) - 10, y + 15), str);

    mFrames++;

#ifdef GLES
    update();
#endif
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    mLastPos = event->pos();
    mXLastRot = mXRot;
    mYLastRot = mYRot;
    mXLastPan = mXPan;
    mYLastPan = mYPan;
    repaint();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if  (
            (
                event->buttons() & Qt::MiddleButton &&
                !(event->modifiers() & Qt::ShiftModifier)
            )
            ||
            event->buttons() & Qt::LeftButton
        )
    {

        stopViewAnimation();

        mYRot = normalizeAngle(mYLastRot - (event->pos().x() - mLastPos.x()) * 0.5);
        mXRot = mXLastRot + (event->pos().y() - mLastPos.y()) * 0.5;

        if (mXRot < -90) mXRot = -90;
        if (mXRot > 90) mXRot = 90;

        updateView();
        emit rotationChanged();
    }

    if  (
            (
                event->buttons() & Qt::MiddleButton &&
                event->modifiers() & Qt::ShiftModifier
            )
            ||
            event->buttons() & Qt::RightButton
         )
    {
        mXPan = mXLastPan - (event->pos().x() - mLastPos.x()) * 1 / (double)width();
        mYPan = mYLastPan + (event->pos().y() - mLastPos.y()) * 1 / (double)height();

        updateProjection();
    }
    repaint();
}

void GLWidget::wheelEvent(QWheelEvent *we)
{
    if (mZoom > 0.1 && we->delta() < 0)
    {
        mXPan -= ((double)we->pos().x() / width() - 0.5 + mXPan) * (1 - 1 / ZOOMSTEP);
        mYPan += ((double)we->pos().y() / height() - 0.5 - mYPan) * (1 - 1 / ZOOMSTEP);

        mZoom /= ZOOMSTEP;
    }
    else if (mZoom < 10 && we->delta() > 0)
    {
        mXPan -= ((double)we->pos().x() / width() - 0.5 + mXPan) * (1 - ZOOMSTEP);
        mYPan += ((double)we->pos().y() / height() - 0.5 - mYPan) * (1 - ZOOMSTEP);

        mZoom *= ZOOMSTEP;
    }

    updateProjection();
    updateView();
    repaint();
}

void GLWidget::timerEvent(QTimerEvent *te)
{
    //qDebug() << "GLWidget: Timer Event!";
    //if (te->timerId() == mTimerPaint.timerId())
    {
        if (mAnimateView) viewAnimation();
#ifndef GLES
        if (mUpdatesEnabled) update();
#endif
    }
   /* else
    {
#ifdef GLES
        QOpenGLWidget::timerEvent(te);
#else
        QGLWidget::timerEvent(te);
#endif
    }
    */
}

double GLWidget::normalizeAngle(double angle)
{
    while (angle < 0) angle += 360;
    while (angle > 360) angle -= 360;

    return angle;
}
