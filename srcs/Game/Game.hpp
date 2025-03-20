#pragma once

#include "AProgram/AProgram.hpp"
#include "Model/Model.hpp"
#include "Camera/Camera.hpp"
#include "Texture/Texture.hpp"
#include "Shader/Shader.hpp"
#include "Light/Light.hpp"

typedef enum axis
{
    X_AXIS,
    Y_AXIS,
    Z_AXIS
} e_axis;

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
        void AddChildNode(const Glb::GltfData &data, size_t nodeIndex);

    public:
        Game();
        ~Game();

        void LoadObjects(int argc, char **argv);
        void Run();

};