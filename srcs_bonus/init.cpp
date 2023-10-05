#include "init.hpp"

/**
 * callback function when the window is resized.
 * the viewport is the part of the window where we can draw
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;

    glViewport(0, 0, width, height);
}

/**
 * instantiate a window,
 * defined it as the current context
 * and set the framebuffer_size_callback() function
 * to the callback of the same name
*/
GLFWwindow* initWindow()
{
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window)
        throw(Utils::Exception("INIT_WINDOW::INITIALIZATION_FAILED"));
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return (window);
}

/**
 * init GLFW to configure it,
 * tell it that we use version 3.3 and core-profile of OpenGL,
 * call initWindow(),
 * and initialize functions pointers for OpenGL
*/
void initGLFW()
{
    if (glfwInit() == GL_FALSE)
        throw(Utils::Exception("INIT_GLFW::INITIALIZATION_FAILED"));

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void initOpenGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw(Utils::Exception("INIT_OPENGL::INITIALIZATION_FAILED"));
}