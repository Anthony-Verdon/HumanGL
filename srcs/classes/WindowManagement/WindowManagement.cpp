#include "WindowManagement.hpp"
#include "../Shader/Shader.hpp"
#include "../Texture/Texture.hpp"
#include "../Time/Time.hpp"
#include "../Utils/Utils.hpp"
#include <cmath>

WindowManagement::WindowManagement(const std::vector<Object> &objects)
{
    start(objects);
    updateLoop();
}

WindowManagement::~WindowManagement()
{
    glfwTerminate();
}

void WindowManagement::start(const std::vector<Object> &objects)
{
    initGLFW();
    initOpenGL();
    initData(objects);
}

void WindowManagement::initGLFW()
{
    if (glfwInit() == GL_FALSE)
        throw(std::runtime_error("INIT_GLFW::INITIALIZATION_FAILED"));

    initWindow();

    glfwSetWindowUserPointer(window, &camera);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void WindowManagement::initWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Scop", NULL, NULL);
    if (!window)
        throw(std::runtime_error("INIT_WINDOW::INITIALIZATION_FAILED"));
    glfwMakeContextCurrent(window);
}

void WindowManagement::initOpenGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw(std::runtime_error("INIT_OPENGL::INITIALIZATION_FAILED"));

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void WindowManagement::initData(const std::vector<Object> &objects)
{
    this->objects = objects;
    for (size_t i = 0; i < this->objects.size(); i++)
        this->objects[i].initVAO();
    camera = Camera();
    displayColor = true;
    mixedValue = 0;
    for (size_t i = 0; i < 3; i++)
    {
        inputRotation[i] = 0;
        sceneRotation[i] = 0;
        axis[i] = Matrix(3, 1);
    }
    float values[3] = {0};
    values[0] = 1;
    axis[X_AXIS].setData(values, 3);
    values[0] = 0;
    values[1] = 1;
    axis[Y_AXIS].setData(values, 3);
    values[1] = 0;
    values[2] = 1;
    axis[Z_AXIS].setData(values, 3);
}

void WindowManagement::updateLoop()
{
    const Texture texture("textures/wall.ppm");
    const Shader shader("srcs/shaders/shader.vs", "srcs/shaders/shader.fs");
    while (!glfwWindowShouldClose(window))
    {
        Time::updateTime();
        processInput();
        updateScene(texture, shader);
    }
}

void WindowManagement::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    updateCameraPosition();
    updateSceneOrientation();
    updateDisplayMode();
}

bool WindowManagement::isKeyPressed(int key)
{
    return (glfwGetKey(window, key) == GLFW_PRESS);
}

void WindowManagement::updateCameraPosition()
{
    const float speed = camera.getSpeed() * Time::getDeltaTime();

    int front = isKeyPressed(GLFW_KEY_W) - isKeyPressed(GLFW_KEY_S);
    camera.addToPosition(front * camera.getFrontDirection() * speed);

    int right = isKeyPressed(GLFW_KEY_D) - isKeyPressed(GLFW_KEY_A);
    camera.addToPosition(right * camera.getRightDirection() * speed);
}

void WindowManagement::updateSceneOrientation()
{
    inputRotation[X_AXIS] = isKeyPressed(GLFW_KEY_U) - isKeyPressed(GLFW_KEY_Y);
    inputRotation[Y_AXIS] = isKeyPressed(GLFW_KEY_J) - isKeyPressed(GLFW_KEY_H);
    inputRotation[Z_AXIS] = isKeyPressed(GLFW_KEY_M) - isKeyPressed(GLFW_KEY_N);
}

void WindowManagement::updateDisplayMode()
{
    updateWireframeMode();
    updateTextureMode();
}

void WindowManagement::updateWireframeMode()
{
    static bool wireFrameMode = false;
    static bool keyEnable = true;

    if (isKeyPressed(GLFW_KEY_F1))
    {
        if (keyEnable == true)
        {
            if (wireFrameMode)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wireFrameMode = !wireFrameMode;
        }
        keyEnable = false;
    }
    else
        keyEnable = true;
}

void WindowManagement::updateTextureMode()
{
    static bool keyEnable = true;

    if (isKeyPressed(GLFW_KEY_F2))
    {
        if (keyEnable == true)
            displayColor = !displayColor;
        keyEnable = false;
    }
    else
        keyEnable = true;
}

