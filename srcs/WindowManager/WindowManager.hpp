#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include "AProgram/AProgram.hpp"
#include <glm/glm.hpp>

class WindowManager
{
  private:
    static GLFWwindow *window;
    static glm::vec2 windowSize;
    static glm::vec2 mousePosition;

    WindowManager() = delete;
    ~WindowManager() = delete;

  public:
    static void InitWindow(const std::string &name, unsigned int width, unsigned int height);
    static void StartUpdateLoop(AProgram *game);
    static void StopUpdateLoop();
    static void DestructWindowManager();

    static bool IsKeyPressed(int key);
    static bool IsMouseButtonPressed(int mouseButton);
    
    static GLFWwindow *GetWindow();

    static glm::vec2 GetWindowSize();
    static unsigned int GetWindowWidth();
    static unsigned int GetWindowHeight();

    static glm::vec2 GetMousePosition();
    static void SetMousePosition(double xPos, double yPos);

    static void SetUserPointer(void *ptr);
    
    static void SetCharCallback(void (*func)(GLFWwindow *window, unsigned int character));
    static void SetScrollCallback(void (*func)(GLFWwindow *window, double xOffset, double yOffset));
};