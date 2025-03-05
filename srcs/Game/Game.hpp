#pragma once

#include "AProgram/AProgram.hpp"
#include "ObjectClasses/Object/Object.hpp"
#include "Mesh/MeshRenderer/MeshRenderer.hpp"
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
        std::vector<Object> objects;
        std::vector<MeshRenderer> meshes;
        bool displayColor;
        float mixedValue;
        float sceneRotation[3];
        float inputRotation[3];
        AlgOps::vec3 axis[3];
        Light light;
        
        void ProcessInput();
        void updateCamera();
        void updateSceneOrientation();
        void updateDisplayMode();
        void updateWireframeMode();
        void updateTextureMode();
        void updateScene();
        void updateCameraView();
        void updateTexture();
        void renderObject(const Object &object);
        void updateShader(const Object &object);
    
    public:
        Game();
        ~Game();

        void LoadObjects(int argc, char **argv);
        void Run();

};