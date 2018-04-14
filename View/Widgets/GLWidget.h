// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#ifndef GLES
#include <QGLWidget>
#else
#include <QOpenGLWidget>
#endif

#include <QMetaType>
#include <QTimer>
#include <QTime>
#include "View/Drawers/ShaderDrawable.h"

enum class ProjectionMode
{
    ORTHO,
    PERSPECTIVE
};

Q_DECLARE_METATYPE(ProjectionMode)

#ifdef GLES
class GLWidget : public QOpenGLWidget
#else
class GLWidget : public QGLWidget, protected QOpenGLFunctions
#endif
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget();


    void addDrawable(ShaderDrawable *drawable);
    void updateExtremes(const ShaderDrawable& drawable);
    void fitDrawable(const ShaderDrawable& drawable);

    bool antialiasing() const;
    void setAntialiasing(bool antialiasing);

    QTime spendTime() const;
    void setSpendTime(const QTime &spendTime);

    QTime estimatedTime() const;
    void setEstimatedTime(const QTime &estimatedTime);

    float lineWidth() const;
    void setLineWidth(float lineWidth);

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

    float pointSize() const;
    void setPointSize(float pointSize);

    bool vsync() const;
    void setVsync(bool vsync);
    void timerEvent(QTimerEvent *);

signals:
    void rotationChanged();
    void resized();

public slots:

private slots:
    void onFramesTimer();
    void viewAnimation();

private:
    float normalizeAngle(float angle);
    float calculateVolume(QVector3D size);
    void beginViewAnimation();
    void stopViewAnimation();

    ProjectionMode mProjectionMode;
    float mXRot, mYRot, mXLastRot, mYLastRot;
    float mXPan, mYPan, mXLastPan, mYLastPan;
    float mXLookAt, mYLookAt, mZLookAt;
    QPoint mLastPos;
    float mZoom;
    float mDistance;
    float mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mXSize, mYSize, mZSize;
    float mLineWidth;
    float mPointSize;
    float mNearPlane;
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
    float mXRotTarget, mYRotTarget;
    float mXRotStored, mYRotStored;
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

};

