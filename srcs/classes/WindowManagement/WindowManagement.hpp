#pragma once

#include "../../../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class WindowManagement
{
  private:
    GLFWwindow *window;

    void start();
    void initGLFW();
    void initWindow();
    void initOpenGL();

    void updateLoop();
    void processInput(GLFWwindow *window);
    void mouse_callback(GLFWwindow *window, double xPos, double yPos);
    void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

  public:
    WindowManagement();
    ~WindowManagement();
};