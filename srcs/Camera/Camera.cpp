#include "Camera/Camera.hpp"

Camera::Camera()
{
    position = {0.0f, 0.0f, 3.0f};
    frontDirection = glm::vec3(1.0f);
    rightDirection = glm::vec3(1.0f);
    upDirection = {0.0f, 1.0f, 0.0f};

    yaw = -90.0f;
    pitch = 0.0f;
    roll = 0.0f;
    fov = 45.0f;
    speed = 2.5f;
}

Camera::Camera(const glm::vec3 &position, const glm::vec3 &upDirection, float yaw, float pitch, float roll, float fov,
               float speed)
{
    this->position = position;
    frontDirection = glm::vec3(1.0f);
    rightDirection = glm::vec3(1.0f);
    this->upDirection = upDirection;
    this->yaw = yaw;
    this->pitch = pitch;
    this->roll = roll;
    this->fov = fov;
    this->speed = speed;
}

Camera::Camera(const Camera &copy)
{
    *this = copy;
}

Camera &Camera::operator=(const Camera &copy)
{
    if (&copy != this)
    {
        position = copy.getPosition();
        frontDirection = copy.getFrontDirection();
        rightDirection = copy.getRightDirection();
        upDirection = copy.getUpDirection();
        yaw = copy.getYaw();
        pitch = copy.getPitch();
        roll = copy.getRoll();
        fov = copy.getFov();
        speed = copy.getSpeed();
    }
    return (*this);
}

Camera::~Camera()
{
}

glm::vec3 Camera::getPosition() const
{
    return (position);
}

glm::vec3 Camera::getFrontDirection() const
{
    return (frontDirection);
}

glm::vec3 Camera::getRightDirection() const
{
    return (rightDirection);
}

glm::vec3 Camera::getUpDirection() const
{
    return (upDirection);
}

float Camera::getYaw() const
{
    return (yaw);
}

float Camera::getPitch() const
{
    return (pitch);
}
float Camera::getRoll() const
{
    return (roll);
}

float Camera::getFov() const
{
    return (fov);
}
float Camera::getSpeed() const
{
    return (speed);
}

void Camera::setPosition(const glm::vec3 &position)
{
    this->position = position;
}
void Camera::setFrontDirection(const glm::vec3 &frontDirection)
{
    this->frontDirection = frontDirection;
}

void Camera::setRightDirection(const glm::vec3 &rightDirection)
{
    this->rightDirection = rightDirection;
}

void Camera::setUpDirection(const glm::vec3 &upDirection)
{
    this->upDirection = upDirection;
}

void Camera::setYaw(float yaw)
{
    this->yaw = yaw;
}

void Camera::setPitch(float pitch)
{
    this->pitch = pitch;
}

void Camera::setRoll(float roll)
{
    this->roll = roll;
}

void Camera::setFov(float fov)
{
    this->fov = fov;
}

void Camera::setSpeed(float speed)
{
    this->speed = speed;
}

void Camera::addToPosition(const glm::vec3 &position)
{
    this->position = this->position + position;
}

void Camera::addToYaw(float yaw)
{
    this->yaw += yaw;
}

void Camera::addToPitch(float pitch)
{
    this->pitch += pitch;
}

void Camera::addToFov(float fov)
{
    this->fov += fov;
}