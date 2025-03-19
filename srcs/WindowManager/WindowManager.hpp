#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include "AProgram/AProgram.hpp"
#include "vec/vec.hpp"

class WindowManager
{
  private:
    static GLFWwindow *window;
    static ml::vec2 windowSize;
    static ml::vec2 mousePosition;

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

    static ml::vec2 GetWindowSize();
    static unsigned int GetWindowWidth();
    static unsigned int GetWindowHeight();

    static void SetInputMode(int mode, int value);
    static int GetInputMode(int mode);

    static ml::vec2 GetMousePosition();
    static void SetMousePosition(double xPos, double yPos);

    static void SetUserPointer(void *ptr);
    
    static void SetCharCallback(void (*func)(GLFWwindow *window, unsigned int character));
    static void SetScrollCallback(void (*func)(GLFWwindow *window, double xOffset, double yOffset));
};