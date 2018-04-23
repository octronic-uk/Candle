#pragma once

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
private:
    // Camera Attributes
    QVector3D mTranslation;
    QVector3D mFront;
    QVector3D mUp;
    QVector3D mRight;
    QVector3D mWorldUp;
    // Eular Angles
    float mYaw;
    float mPitch;
    // Camera options
    float mMovementSpeed;
    float mMouseSensitivity;
    float mZoom;
public:
    // Constructor with vectors
    Camera(
            QVector3D position = QVector3D(0.0f, 0.0f, 0.0f),
            QVector3D up = QVector3D(0.0f, 1.0f, 0.0f),
            float   yaw      = 0.0f,
            float   pitch    = 0.0f
            );

    // Constructor with scalar values
    Camera(float, float, float, float, float, float, float, float);
    ~Camera();
    QMatrix4x4 getViewMatrix();
    void processKeyboard(int, float);
    void processMouseMovement(float, float, bool);
    void processMouseScroll(float);
    void updateCameraVectors();
    void setTranslation(QVector3D);
    QVector3D getTranslation();
    void setRotation(QVector3D);
    QVector3D getRotation();
    void  setMovementSpeed(float);
    float getMovementSpeed();
    void  setMouseSensitivity(float);
    float getMouseSensitivity();
    float getZoom();
private:
    QMatrix4x4 mViewMatrix;
    float radians(float);
    const static bool MOVEMENT_FORWARD;
    const static bool MOVEMENT_BACKWARD;
    const static bool MOVEMENT_LEFT;
    const static bool MOVEMENT_RIGHT;
    const static float PITCH_MIN; // 1.0
    const static float PITCH_MAX; // 45.0

}; // End of Camera
