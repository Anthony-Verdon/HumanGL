#pragma once

#include "../../../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../Camera/Camera.hpp"
#include "../ObjectClasses/Object/Object.hpp"
#include <vector>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef enum axis
{
    X_AXIS,
    Y_AXIS,
    Z_AXIS
} e_axis;

typedef struct data
{
    std::vector<Object> objects;
    Camera camera;

    bool displayColor;
    float mixedValue;
    float sceneRotation[3];
    float inputRotation[3];
    Matrix axis[3];
} s_data;

class WindowManagement : public data
{
  private:
    GLFWwindow *window;

    void start(const std::vector<Object> &objects);
    void initGLFW();
    void initWindow();
    void initOpenGL();
    void initData(const std::vector<Object> &objects);

    void updateLoop();

    void processInput();
    bool isKeyPressed(int key);
    void updateDisplayMode(); // texture, color, wireframe
    void updateWireframeMode();
    void updateTextureMode();
    void updateCameraPosition();
    void updateSceneOrientation();

    void updateScene();
    void updateTexture();

  public:
    WindowManagement(const std::vector<Object> &objects);
    ~WindowManagement();
};