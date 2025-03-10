#pragma once

#include "GlbParser/GlbParser.hpp"

class Mesh
{
    public:
        Mesh(const Glb::GltfData &data, size_t nodeIndex);
        ~Mesh();

        void Init();
        void Draw(const AlgOps::mat4 &projection, const AlgOps::mat4 &view, std::map<int, AlgOps::mat4> &nodesTransform) const;

    private:
        unsigned int VAO, VBO, EBO;
        size_t nodeIndex;
        std::string name;
        std::vector<Glb::Vertex> vertices;
        std::vector<uint16_t> indices;
        std::string texture;
        std::vector<Glb::Joint> joints;
        Glb::GltfData data;
};