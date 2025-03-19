#pragma once

#include "vec/vec.hpp"

class Light
{
    private:
        unsigned int VAO;
        unsigned int VBO;

        ml::vec3 color;
        ml::vec3 pos;
        ml::vec3 scale;
        
    public:
        Light();
        ~Light();

        void Draw();
        ml::vec3 GetColor() const { return color; }
        ml::vec3 GetPos() const { return pos; }
        ml::vec3 GetScale() const { return scale; }
};