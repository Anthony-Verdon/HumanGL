#ifndef MAIN_HPP
#define MAIN_HPP

#include "../includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLFWwindow* initGLFW();
unsigned int createShaderProgram();
void processInput(GLFWwindow *window);

#endif