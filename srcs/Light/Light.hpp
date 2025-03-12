#pragma once

#include <glm/glm.hpp>

class Light
{
    private:
        unsigned int VAO;
        unsigned int VBO;

        glm::vec3 color;
        glm::vec3 pos;
        glm::vec3 scale;
        
    public:
        Light();
        ~Light();

        void Draw();
        glm::vec3 GetColor() const { return color; }
        glm::vec3 GetPos() const { return pos; }
        glm::vec3 GetScale() const { return scale; }
};