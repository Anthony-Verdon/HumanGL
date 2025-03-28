#pragma once

#include "Engine/AProgram/AProgram.hpp"
#include "Engine/3D/Model/Model.hpp"
#include "Engine/3D/Camera3D/Camera3D.hpp"
#include "Engine/Texture/Texture.hpp"
#include "Engine/3D/Light/Light.hpp"

class Game: public AProgram
{
    private:
        Camera3D camera;
        std::vector<int> modelsIndex;
        bool HoverOrFocusImGUI;
        Light lights[4];
        
        void ProcessInput();
        void updateCamera();
        void updateScene();
        
        void DrawImGui();
        void AddModels(std::vector<int> *modelsIndex);
        void AddChildNode(std::map<int, NodeModel> &nodes, int nodeIndex);
        void AddDragAndDropSource(std::vector<int> *modelsIndex, int modelIndex);
        void AddDragAndDropTarget();
        void AddDragAndDropTarget(std::map<int, NodeModel> &nodes, int nodeIndex);
        bool NodeChildOfModel(std::map<int, NodeModel> &nodes, int rootNode, int nodeIndex);

    public:
        Game();
        ~Game();

        void LoadObjects(int argc, char **argv);
        void Run();

};