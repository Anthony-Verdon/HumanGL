#include <glad/glad.h>
#include "WindowManager/WindowManager.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "globals.hpp"

GLFWwindow *WindowManager::window = NULL;
ml::vec2 WindowManager::mousePosition;
ml::vec2 WindowManager::windowSize;

void mouse_callback(GLFWwindow *window, double xPos, double yPos);

void WindowManager::InitWindow(const std::string &name, unsigned int width, unsigned int height)
{
    if (glfwInit() == GL_FALSE)
        throw(std::runtime_error("INIT_GLFW::INITIALIZATION_FAILED"));

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef FULL_SCREEN
    window = glfwCreateWindow(width, height, name.c_str(), glfwGetPrimaryMonitor(), NULL);
#else
    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
#endif

    if (!window)
        throw(std::runtime_error("INIT_WINDOW::INITIALIZATION_FAILED"));
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw(std::runtime_error("INIT_OPENGL::INITIALIZATION_FAILED"));

    int viewPortWidth, viewportHeight;
    glfwGetFramebufferSize(window, &viewPortWidth, &viewportHeight);
    glViewport(0, 0, viewPortWidth, viewportHeight);

    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPosCallback(window, mouse_callback);

    windowSize = {(float)width, (float)height};
}

void WindowManager::DestructWindowManager()
{
    glfwTerminate();
}

void WindowManager::StartUpdateLoop(AProgram *game)
{
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        game->Run();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void WindowManager::StopUpdateLoop()
{
    glfwSetWindowShouldClose(window, true);
}

bool WindowManager::IsKeyPressed(int key)
{
    return (glfwGetKey(window, key) == GLFW_PRESS);
}

bool WindowManager::IsMouseButtonPressed(int mouseButton)
{
    return (glfwGetMouseButton(window, mouseButton) == GLFW_PRESS);
}

GLFWwindow* WindowManager::GetWindow()
{
    return (window);
}

ml::vec2 WindowManager::GetWindowSize()
{
    return (windowSize);
}

unsigned int WindowManager::GetWindowWidth()
{
    return (windowSize.x);
}

unsigned int WindowManager::GetWindowHeight()
{
    return (windowSize.y);
}

void WindowManager::SetInputMode(int mode, int value)
{
    glfwSetInputMode(window, mode, value);
}

int WindowManager::GetInputMode(int mode)
{
    return (glfwGetInputMode(window, mode));
}

void WindowManager::SetMousePosition(double xPos, double yPos)
{
    mousePosition = {(float)xPos, (float)yPos};
}

ml::vec2 WindowManager::GetMousePosition()
{
    return (mousePosition);
}

void WindowManager::SetUserPointer(void *ptr)
{
    glfwSetWindowUserPointer(window, ptr);
}

void WindowManager::SetCharCallback(void (*func)(GLFWwindow *window, unsigned int character))
{
    glfwSetCharCallback(window, func);
}

void WindowManager::SetScrollCallback(void (*func)(GLFWwindow *window, double xOffset, double yOffset))
{
    glfwSetScrollCallback(window, func);
}

void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
    (void)window;
    WindowManager::SetMousePosition(xPos, yPos);
}