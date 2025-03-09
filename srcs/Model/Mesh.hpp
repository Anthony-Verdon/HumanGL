#pragma once

#include "GlbParser/GlbParser.hpp"

class Mesh
{
    public:
        Mesh(const std::vector<Glb::Vertex> &vertices, const std::vector<uint16_t> &indices);
        ~Mesh();

        void Init();
        void Draw(const AlgOps::mat4 &projection, const AlgOps::mat4 &view) const;

    private:
        unsigned int VAO, VBO, EBO;
        std::vector<Glb::Vertex> vertices;
        std::vector<uint16_t> indices;
};