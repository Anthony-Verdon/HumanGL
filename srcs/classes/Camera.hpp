#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../../includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../../includes/glm/glm/glm.hpp"
#include "../../includes/glm/glm/gtc/matrix_transform.hpp"
#include "../../includes/glm/glm/gtc/type_ptr.hpp"

class Camera
{
    private:
        Camera();

        glm::vec3 position;
        glm::vec3 frontDirection;
        glm::vec3 rightDirection;
        glm::vec3 upDirection;
        float yaw; //x-axis by euler
        float pitch; //y-axis by euler
        float roll; //z-axis by euler
        float fov;
        float speed;

    public:
        Camera(const glm::vec3 &position, const glm::vec3 &upDirection, const float &yaw, const float &pitch, const float &roll, const float &fov, const float &speed);
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
        void setYaw(const float &yaw);
        void setPitch(const float &pitch);
        void setRoll(const float &roll);
        void setFov(const float &fov);
        void setSpeed(const float &speed);

        void addToPosition(const glm::vec3 &position);
        void addToYaw(const float &yaw);
        void addToPitch(const float &pitch);
        void addToFov(const float &fov);
};

void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

#endif
