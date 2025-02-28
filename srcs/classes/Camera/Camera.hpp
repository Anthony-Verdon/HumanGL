#pragma once

#include "../Matrix/Matrix.hpp"

class Camera
{
  private:
    AlgOps::vec3 position;
    AlgOps::vec3 frontDirection;
    AlgOps::vec3 rightDirection;
    AlgOps::vec3 upDirection;
    float yaw;   // x-axis by euler
    float pitch; // y-axis by euler
    float roll;  // z-axis by euler
    float fov;
    float speed;

  public:
    Camera();
    Camera(const AlgOps::vec3 &position, const AlgOps::vec3 &upDirection, float yaw, float pitch, float roll, float fov,
           float speed);
    Camera(const Camera &copy);
    Camera &operator=(const Camera &copy);
    ~Camera();

    AlgOps::vec3 getPosition() const;
    AlgOps::vec3 getFrontDirection() const;
    AlgOps::vec3 getRightDirection() const;
    AlgOps::vec3 getUpDirection() const;
    float getYaw() const;
    float getPitch() const;
    float getRoll() const;
    float getFov() const;
    float getSpeed() const;

    void setPosition(const AlgOps::vec3 &position);
    void setFrontDirection(const AlgOps::vec3 &frontDirection);
    void setRightDirection(const AlgOps::vec3 &rightDirection);
    void setUpDirection(const AlgOps::vec3 &upDirection);
    void setYaw(float yaw);
    void setPitch(float pitch);
    void setRoll(float roll);
    void setFov(float fov);
    void setSpeed(float speed);

    void addToPosition(const AlgOps::vec3 &position);
    void addToYaw(float yaw);
    void addToPitch(float pitch);
    void addToFov(float fov);
};
