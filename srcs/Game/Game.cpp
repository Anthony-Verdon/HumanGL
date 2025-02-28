#include "Game/Game.hpp"
#include "ObjectClasses/ObjectParser/ObjectParser.hpp"
#include "WindowManager/WindowManager.hpp"
#include "RessourceManager/RessourceManager.hpp"
#include "Time/Time.hpp"
#include "Utils/Utils.hpp"
#include "globals.hpp"
#include <cmath>

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

    RessourceManager::AddShader("basic_shader", "shaders/shader.vs", "shaders/shader.fs");
    RessourceManager::AddShader("light", "shaders/lightShader.vs", "shaders/lightShader.fs");
    RessourceManager::AddTexture("my_little_pony", "textures/myLittlePony.ppm");
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
    Time::updateTime();
    ProcessInput();
    updateScene();
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

    AlgOps::vec2 mousePos = WindowManager::GetMousePosition();
    static float lastX = mousePos.getX();
    static float lastY = mousePos.getY();

    float xOffset;
    float yOffset;
    xOffset = (mousePos.getX() - lastX) * sensitivity;
    yOffset = (lastY - mousePos.getY()) * sensitivity;
    lastX = mousePos.getX();
    lastY = mousePos.getY();
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

void Game::updateScene()
{
    updateCameraView();
    updateTexture();

    for (size_t i = 0; i < objects.size(); i++)
        renderObject(objects[i]);

    // light
    auto shader = RessourceManager::GetShader("light");
    shader->use();
    AlgOps::mat4 projection = AlgOps::perspective(camera.getFov(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    shader->setMat4("projection", projection);
    AlgOps::mat4 view = AlgOps::lookAt(camera.getPosition(), camera.getPosition() + camera.getFrontDirection(), camera.getUpDirection());
    shader->setMat4("view", view);
    shader->setVec3("lightColor", light.GetColor());

    AlgOps::vec3 lightPos;
    float posValues[] = {0, 0, -3};
    lightPos.setData(posValues, 3);
    AlgOps::vec3 lightScale;
    float scale = 0.5f;
    float scaleValues[] = {scale, scale, scale};
    lightScale.setData(scaleValues, 3);

    AlgOps::mat4 model;
    model.identity();
    model = AlgOps::translate(model, lightPos);
    model = AlgOps::scale(model, lightScale); 
    shader->setMat4("model", model);

    light.Draw();
}

void Game::updateCameraView()
{
    AlgOps::vec3 direction;
    float directionValues[] = {cosf(Utils::DegToRad(camera.getYaw())) * cosf(Utils::DegToRad(camera.getPitch())),
                               sinf(Utils::DegToRad(camera.getPitch())),
                               sinf(Utils::DegToRad(camera.getYaw())) * cosf(Utils::DegToRad(camera.getPitch()))};
    direction.setData(directionValues, 3);
    camera.setFrontDirection(AlgOps::normalize(direction));
    camera.setRightDirection(
        AlgOps::normalize(AlgOps::crossProduct(camera.getFrontDirection(), camera.getUpDirection())));
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

void Game::renderObject(const Object &object)
{
    updateShader();
    glBindVertexArray(object.getVAO());
    glDrawElements(GL_TRIANGLES, object.getFaces().size() * 3, GL_UNSIGNED_INT, 0);
}

void Game::updateShader()
{
    auto shader = RessourceManager::GetShader("basic_shader");
    auto texture = RessourceManager::GetTexture("my_little_pony");
    shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getID());
    shader->setInt("texture1", 0);
    shader->setFloat("aMixValue", mixedValue);

    sceneRotation[X_AXIS] += inputRotation[X_AXIS] * Time::getDeltaTime();
    sceneRotation[Y_AXIS] += inputRotation[Y_AXIS] * Time::getDeltaTime();
    sceneRotation[Z_AXIS] += inputRotation[Z_AXIS] * Time::getDeltaTime();
    AlgOps::mat4 rotation;
    rotation.uniform(1);
    rotation = AlgOps::rotate(rotation, sceneRotation[X_AXIS], axis[X_AXIS]) *
                AlgOps::rotate(rotation, sceneRotation[Y_AXIS], axis[Y_AXIS]) *
                AlgOps::rotate(rotation, sceneRotation[Z_AXIS], axis[Z_AXIS]);
    shader->setMat4("rotation", rotation);

    AlgOps::mat4 projection = AlgOps::perspective(camera.getFov(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    shader->setMat4("projection", projection);

    AlgOps::mat4 view = AlgOps::lookAt(camera.getPosition(), camera.getPosition() + camera.getFrontDirection(),
                                 camera.getUpDirection());
    shader->setMat4("view", view);
    shader->setVec3("lightColor", light.GetColor());
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