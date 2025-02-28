#include "Camera.hpp"

Camera::Camera()
{
    float positionValues[] = {0.0f, 0.0f, 3.0f};
    position.setData(positionValues, 3);
    frontDirection.uniform(1.0f);
    rightDirection.uniform(1.0f);
    float upDirectionValues[] = {0.0f, 1.0f, 0.0f};
    upDirection.setData(upDirectionValues, 3);

    yaw = -90.0f;
    pitch = 0.0f;
    roll = 0.0f;
    fov = 45.0f;
    speed = 2.5f;
}

Camera::Camera(const AlgOps::vec3 &position, const AlgOps::vec3 &upDirection, float yaw, float pitch, float roll, float fov,
               float speed)
{
    this->position = position;
    frontDirection.uniform(1.0f);
    rightDirection.uniform(1.0f);
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

AlgOps::vec3 Camera::getPosition() const
{
    return (position);
}

AlgOps::vec3 Camera::getFrontDirection() const
{
    return (frontDirection);
}

AlgOps::vec3 Camera::getRightDirection() const
{
    return (rightDirection);
}

AlgOps::vec3 Camera::getUpDirection() const
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

void Camera::setPosition(const AlgOps::vec3 &position)
{
    if (position.getRows() != 3 || position.getColumns() != 1)
        throw(Exception("SET_POSITION", "INVALID_SIZE", position));

    this->position = position;
}
void Camera::setFrontDirection(const AlgOps::vec3 &frontDirection)
{
    if (frontDirection.getRows() != 3 || frontDirection.getColumns() != 1)
        throw(Exception("SET_FRONT_DIRECTION", "INVALID_SIZE", frontDirection));

    this->frontDirection = frontDirection;
}

void Camera::setRightDirection(const AlgOps::vec3 &rightDirection)
{
    if (rightDirection.getRows() != 3 || rightDirection.getColumns() != 1)
        throw(Exception("SET_RIGHT_DIRECTION", "INVALID_SIZE", rightDirection));

    this->rightDirection = rightDirection;
}

void Camera::setUpDirection(const AlgOps::vec3 &upDirection)
{
    if (upDirection.getRows() != 3 || upDirection.getColumns() != 1)
        throw(Exception("SET_UP_DIRECTION", "INVALID_SIZE", upDirection));

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

void Camera::addToPosition(const AlgOps::vec3 &position)
{
    if (position.getRows() != 3 || position.getColumns() != 1)
        throw(Exception("ADD_TO_POSITION", "INVALID_SIZE", position));

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

Camera::Exception::Exception(const std::string &functionName, const std::string &errorMessage, const AlgOps::vec3 &position)
{
    this->errorMessage = "CAMERA::" + functionName + "::" + errorMessage;
    this->errorMessage +=
        "\n|\n| " + std::to_string(position.getRows()) + " * " + std::to_string(position.getColumns()) + "\n|";
    this->errorMessage += "\n| should be: 3 * 1\n|";
}

const char *Camera::Exception::what(void) const throw()
{
    return (errorMessage.c_str());
}