#ifndef MAIN_HPP
#define MAIN_HPP

#include "../includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "classes/Utils/Utils.hpp"
#include "classes/Camera/Camera.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct s_scene {
    Camera *camera;
    bool displayColor;
    float mixValue;
} t_scene;


void initGLFW();
void initOpenGL();
GLFWwindow *initWindow();
unsigned int createShaderProgram();
void processInput(GLFWwindow *window);

#endif