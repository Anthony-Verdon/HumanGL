#pragma once

#include "Matrix/Matrix.hpp"

class Light
{
    private:
        unsigned int VAO;
        unsigned int VBO;

        AlgOps::vec3 color;
        
    public:
        Light();
        ~Light();

        void Draw();
        AlgOps::vec3 GetColor() const { return color; }
};