#include "main.hpp"

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

/**
 * main function to check any input of the user.
*/
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    changeDisplayMode(glfwGetKey(window, GLFW_KEY_F1));
}