void WindowManagement::updateScene(const Texture &texture, const Shader &shader)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCameraView();
    updateTexture();

    for (size_t i = 0; i < objects.size(); i++)
        renderObject(objects[i], texture, shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void WindowManagement::updateCameraView()
{
    Matrix direction(3, 1);
    float directionValues[] = {cosf(Utils::DegToRad(camera.getYaw())) * cosf(Utils::DegToRad(camera.getPitch())),
                               sinf(Utils::DegToRad(camera.getPitch())),
                               sinf(Utils::DegToRad(camera.getYaw())) * cosf(Utils::DegToRad(camera.getPitch()))};
    direction.setData(directionValues, 3);
    camera.setFrontDirection(Matrix::normalize(direction));
    camera.setRightDirection(
        Matrix::normalize(Matrix::crossProduct(camera.getFrontDirection(), camera.getUpDirection())));
}

void WindowManagement::updateTexture()
{
    if (displayColor == true && mixedValue > 0)
    {
        mixedValue -= Time::getDeltaTime();
        if (mixedValue < 0)
            mixedValue = 0;
    }
    else if (displayColor == false && mixedValue < 1)
    {
        mixedValue += Time::getDeltaTime();
        if (mixedValue > 1)
            mixedValue = 1;
    }
}

void WindowManagement::renderObject(const Object &object, const Texture &texture, const Shader &shader)
{
    updateShader(object, texture, shader);
    glBindVertexArray(object.getVAO());
    glDrawElements(GL_TRIANGLES, object.getFaces().size() * 3, GL_UNSIGNED_INT, 0);
}

void WindowManagement::updateShader(const Object &object, const Texture &texture, const Shader &shader)
{
    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.getID());
    shader.setInt("texture1", 0);
    std::array<float, 3> color = object.getMaterial().getColor(AMBIANT_COLOR);
    shader.setVec3("aColor", color[0], color[1], color[2]);
    shader.setFloat("aMixValue", mixedValue);

    sceneRotation[X_AXIS] += inputRotation[X_AXIS] * Time::getDeltaTime();
    sceneRotation[Y_AXIS] += inputRotation[Y_AXIS] * Time::getDeltaTime();
    sceneRotation[Z_AXIS] += inputRotation[Z_AXIS] * Time::getDeltaTime();
    Matrix rotation(4, 4);
    rotation.uniform(1);
    rotation = Matrix::rotate(rotation, sceneRotation[X_AXIS], axis[X_AXIS]) *
               Matrix::rotate(rotation, sceneRotation[Y_AXIS], axis[Y_AXIS]) *
               Matrix::rotate(rotation, sceneRotation[Z_AXIS], axis[Z_AXIS]);
    shader.setMat4("rotation", rotation);

    Matrix projection = Matrix::perspective(camera.getFov(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    Matrix view = Matrix::lookAt(camera.getPosition(), camera.getPosition() + camera.getFrontDirection(),
                                 camera.getUpDirection());
    shader.setMat4("view", view);
}

/**
 * callback function when the window is resized.
 * the viewport is the part of the window where we can draw
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
    const float sensitivity = 0.1f;

    static float lastX = xPos;
    static float lastY = yPos;

    float xOffset;
    float yOffset;
    Camera *camera = reinterpret_cast<Camera *>(glfwGetWindowUserPointer(window));
    xOffset = (xPos - lastX) * sensitivity;
    yOffset = (lastY - yPos) * sensitivity;
    lastX = xPos;
    lastY = yPos;
    camera->addToYaw(xOffset);
    camera->addToPitch(yOffset);
    if (camera->getPitch() > 89.0f)
        camera->setPitch(89.0f);
    else if (camera->getPitch() < -89.0f)
        camera->setPitch(-89.0f);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
{
    (void)xOffset;

    Camera *camera = reinterpret_cast<Camera *>(glfwGetWindowUserPointer(window));
    camera->addToFov((float)-yOffset);
    if (camera->getFov() < 1.0f)
        camera->setFov(1.0f);
    else if (camera->getFov() > 45.0f)
        camera->setFov(45.0f);
}
