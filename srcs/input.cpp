#include "main.hpp"

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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    changeDisplayMode(glfwGetKey(window, GLFW_KEY_F1));
}