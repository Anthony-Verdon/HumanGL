#ifndef INIT_HPP
#define INIT_HPP

#include "../includes/glad/glad.h"
#include <GLFW/glfw3.h>

#include "classes/Utils/Utils.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void initGLFW();
void initOpenGL();
GLFWwindow *initWindow();

#endif