#include "Game/Game.hpp"
#include "WindowManager/WindowManager.hpp"
#include "RessourceManager/RessourceManager.hpp"
#include "Model/ModelLoader/ModelLoader.hpp"
#include "Time/Time.hpp"
#include "Toolbox.hpp"
#include "globals.hpp"
#include <cmath>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "geometry/geometry.hpp"
#include <algorithm>

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

Game::Game() 
{
    camera = Camera();

    WindowManager::SetUserPointer(&camera);
    WindowManager::SetScrollCallback(scroll_callback);
    WindowManager::SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
        if (Toolbox::checkExtension(argv[i], ".glb"))
        {
            std::vector<Model> newModels =  ModelLoader::LoadModel(argv[i]);
            models.insert(models.end(), newModels.begin(), newModels.end());
        }
    }
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
    
    static bool keyEnable = true; //@todo when creating engine submodule, replace that
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
        updateCamera();
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

    ml::vec3 direction(cosf(Toolbox::DegToRad(camera.getYaw())) * cosf(Toolbox::DegToRad(camera.getPitch())),
                sinf(Toolbox::DegToRad(camera.getPitch())),
                sinf(Toolbox::DegToRad(camera.getYaw())) * cosf(Toolbox::DegToRad(camera.getPitch())));
    camera.setFrontDirection(ml::normalize(direction));
    camera.setRightDirection(ml::normalize(ml::crossProduct(camera.getFrontDirection(), camera.getUpDirection())));
}

void Game::updateScene()
{
    ml::mat4 projection = ml::perspective(camera.getFov(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    ml::mat4 view = ml::lookAt(camera.getPosition(), camera.getPosition() + camera.getFrontDirection(), camera.getUpDirection());
    
    // models
    for (size_t i = 0; i < models.size(); i++)
        models[i].Draw(camera.getPosition(), light, projection, view);
    
    // light
    auto shader = RessourceManager::GetShader("light");
    shader->use();
    ml::mat4 model = ml::translate(light.GetPos()) * ml::rotate(45, ml::vec3(1, 1, 1)) * ml::scale(light.GetScale());
    shader->setMat4("model", model);
    shader->setVec3("lightColor", light.GetColor());
    light.Draw();
}

void Game::DrawImGui()
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Characters");
    HoverOrFocusImGUI = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
    for (size_t i = 0; i < models.size(); i++)
    {
        std::string model = "model " + std::to_string(i);
        if (ImGui::CollapsingHeader(model.c_str()))
        {
            std::string skeletton = "skeletton## " + model;
            if (ImGui::TreeNode(skeletton.c_str()))
            {
                auto [data, nodeIndex] = models[i].GetRootNode();
                AddChildNode(data, -1, nodeIndex);
                models[i].SetData(data);
                ImGui::TreePop();
            }
            std::string animations = "animations## " + model;
            if (ImGui::TreeNode(animations.c_str()))
            {
                std::vector<std::string> animations = models[i].GetAnimations();
                for (auto it = animations.begin(); it != animations.end(); it++)
                {
                    if (ImGui::Button(it->c_str()))
                    {
                        models[i].SetAnimation(*it);
                    }
                }
                ImGui::TreePop();
            }
            
        }
    }
    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::AddChildNode(Glb::GltfData &data, int parentIndex, int nodeIndex)
{
    auto &node = data.nodes[nodeIndex];

    if (node.children.size() == 0)
    {
        ImGui::Selectable(node.name.c_str());
        AddDragAndDrop(data, parentIndex, nodeIndex);
    }
    else
    {
        if (ImGui::TreeNode(node.name.c_str()))
        {
            AddDragAndDrop(data, parentIndex, nodeIndex);
            for (size_t i = 0; i < node.children.size(); i++)
                AddChildNode(data, nodeIndex, node.children[i]);
            ImGui::TreePop();
        }
        else
        {
            AddDragAndDrop(data, parentIndex, nodeIndex);
        }
    }
}

void Game::AddDragAndDrop(Glb::GltfData &data, int parentIndex, int nodeIndex)
{
    auto &node = data.nodes[nodeIndex];
    if (ImGui::BeginDragDropSource())
    {
        std::pair<int, int> indexes = std::make_pair(parentIndex, nodeIndex);
        ImGui::SetDragDropPayload("SKELETTON_NODE_SELECTED", &indexes, sizeof(std::pair<int, int>));
        ImGui::Text("%s", node.name.c_str());
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SKELETTON_NODE_SELECTED"))
        {
            auto [parentIndex, childIndex] = *(std::pair<int, int>*)payload->Data;
            if (CheckNoRecursiveChild(data, nodeIndex, childIndex))
            {
                node.children.push_back(childIndex);
                if (parentIndex != -1)
                {
                    auto &parent = data.nodes[parentIndex];
                    auto childIt = std::find(parent.children.begin(), parent.children.end(), childIndex);
                    parent.children.erase(childIt);
                }
            }
        }
        ImGui::EndDragDropTarget();
    }
}

bool Game::CheckNoRecursiveChild(Glb::GltfData &data, int newParent, int newChild)
{
    auto newChildNode = data.nodes[newChild];

    for (size_t i = 0; i < newChildNode.children.size(); i++)
    {
        if (newChildNode.children[i] == newParent || !CheckNoRecursiveChild(data, newParent, newChildNode.children[i]))
            return (false);
    }

    return (true);
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