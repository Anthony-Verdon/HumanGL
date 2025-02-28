#pragma once

#include "classes/AProgram/AProgram.hpp"
#include "classes/ObjectClasses/Object/Object.hpp"
#include "classes/Camera/Camera.hpp"
#include "classes/Texture/Texture.hpp"
#include "classes/Shader/Shader.hpp"

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
        bool displayColor;
        float mixedValue;
        float sceneRotation[3];
        float inputRotation[3];
        AlgOps::vec3 axis[3];

        void ProcessInput();
        void updateCamera();
        void updateSceneOrientation();
        void updateDisplayMode();
        void updateWireframeMode();
        void updateTextureMode();
        void updateScene(const Texture &texture, const Shader &shader);
        void updateCameraView();
        void updateTexture();
        void renderObject(const Object &object, const Texture &texture, const Shader &shader);
        void updateShader(const Texture &texture, const Shader &shader);
    
    public:
        Game();
        ~Game();

        void LoadObjects(int argc, char **argv);
        void Run();

};