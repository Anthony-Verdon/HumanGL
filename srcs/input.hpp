#ifndef INPUT_HPP
#define INPUT_HPP

#include <GLFW/glfw3.h>

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

#endif