#ifndef INPUT_HPP
#define INPUT_HPP

#include "../includes/glad/glad.h"
#include <GLFW/glfw3.h>

#include "classes/Camera/Camera.hpp"
#include "classes/Time/Time.hpp"
#include "scene.hpp"

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

#endif