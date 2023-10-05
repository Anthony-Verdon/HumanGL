#ifndef SCENE_HPP
#define SCENE_HPP

#include "classes/Camera/Camera.hpp"

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

typedef struct s_scene {
    Camera *camera;
    bool displayColor;
    float mixValue;
    float rotation[3];
} t_scene;

#endif