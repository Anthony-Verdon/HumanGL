#pragma once

#include "vec/vec.hpp"

class Camera
{
  private:
    ml::vec3 position;
    ml::vec3 frontDirection;
    ml::vec3 rightDirection;
    ml::vec3 upDirection;
    float yaw;   // x-axis by euler
    float pitch; // y-axis by euler
    float roll;  // z-axis by euler
    float fov;
    float speed;

  public:
    Camera();
    Camera(const ml::vec3 &position, const ml::vec3 &upDirection, float yaw, float pitch, float roll, float fov,
           float speed);
    Camera(const Camera &copy);
    Camera &operator=(const Camera &copy);
    ~Camera();

    ml::vec3 getPosition() const;
    ml::vec3 getFrontDirection() const;
    ml::vec3 getRightDirection() const;
    ml::vec3 getUpDirection() const;
    float getYaw() const;
    float getPitch() const;
    float getRoll() const;
    float getFov() const;
    float getSpeed() const;

    void setPosition(const ml::vec3 &position);
    void setFrontDirection(const ml::vec3 &frontDirection);
    void setRightDirection(const ml::vec3 &rightDirection);
    void setUpDirection(const ml::vec3 &upDirection);
    void setYaw(float yaw);
    void setPitch(float pitch);
    void setRoll(float roll);
    void setFov(float fov);
    void setSpeed(float speed);

    void addToPosition(const ml::vec3 &position);
    void addToYaw(float yaw);
    void addToPitch(float pitch);
    void addToFov(float fov);
};
