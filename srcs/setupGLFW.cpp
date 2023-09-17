#include "main.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

GLFWwindow* initWindow()
{
    //create a window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return (NULL);
    }
    glfwMakeContextCurrent(window);

    //when the window is resized, the viewport is also resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return (window);
}

GLFWwindow* initGLFW()
{
    //init GLFW so we can configure it
    glfwInit();

    //tell GLFW that we use version 3.3 and core-profile of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = initWindow();
    
    //initialize functions pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return (NULL);
    }

    //initialize base color to clear the screen 
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    return (window);
}