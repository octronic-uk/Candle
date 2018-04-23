#include "Camera.h"
#include <math.h>
#include <numeric>

// Constructor with vectors
Camera::Camera
(QVector3D translation, QVector3D up, float yaw, float pitch)
{
    mFront = QVector3D(0.0f, 0.0f, -1.0f);
    mMovementSpeed = 1.0f;
    mMouseSensitivity = 1.0f;
    mZoom = 1.0f;
    mTranslation = translation;
    mWorldUp = up;
    mYaw = yaw;
    mPitch = pitch;
    updateCameraVectors();
}

// Constructor with scalar values
Camera::Camera
(
        float posX, float posY, float posZ,
        float upX, float upY, float upZ,
        float yaw, float pitch
)
{
    mFront            = {0.0f, 0.0f, -1.0f};
    mMovementSpeed    = 1.0;
    mMouseSensitivity = 1.0;
    mZoom             = 1.0;
    mTranslation      = QVector3D(posX, posY, posZ);
    mWorldUp          = QVector3D(upX,  upY,  upZ);
    mYaw              = yaw;
    mPitch            = pitch;
    updateCameraVectors();
}

Camera::~Camera()
{
    qDebug() << "Camera: Destroying Object" << endl;
}

QMatrix4x4 Camera::getViewMatrix()
{
    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(mTranslation,mTranslation+mFront,mUp);
    return mViewMatrix;
}

void Camera::processKeyboard (int direction, float deltaTime)
{
    float velocity = mMovementSpeed * deltaTime;

    // Scroll up
    if (direction == MOVEMENT_FORWARD)
    {
        mTranslation += mFront * velocity;
    }

    // Scroll Down
    if (direction == MOVEMENT_BACKWARD)
    {
        mTranslation -= mFront * velocity;
    }

    // Pan Left
    if (direction == MOVEMENT_LEFT)
    {
        mTranslation -= mRight * velocity;
    }

    // Pan Right
    if (direction == MOVEMENT_RIGHT)
    {
        mTranslation += mRight * velocity;
    }
}

QVector3D
Camera::getTranslation
()
{
    return mTranslation;
}

QVector3D
Camera::getRotation
()
{
    return QVector3D (mPitch,mYaw,0.0f);
}

void
Camera::processMouseMovement
(float xoffset, float yoffset, bool constrainPitch)
{
    mYaw   += xoffset * mMouseSensitivity;
    mPitch -= yoffset * mMouseSensitivity;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (mPitch > PITCH_MAX)
        {
            mPitch = PITCH_MAX;
        }

        if (mPitch < -PITCH_MAX)
        {
            mPitch = -PITCH_MAX;
        }
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    updateCameraVectors();
}

void
Camera::processMouseScroll
(float yoffset)
{
    if (yoffset == 0.0f)
    {
        return;
    }

    if (mZoom >= 1.0f && mZoom <= 45.0f)
    {
        mZoom -= yoffset;
    }

    if (mZoom <= 1.0f )
    {
        mZoom = 1.0f;
    }

    if (mZoom >= 45.0f)
    {
        mZoom = 45.0f;
    }

    qDebug() << "Camera: Zoom is " << mZoom << endl;
}

void
Camera::updateCameraVectors
()
{
    //float adjustedYaw = mYaw + Constants::CAMERA_YAW_OFFSET;
    mFront.setX(static_cast<float>(cos(mYaw) * cos(mPitch)));
    mFront.setY(static_cast<float>(sin(mPitch)));
    mFront.setZ(static_cast<float>(sin(mYaw) * cos(mPitch)));

    mFront.normalize();

    mRight = QVector3D::crossProduct(mFront, mWorldUp);
    mRight.normalize();

    mUp = QVector3D::crossProduct(mRight, mFront);
    mUp.normalize();
}

void
Camera::setRotation
(QVector3D rotation)
{
    mPitch = rotation.x();
    mYaw = rotation.y();
    updateCameraVectors();
}

void
Camera::setTranslation
(QVector3D translation)
{
    mTranslation = translation;
}

void
Camera::setMovementSpeed
(float movementSpeed)
{
    mMovementSpeed = movementSpeed;
}

float
Camera::getMovementSpeed
()
{
    return mMovementSpeed;
}

void
Camera::setMouseSensitivity
(float mouseSensitivity)
{
    mMouseSensitivity = mouseSensitivity;
}

float
Camera::getMouseSensitivity
()
{
    return mMouseSensitivity;
}

float
Camera::getZoom
()
{
    return mZoom;
}

float
Camera::radians
(float degrees)
{
    return degrees * 0.01745329251994329576923690768489f;
}

const bool Camera::MOVEMENT_FORWARD = 1;
const bool Camera::MOVEMENT_BACKWARD = 2;
const bool Camera::MOVEMENT_LEFT = 3;
const bool Camera::MOVEMENT_RIGHT = 4;
const float Camera::PITCH_MIN = 1.0f;
const float Camera::PITCH_MAX = 45.0f;
