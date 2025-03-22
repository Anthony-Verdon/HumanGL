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
        float intensity;
        
    public:
        Light();
        ~Light();

        void Draw();
        ml::vec3 GetColor() const { return color; }
        ml::vec3 GetPos() const { return pos; }
        ml::vec3 GetScale() const { return scale; }
        float GetIntensity() const { return intensity; }

        void SetColor(const ml::vec3 &color) { this->color = color; }
        void SetPos(const ml::vec3 &pos) { this->pos = pos; }
        void SetScale(const ml::vec3 &scale) { this->scale = scale; }
        void SetIntensity(float intensity) { this->intensity = intensity; }
};