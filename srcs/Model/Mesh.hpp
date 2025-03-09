#pragma once

#include "GlbParser/GlbParser.hpp"

class Mesh
{
    public:
        Mesh(const Glb::GltfData &data, const Glb::Mesh &mesh);
        ~Mesh();

        void Init();
        void Draw(const AlgOps::mat4 &projection, const AlgOps::mat4 &view) const;

    private:
        unsigned int VAO, VBO, EBO;
        std::vector<Glb::Vertex> vertices;
        std::vector<uint16_t> indices;
        std::string texture;
};