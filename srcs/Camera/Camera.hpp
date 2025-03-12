#pragma once

#include <glm/glm.hpp>

class Camera
{
  private:
    glm::vec3 position;
    glm::vec3 frontDirection;
    glm::vec3 rightDirection;
    glm::vec3 upDirection;
    float yaw;   // x-axis by euler
    float pitch; // y-axis by euler
    float roll;  // z-axis by euler
    float fov;
    float speed;

  public:
    Camera();
    Camera(const glm::vec3 &position, const glm::vec3 &upDirection, float yaw, float pitch, float roll, float fov,
           float speed);
    Camera(const Camera &copy);
    Camera &operator=(const Camera &copy);
    ~Camera();

    glm::vec3 getPosition() const;
    glm::vec3 getFrontDirection() const;
    glm::vec3 getRightDirection() const;
    glm::vec3 getUpDirection() const;
    float getYaw() const;
    float getPitch() const;
    float getRoll() const;
    float getFov() const;
    float getSpeed() const;

    void setPosition(const glm::vec3 &position);
    void setFrontDirection(const glm::vec3 &frontDirection);
    void setRightDirection(const glm::vec3 &rightDirection);
    void setUpDirection(const glm::vec3 &upDirection);
    void setYaw(float yaw);
    void setPitch(float pitch);
    void setRoll(float roll);
    void setFov(float fov);
    void setSpeed(float speed);

    void addToPosition(const glm::vec3 &position);
    void addToYaw(float yaw);
    void addToPitch(float pitch);
    void addToFov(float fov);
};
