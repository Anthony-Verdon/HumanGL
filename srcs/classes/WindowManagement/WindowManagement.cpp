#include "WindowManagement.hpp"
#include <iostream>
#include <stdexcept>

WindowManagement::WindowManagement()
{
    start();
    updateLoop();
}

WindowManagement::~WindowManagement()
{
    glfwTerminate();
}

void WindowManagement::start()
{
    initGLFW();
    initWindow();
    initOpenGL();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetWindowUserPointer(window, &scene);
}

void WindowManagement::initGLFW()
{
    if (glfwInit() == GL_FALSE)
        throw(std::runtime_error("INIT_GLFW::INITIALIZATION_FAILED"));

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

/**
 * callback function when the window is resized.
 * the viewport is the part of the window where we can draw
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    std::cout << "Here" << std::endl;
    glViewport(0, 0, width, height);
}

/**
 * instantiate a window,
 * defined it as the current context
 * and set the framebuffer_size_callback() function
 * to the callback of the same name
 */
void WindowManagement::initWindow()
{
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window)
        throw(std::runtime_error("INIT_WINDOW::INITIALIZATION_FAILED"));
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

/**
 * init GLFW to configure it,
 * tell it that we use version 3.3 and core-profile of OpenGL,
 * call initWindow(),
 * and initialize functions pointers for OpenGL
 */
void WindowManagement::initOpenGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw(std::runtime_error("INIT_OPENGL::INITIALIZATION_FAILED"));
}
void WindowManagement::updateLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
