#pragma once

#include "Matrix.hpp"

class Light
{
    private:
        unsigned int VAO;
        unsigned int VBO;

        AlgOps::vec3 color;
        AlgOps::vec3 pos;
        AlgOps::vec3 scale;
        
    public:
        Light();
        ~Light();

        void Draw();
        AlgOps::vec3 GetColor() const { return color; }
        AlgOps::vec3 GetPos() const { return pos; }
        AlgOps::vec3 GetScale() const { return scale; }
};