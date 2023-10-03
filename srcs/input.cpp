#include "main.hpp"
#include "classes/Camera/Camera.hpp"
#include "classes/Time/Time.hpp"

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
void updateDisplayMode(GLFWwindow *window)
{
    static bool wireFrameMode = false;
    static bool keyEnable = true;

    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && keyEnable == true)
    {
        if (wireFrameMode == false)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        wireFrameMode = !wireFrameMode;
        keyEnable = false;
    }
    else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE)
        keyEnable = true;
}

void updateCamera(GLFWwindow *window)
{
    t_scene *scene = reinterpret_cast<t_scene *>(glfwGetWindowUserPointer(window));
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        scene->camera->addToPosition(scene->camera->getFrontDirection() * scene->camera->getSpeed() * Time::getDeltaTime());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        scene->camera->addToPosition(-1 * scene->camera->getFrontDirection() * scene->camera->getSpeed() * Time::getDeltaTime());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        scene->camera->addToPosition(-1 * scene->camera->getRightDirection() * scene->camera->getSpeed() * Time::getDeltaTime());
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        scene->camera->addToPosition(scene->camera->getRightDirection() * scene->camera->getSpeed() * Time::getDeltaTime());
}

void updateTextureMode(GLFWwindow *window)
{
    static bool keyEnable = true;

    t_scene *scene = reinterpret_cast<t_scene *>(glfwGetWindowUserPointer(window));
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && keyEnable == true)
    {
        scene->displayColor = !scene->displayColor;
        keyEnable = false;
    }
    else if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE)
        keyEnable = true;
}

void updateTexture(GLFWwindow *window)
{
    t_scene *scene = reinterpret_cast<t_scene *>(glfwGetWindowUserPointer(window));
    if (scene->displayColor == true && scene->mixValue > 0)
    {
        scene->mixValue -= Time::getDeltaTime();
        if (scene->mixValue < 0)
            scene->mixValue = 0;
    }
    else if (scene->displayColor == false && scene->mixValue < 1)
    {
        scene->mixValue += Time::getDeltaTime();
        if (scene->mixValue > 1)
            scene->mixValue = 1;
    }

}
/**
 * main function to check any input of the user.
*/
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    updateDisplayMode(window);
    updateTextureMode(window);
    updateCamera(window);
    updateTexture(window);
}


void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    const float sensitivity = 0.1f;

    static float lastX = 400;
    static float lastY = 300;
    static bool firstIteration = true;

    float xOffset;
    float yOffset;

    t_scene *scene = reinterpret_cast<t_scene *>(glfwGetWindowUserPointer(window));
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
    scene->camera->addToYaw(xOffset);
    scene->camera->addToPitch(yOffset);
    if (scene->camera->getPitch() > 89.0f)
        scene->camera->setPitch(89.0f);
    else if (scene->camera->getPitch() < -89.0f)
        scene->camera->setPitch(-89.0f);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
{
    (void)xOffset;
    
    t_scene *scene = reinterpret_cast<t_scene *>(glfwGetWindowUserPointer(window));
    scene->camera->addToFov((float)-yOffset);
    if (scene->camera->getFov() < 1.0f)
        scene->camera->setFov(1.0f);
    else if (scene->camera->getFov() > 45.0f)
        scene->camera->setFov(45.0f);
}