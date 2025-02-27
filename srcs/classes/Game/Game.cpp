#include "classes/Game/Game.hpp"
#include "classes/ObjectClasses/ObjectParser/ObjectParser.hpp"
#include "classes/WindowManager/WindowManager.hpp"
#include "classes/Time/Time.hpp"
#include "classes/Utils/Utils.hpp"
#include "globals.hpp"

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

Game::Game() 
{
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

    WindowManager::SetUserPointer(&camera);
    WindowManager::SetScrollCallback(scroll_callback);
}

Game::~Game() 
{
}

void Game::LoadObjects(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        std::vector<Object> newObjects = ObjectParser().parseObjectFile(argv[i]);
        objects.insert(objects.end(), newObjects.begin(), newObjects.end());
    }

    for (size_t i = 0; i < objects.size(); i++)
        objects[i].initVAO();
}

void Game::Run()
{
    static const Texture texture("srcs/textures/myLittlePony.ppm");
    static const Shader shader("srcs/shaders/shader.vs", "srcs/shaders/shader.fs");

    Time::updateTime();
    ProcessInput();
    updateScene(texture, shader);
}

void Game::ProcessInput()
{
    if (WindowManager::IsKeyPressed(GLFW_KEY_ESCAPE))
        WindowManager::StopUpdateLoop();
    updateCamera();
    updateSceneOrientation();
    updateDisplayMode();
}


void Game::updateCamera()
{
    // position
    const float speed = camera.getSpeed() * Time::getDeltaTime();

    int front = WindowManager::IsKeyPressed(GLFW_KEY_W) - WindowManager::IsKeyPressed(GLFW_KEY_S);
    camera.addToPosition(front * camera.getFrontDirection() * speed);

    int right = WindowManager::IsKeyPressed(GLFW_KEY_D) - WindowManager::IsKeyPressed(GLFW_KEY_A);
    camera.addToPosition(right * camera.getRightDirection() * speed);

    int up = WindowManager::IsKeyPressed(GLFW_KEY_SPACE) - WindowManager::IsKeyPressed(GLFW_KEY_LEFT_SHIFT);
    camera.addToPosition(up * camera.getUpDirection() * speed);

    // orientation
    const float sensitivity = 0.1f;

    glm::vec2 mousePos = WindowManager::GetMousePosition();
    static float lastX = mousePos.x;
    static float lastY = mousePos.y;

    float xOffset;
    float yOffset;
    xOffset = (mousePos.x - lastX) * sensitivity;
    yOffset = (lastY - mousePos.y) * sensitivity;
    lastX = mousePos.x;
    lastY = mousePos.y;
    camera.addToYaw(xOffset);
    camera.addToPitch(yOffset);
    if (camera.getPitch() > 89.0f)
        camera.setPitch(89.0f);
    else if (camera.getPitch() < -89.0f)
        camera.setPitch(-89.0f);
}

void Game::updateSceneOrientation()
{
    inputRotation[X_AXIS] = WindowManager::IsKeyPressed(GLFW_KEY_U) - WindowManager::IsKeyPressed(GLFW_KEY_Y);
    inputRotation[Y_AXIS] = WindowManager::IsKeyPressed(GLFW_KEY_J) - WindowManager::IsKeyPressed(GLFW_KEY_H);
    inputRotation[Z_AXIS] = WindowManager::IsKeyPressed(GLFW_KEY_M) - WindowManager::IsKeyPressed(GLFW_KEY_N);
}

void Game::updateDisplayMode()
{
    updateWireframeMode();
    updateTextureMode();
}

void Game::updateWireframeMode()
{
    static bool wireFrameMode = false;
    static bool keyEnable = true;

    if (WindowManager::IsKeyPressed(GLFW_KEY_F1))
    {
        if (keyEnable == true)
        {
            wireFrameMode = !wireFrameMode;
            if (wireFrameMode)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        keyEnable = false;
    }
    else
        keyEnable = true;
}

void Game::updateTextureMode()
{
    static bool keyEnable = true;

    if (WindowManager::IsKeyPressed(GLFW_KEY_F2))
    {
        if (keyEnable == true)
            displayColor = !displayColor;
        keyEnable = false;
    }
    else
        keyEnable = true;
}

void Game::updateScene(const Texture &texture, const Shader &shader)
{
    updateCameraView();
    updateTexture();

    for (size_t i = 0; i < objects.size(); i++)
        renderObject(objects[i], texture, shader);
}

void Game::updateCameraView()
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

void Game::updateTexture()
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

void Game::renderObject(const Object &object, const Texture &texture, const Shader &shader)
{
    updateShader(texture, shader);
    glBindVertexArray(object.getVAO());
    glDrawElements(GL_TRIANGLES, object.getFaces().size() * 3, GL_UNSIGNED_INT, 0);
}

void Game::updateShader(const Texture &texture, const Shader &shader)
{
    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.getID());
    shader.setInt("texture1", 0);
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