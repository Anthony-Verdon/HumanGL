#include "Game/Game.hpp"
#include "WindowManager/WindowManager.hpp"
#include "RessourceManager/RessourceManager.hpp"
#include "Model/ModelLoader/ModelLoader.hpp"
#include "ModelManager/ModelManager.hpp"
#include "Time/Time.hpp"
#include "Toolbox.hpp"
#include "globals.hpp"
#include <cmath>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Matrix/Matrix.hpp"
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

    for (size_t i = 0; i < 4; i++)
    {
        lights[i].SetScale(ml::vec3(0.5, 0.5, 0.5));
        lights[i].SetIntensity(15);
    }
    lights[0].SetPos(ml::vec3(0, 0, -3));
    lights[0].SetColor(ml::vec3(1, 0, 0));
    lights[1].SetPos(ml::vec3(0, 0, 3));
    lights[1].SetColor(ml::vec3(0, 1, 0));
    lights[2].SetPos(ml::vec3(-3, 0, 0));
    lights[2].SetColor(ml::vec3(0, 0, 1));
    lights[3].SetPos(ml::vec3(3, 0, 0));
    lights[3].SetColor(ml::vec3(1, 1, 1));
    HoverOrFocusImGUI = false;
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
    for (size_t i = 0; i < ModelManager::GetNbModel(); i++)
        ModelManager::GetModel(i).Destroy();
}

void Game::LoadObjects(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
        ModelManager::AddModels(ModelLoader::LoadModel(argv[i]));
    for (size_t i = 0; i < ModelManager::GetNbModel(); i++)
    {
        ModelManager::GetModel(i).Init();
        modelsIndex.push_back(i);
    }
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

    ml::vec3 direction(cosf(ml::radians(camera.getYaw())) * cosf(ml::radians(camera.getPitch())),
                sinf(ml::radians(camera.getPitch())),
                sinf(ml::radians(camera.getYaw())) * cosf(ml::radians(camera.getPitch())));
    camera.setFrontDirection(ml::normalize(direction));
    camera.setRightDirection(ml::normalize(ml::crossProduct(camera.getFrontDirection(), camera.getUpDirection())));
}

void Game::updateScene()
{
    ml::mat4 projection = ml::perspective(camera.getFov(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    ml::mat4 view = ml::lookAt(camera.getPosition(), camera.getPosition() + camera.getFrontDirection(), camera.getUpDirection());
    
    // models
    ml::mat4 transform;
    transform.identity();
    for (size_t i = 0; i < modelsIndex.size(); i++)
        ModelManager::GetModel(modelsIndex[i]).Draw(camera.getPosition(), camera.getFrontDirection(), lights, projection, view, transform);
    
    // light
    auto shader = RessourceManager::GetShader("light");
    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    for (size_t i = 0; i < 4; i++)
    {
        ml::mat4 model = ml::translate(ml::mat4(1.0f), lights[i].GetPos()) * ml::scale(ml::mat4(1.0f), lights[i].GetScale());
        shader->setMat4("model", model);
        shader->setVec3("lightColor", lights[i].GetColor());
        lights[i].Draw();
    }
}

void Game::DrawImGui()
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Characters");
    HoverOrFocusImGUI = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
    ImGui::BeginChild("");
    HoverOrFocusImGUI |= ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
    AddModels(&modelsIndex);
    ImGui::EndChild();
    AddDragAndDropTarget();
    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::AddModels(std::vector<int> *modelsIndex)
{
    for (size_t i = 0; i < modelsIndex->size(); i++)
    {
        auto &model = ModelManager::GetModel((*modelsIndex)[i]);
        auto &nodes = model.GetNodes();
        size_t nodeIndex = model.GetRootIndex();
        std::string modelStr = "model " + std::to_string((*modelsIndex)[i]);
        if (ImGui::CollapsingHeader(modelStr.c_str()))
        {
            AddDragAndDropSource(modelsIndex, (*modelsIndex)[i]);
            std::string skeletton = "skeletton## " + modelStr;
            if (ImGui::TreeNode(skeletton.c_str()))
            {
                AddChildNode(nodes, nodeIndex);
                ImGui::TreePop();
            }
            std::string animations = "animations## " + modelStr;
            if (ImGui::TreeNode(animations.c_str()))
            {
                std::vector<std::string> animations = model.GetAnimationsName();
                for (auto it = animations.begin(); it != animations.end(); it++)
                {
                    if (ImGui::Button(it->c_str()))
                        model.Play(*it);
                }
                ImGui::TreePop();
            }
        }
        else
        {
            AddDragAndDropSource(modelsIndex, (*modelsIndex)[i]);
        }
    }
}

void Game::AddChildNode(std::map<int, NodeModel> &nodes, int nodeIndex)
{
    auto &node = nodes[nodeIndex];

    if (node.children.size() == 0 && node.models.size() == 0)
    {
        ImGui::Text("%s", node.name.c_str());
        AddDragAndDropTarget(nodes, nodeIndex);
    }
    else
    {
        if (ImGui::TreeNode(node.name.c_str()))
        {
            AddDragAndDropTarget(nodes, nodeIndex);
            for (size_t i = 0; i < node.children.size(); i++)
                AddChildNode(nodes, node.children[i]);
            AddModels(&node.models);
            ImGui::TreePop();
        }
        else
        {
            AddDragAndDropTarget(nodes, nodeIndex);
        }
    }
}

void Game::AddDragAndDropSource(std::vector<int> *modelsIndex, int modelIndex)
{
    if (ImGui::BeginDragDropSource())
    {
        std::pair<std::vector<int>*, int> modelData = std::make_pair(modelsIndex, modelIndex);
        ImGui::SetDragDropPayload("MODEL_SELECTED", &modelData, sizeof(std::pair<std::vector<int>*, int>));
        ImGui::Text("%s", "model");
        ImGui::EndDragDropSource();
    }
}

void Game::AddDragAndDropTarget()
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL_SELECTED"))
        {
            auto [nodeModelsIndex, nodeModelIndex] = *(std::pair<std::vector<int>*, int>*)payload->Data;
            modelsIndex.push_back(nodeModelIndex);
            auto it = std::find(nodeModelsIndex->begin(), nodeModelsIndex->end(), nodeModelIndex);
            nodeModelsIndex->erase(it); 
        }
        ImGui::EndDragDropTarget();
    }
}

void Game::AddDragAndDropTarget(std::map<int, NodeModel> &nodes, int nodeIndex)
{
    auto &node = nodes[nodeIndex];
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL_SELECTED"))
        {
            auto [modelsIndex, modelIndex] = *(std::pair<std::vector<int>*, int>*)payload->Data;
            auto model = ModelManager::GetModel(modelIndex);
            if (!NodeChildOfModel(model.GetNodes(), model.GetRootIndex(), nodeIndex))
            {
                node.models.push_back(modelIndex);
                auto it = std::find(modelsIndex->begin(), modelsIndex->end(), modelIndex);
                modelsIndex->erase(it); 
            }
        }
        ImGui::EndDragDropTarget();
    }
}

bool Game::NodeChildOfModel(std::map<int, NodeModel> &nodes, int rootNode, int nodeIndex)
{
    if (rootNode == nodeIndex)
        return (true);

    auto node = nodes[rootNode];
    for (size_t i = 0; i < node.children.size(); i++)
    {
        if (node.children[i] == nodeIndex || NodeChildOfModel(nodes, node.children[i], nodeIndex))
            return (true);
    }
    return (false);
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