#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Matrix/Matrix.hpp"
#include "../Utils/Utils.hpp"

class Camera
{
  private:
    Matrix position;
    Matrix frontDirection;
    Matrix rightDirection;
    Matrix upDirection;
    float yaw;   // x-axis by euler
    float pitch; // y-axis by euler
    float roll;  // z-axis by euler
    float fov;
    float speed;

  public:
    Camera();
    Camera(const Matrix &position, const Matrix &upDirection, float yaw, float pitch, float roll, float fov,
           float speed);
    Camera(const Camera &copy);
    Camera &operator=(const Camera &copy);
    ~Camera();

    Matrix getPosition() const;
    Matrix getFrontDirection() const;
    Matrix getRightDirection() const;
    Matrix getUpDirection() const;
    float getYaw() const;
    float getPitch() const;
    float getRoll() const;
    float getFov() const;
    float getSpeed() const;

    void setPosition(const Matrix &position);
    void setFrontDirection(const Matrix &frontDirection);
    void setRightDirection(const Matrix &rightDirection);
    void setUpDirection(const Matrix &upDirection);
    void setYaw(float yaw);
    void setPitch(float pitch);
    void setRoll(float roll);
    void setFov(float fov);
    void setSpeed(float speed);

    void addToPosition(const Matrix &position);
    void addToYaw(float yaw);
    void addToPitch(float pitch);
    void addToFov(float fov);

    class Exception : public std::exception
    {
      public:
        Exception(const std::string &functionName, const std::string &errorMessage, const Matrix &position);
        const char *what(void) const throw();

      private:
        std::string errorMessage;
    };
};

#endif
