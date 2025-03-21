#pragma once

#include "AProgram/AProgram.hpp"
#include "Model/Model.hpp"
#include "Camera/Camera.hpp"
#include "Texture/Texture.hpp"
#include "Shader/Shader.hpp"
#include "Light/Light.hpp"

class Game: public AProgram
{
    private:
        Camera camera;
        std::vector<Model> models;
        bool HoverOrFocusImGUI;
        Light light;
        
        void ProcessInput();
        void updateCamera();
        void updateScene();
        
        void DrawImGui();
        void AddChildNode(std::map<int, NodeModel> &nodes, int parentIndex, int nodeIndex);
        void AddDragAndDrop(std::map<int, NodeModel> &nodes, int parentIndex, int nodeIndex);
        void AddDragAndDrop(std::vector<Model> &models, int modelIndex);
        bool CheckNoRecursiveChild(std::map<int, NodeModel> &nodes, int nodeIndex, int childIndex);

    public:
        Game();
        ~Game();

        void LoadObjects(int argc, char **argv);
        void Run();

};