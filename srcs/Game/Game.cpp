#include "Game/Game.hpp"
#include "ObjectClasses/ObjectParser/ObjectParser.hpp"
#include "WindowManager/WindowManager.hpp"
#include "RessourceManager/RessourceManager.hpp"
#include "ModelLoader/ModelLoader.hpp"
#include "Time/Time.hpp"
#include "Toolbox.hpp"
#include "globals.hpp"
#include <cmath>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "geometry/geometry.hpp"

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
    axis[X_AXIS] = {1, 0, 0};
    axis[Y_AXIS] = {0, 1, 0};
    axis[Z_AXIS] = {0, 0, 1};

    WindowManager::SetUserPointer(&camera);
    WindowManager::SetScrollCallback(scroll_callback);
    WindowManager::SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    RessourceManager::AddShader("basic_shader", "shaders/shader.vs", "shaders/shader.fs");
    RessourceManager::AddShader("mesh_shader", "shaders/meshShader.vs", "shaders/meshShader.fs");
    RessourceManager::AddShader("light", "shaders/lightShader.vs", "shaders/lightShader.fs");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(WindowManager::GetWindow(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

Game::~Game() 
{
}

void Game::LoadObjects(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (Toolbox::checkExtension(argv[i], ".obj"))
        {
            std::vector<Object> newObjects = ObjectParser().parseObjectFile(argv[i]);
            objects.insert(objects.end(), newObjects.begin(), newObjects.end());
        }
        else if (Toolbox::checkExtension(argv[i], ".glb"))
        {
            std::vector<Model> newModels =  ModelLoader::LoadModel(argv[i]);
            models.insert(models.end(), newModels.begin(), newModels.end());
        }
    }
    for (size_t i = 0; i < objects.size(); i++)
        objects[i].initVAO();
    for (size_t i = 0; i < models.size(); i++)
        models[i].Init();

}

void Game::Run()
{
    Time::updateTime();
    ProcessInput();
    updateScene();
    DrawImGui();
}

void Game::ProcessInput()
{
    bool focusRenderer = WindowManager::GetInputMode(GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
    if (!HoverOrFocusImGUI && WindowManager::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
        WindowManager::SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    static bool keyEnable = true;
    if (keyEnable && WindowManager::IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        if (focusRenderer)
            WindowManager::SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            WindowManager::StopUpdateLoop();
        keyEnable = false;
    }
    else if (!WindowManager::IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        keyEnable = true;
    }
    if (focusRenderer)
    {
        updateCamera();
        updateSceneOrientation();
        updateDisplayMode();
    }
}


void Game::updateCamera()
{
    // position
    const float speed = camera.getSpeed() * Time::getDeltaTime();

    int front = WindowManager::IsKeyPressed(GLFW_KEY_W) - WindowManager::IsKeyPressed(GLFW_KEY_S);
    camera.addToPosition((float)front * camera.getFrontDirection() * speed);

    int right = WindowManager::IsKeyPressed(GLFW_KEY_D) - WindowManager::IsKeyPressed(GLFW_KEY_A);
    camera.addToPosition((float)right * camera.getRightDirection() * speed);

    int up = WindowManager::IsKeyPressed(GLFW_KEY_SPACE) - WindowManager::IsKeyPressed(GLFW_KEY_LEFT_SHIFT);
    camera.addToPosition((float)up * camera.getUpDirection() * speed);

    // orientation
    const float sensitivity = 0.1f;

    ml::vec2 mousePos = WindowManager::GetMousePosition();
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

void Game::updateScene()
{
    updateCameraView();
    updateTexture();
    
    /*
    for (size_t i = 0; i < objects.size(); i++)
        renderObject(objects[i]);
    
    {
        sceneRotation[X_AXIS] += inputRotation[X_AXIS] * Time::getDeltaTime();
        sceneRotation[Y_AXIS] += inputRotation[Y_AXIS] * Time::getDeltaTime();
        sceneRotation[Z_AXIS] += inputRotation[Z_AXIS] * Time::getDeltaTime();
        ml::mat4 rotation;
        rotation.identity();
        rotation =  ml::rotate(sceneRotation[X_AXIS], axis[X_AXIS]) *
        ml::rotate(sceneRotation[Y_AXIS], axis[Y_AXIS]) *
        ml::rotate(sceneRotation[Z_AXIS], axis[Z_AXIS]);
        
        ml::mat4 projection = ml::perspective(camera.getFov(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        
        ml::mat4 view = ml::lookAt(camera.getPosition(), camera.getPosition() + camera.getFrontDirection(),
        camera.getUpDirection());
        for (size_t i = 0; i < models.size(); i++)
            models[i].Draw(camera.getPosition(), light, projection, view);
    }
    */
    // light
    auto shader = RessourceManager::GetShader("light");
    shader->use();
    
    ml::mat4 projection = ml::perspective(camera.getFov(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    shader->setMat4("projection", projection);
    
    ml::mat4 view = ml::lookAt(camera.getPosition(), camera.getPosition() + camera.getFrontDirection(), camera.getUpDirection());
    shader->setMat4("view", view);
    
    ml::mat4 model = ml::translate(light.GetPos()) * ml::scale(light.GetScale());
    shader->setMat4("model", model);
    
    shader->setVec3("lightColor", light.GetColor());

    light.Draw();
}

void Game::updateCameraView()
{
    ml::vec3 direction(cosf(Toolbox::DegToRad(camera.getYaw())) * cosf(Toolbox::DegToRad(camera.getPitch())),
                    sinf(Toolbox::DegToRad(camera.getPitch())),
                    sinf(Toolbox::DegToRad(camera.getYaw())) * cosf(Toolbox::DegToRad(camera.getPitch())));
    camera.setFrontDirection(ml::normalize(direction));
    camera.setRightDirection(ml::normalize(ml::crossProduct(camera.getFrontDirection(), camera.getUpDirection())));
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
    updateShader(object);
    glBindVertexArray(object.getVAO());
    glDrawElements(GL_TRIANGLES, object.getFaces().size() * 3, GL_UNSIGNED_INT, 0);
}

void Game::updateShader(const Object &object)
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
    ml::mat4 rotation;
    rotation.identity();
    rotation = ml::rotate(sceneRotation[X_AXIS], axis[X_AXIS]) *
                ml::rotate(sceneRotation[Y_AXIS], axis[Y_AXIS]) *
                ml::rotate(sceneRotation[Z_AXIS], axis[Z_AXIS]);
    shader->setMat4("rotation", rotation);

    ml::mat4 projection = ml::perspective(camera.getFov(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    shader->setMat4("projection", projection);

    ml::mat4 view = ml::lookAt(camera.getPosition(), camera.getPosition() + camera.getFrontDirection(),
                                 camera.getUpDirection());
    shader->setMat4("view", view);
    ml::mat4 model;
    model.identity();
    shader->setMat4("model", model);

    shader->setVec3("viewPos", camera.getPosition());

    std::vector<Material> materials = object.getMaterials();
    for (size_t i = 0; i < materials.size(); i++)
    {
        ml::vec3 color = {materials[i].getColor(AMBIANT_COLOR)[0], materials[i].getColor(AMBIANT_COLOR)[1], materials[i].getColor(AMBIANT_COLOR)[2]};
        shader->setVec3("materials[" + std::to_string(i) + "].ambient", color);
        color = {materials[i].getColor(DIFFUSE_COLOR)[0], materials[i].getColor(DIFFUSE_COLOR)[1], materials[i].getColor(DIFFUSE_COLOR)[2]};
        shader->setVec3("materials[" + std::to_string(i) + "].diffuse", color);
        color = {materials[i].getColor(SPECULAR_COLOR)[0], materials[i].getColor(SPECULAR_COLOR)[1], materials[i].getColor(SPECULAR_COLOR)[2]};
        shader->setVec3("materials[" + std::to_string(i) + "].specular", color);
        shader->setFloat("materials[" + std::to_string(i) + "].shininess", 32.0f);
    }
    
    shader->setVec3("light.position", light.GetPos()); 
    shader->setVec3("light.ambient",  light.GetColor());
    shader->setVec3("light.diffuse",  0.5f, 0.5f, 0.5f);
    shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f); 
}

void Game::DrawImGui()
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("animations");
    HoverOrFocusImGUI = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
    for (size_t i = 0; i < models.size(); i++)
    {
        std::string model = "model " + std::to_string(i); 
        if (ImGui::CollapsingHeader(model.c_str()))
        {
            std::vector<std::string> animations = models[i].GetAnimations();
            for (auto it = animations.begin(); it != animations.end(); it++)
            {
                if (ImGui::Button(it->c_str()))
                {
                    models[i].SetAnimation(*it);
                }
            }
        }
    }
    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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