#include "main.hpp"
#include "classes/Camera.hpp"
#include "classes/Time.hpp"

/**
 * update the display mode of the drawing.
 * 
 * if the player press the key (action == GLFW_PRESS)
 * and if it's the first frame (keyEnable == true),
 * then it update the displayMode and disable the key
 * 
 * if the playe release the key
 * then it enable the key
*/
void changeDisplayMode(unsigned int action)
{
    static bool wireFrameMode = false;
    static bool keyEnable = true;

    if (action == GLFW_PRESS && keyEnable == true)
    {
        if (wireFrameMode == false)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        wireFrameMode = !wireFrameMode;
        keyEnable = false;
    }
    else if (action == GLFW_RELEASE)
        keyEnable = true;
}

void updateCamera(GLFWwindow *window)
{
    Camera *camera = reinterpret_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->addToPosition(camera->getFrontDirection() * camera->getSpeed() * Time::getDeltaTime());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->addToPosition(-camera->getFrontDirection() * camera->getSpeed() * Time::getDeltaTime());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->addToPosition(-camera->getRightDirection() * camera->getSpeed() * Time::getDeltaTime());
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->addToPosition(camera->getRightDirection() * camera->getSpeed() * Time::getDeltaTime());
}

/**
 * main function to check any input of the user.
*/
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    changeDisplayMode(glfwGetKey(window, GLFW_KEY_F1));
    updateCamera(window);
}