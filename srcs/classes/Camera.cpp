#include "Camera.hpp"
#include <iostream>

Camera::Camera(const glm::vec3 &position, const glm::vec3 &upDirection, const float &yaw, const float &pitch, const float &roll, const float &fov, const float &speed)
{
    this->position = position;
    this->frontDirection = glm::vec3(1.0f);
    this->rightDirection = glm::vec3(1.0f);
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

Camera& Camera::operator=(const Camera &copy)
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

void Camera::setYaw(const float &yaw)
{
    this->yaw = yaw;
}

void Camera::setPitch(const float &pitch)
{
    this->pitch = pitch;
}

void Camera::setRoll(const float &roll)
{
    this->roll = roll;
}

void Camera::setFov(const float &fov)
{
    this->fov = fov;
}

void Camera::setSpeed(const float &speed)
{
    this->speed = speed;
}

void Camera::addToPosition(const glm::vec3 &position)
{
    this->position += position;
}

void Camera::addToYaw(const float &yaw)
{
    this->yaw += yaw;
}

void Camera::addToPitch(const float &pitch)
{
    this->pitch += pitch;
}

void Camera::addToFov(const float &fov)
{
    this->fov += fov;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    const float sensitivity = 0.1f;

    static float lastX = 400;
    static float lastY = 300;
    static bool firstIteration = true;

    float xOffset;
    float yOffset;

    Camera *camera = reinterpret_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (firstIteration)
    {
        lastX = xPos;
        lastY = yPos;
        firstIteration = false;
    }
    xOffset = (xPos - lastX) * sensitivity;
    yOffset = (lastY - yPos) * sensitivity;
    lastX = xPos;
    lastY = yPos;
    camera->addToYaw(xOffset);
   camera->addToPitch(yOffset);
    if (camera->getPitch() > 89.0f)
        camera->setPitch(89.0f);
    else if (camera->getPitch() < -89.0f)
        camera->setPitch(-89.0f);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
{
    (void)xOffset;

    Camera *camera = reinterpret_cast<Camera *>(glfwGetWindowUserPointer(window));
    camera->addToFov((float)-yOffset);
    if (camera->getFov() > 1.0f)
        camera->setFov(1.0f);
    else if (camera->getFov() < -45.0f)
        camera->setFov(-45.0f);
}