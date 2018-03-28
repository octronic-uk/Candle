// This file is a part of "Cocoanut" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#ifndef GLES
#include <QGLWidget>
#else
#include <QOpenGLWidget>
#endif

#include <QTimer>
#include <QTime>
#include "View/Drawers/ShaderDrawable.h"

#ifdef GLES
class GLWidget : public QOpenGLWidget
#else
class GLWidget : public QGLWidget, protected QOpenGLFunctions
#endif
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
    void addDrawable(ShaderDrawable *drawable);
    void updateExtremes(ShaderDrawable *drawable);
    void fitDrawable(ShaderDrawable *drawable = NULL);
    bool antialiasing() const;
    void setAntialiasing(bool antialiasing);

    QTime spendTime() const;
    void setSpendTime(const QTime &spendTime);

    QTime estimatedTime() const;
    void setEstimatedTime(const QTime &estimatedTime);

    double lineWidth() const;
    void setLineWidth(double lineWidth);

    void setIsometricView();
    void setTopView();
    void setFrontView();
    void setLeftView();

    int fps();
    void setFps(int fps);

    QString parserStatus() const;
    void setParserStatus(const QString &parserStatus);

    QString bufferState() const;
    void setBufferState(const QString &bufferState);

    bool zBuffer() const;
    void setZBuffer(bool zBuffer);

    bool updatesEnabled() const;
    void setUpdatesEnabled(bool updatesEnabled);

    bool msaa() const;
    void setMsaa(bool msaa);

    QColor colorBackground() const;
    void setColorBackground(const QColor &colorBackground);

    QColor colorText() const;
    void setColorText(const QColor &colorText);

    double pointSize() const;
    void setPointSize(double pointSize);

    bool vsync() const;
    void setVsync(bool vsync);

signals:
    void rotationChanged();
    void resized();

public slots:

private slots:
    void onFramesTimer();
    void viewAnimation();

private:
    double normalizeAngle(double angle);
    double calculateVolume(QVector3D size);
    void beginViewAnimation();
    void stopViewAnimation();

    double mXRot, mYRot, mXLastRot, mYLastRot;
    double mXPan, mYPan, mXLastPan, mYLastPan;
    double mXLookAt, mYLookAt, mZLookAt;
    QPoint mLastPos;
    double mZoom;
    double m_distance;
    double mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mXSize, mYSize, mZSize;
    double mLineWidth;
    double mPointSize;
    bool mAntialiasing;
    bool mMsaa;
    bool mVsync;
    bool mZBuffer;
    int mFrames = 0;
    int mFps = 0;
    int mTargetFps;
    int mAnimationFrame;
    QTime mSpendTime;
    QTime mEstimatedTime;
    QBasicTimer mTimerPaint;
    double mXRotTarget, mYRotTarget;
    double mXRotStored, mYRotStored;
    bool mAnimateView;
    QString mParserStatus;
    QString mBufferState;
    bool mUpdatesEnabled;
    QList<ShaderDrawable*> mShaderDrawables;
    QOpenGLShaderProgram *mShaderProgram;
    QMatrix4x4 mProjectionMatrix;
    QMatrix4x4 mViewMatrix;

    QColor mColorBackground;
    QColor mColorText;

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void updateProjection();
    void updateView();
#ifdef GLES
    void paintGL();
#else
    void paintEvent(QPaintEvent *pe);
#endif

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *we);

    void timerEvent(QTimerEvent *);
};

