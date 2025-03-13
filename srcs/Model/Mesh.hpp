#pragma once

#include "GlbParser/GlbParser.hpp"

class Mesh
{
    public:
        Mesh(const Glb::GltfData &data, size_t nodeIndex);
        ~Mesh();

        void Init();
        void Draw(const glm::mat4 &projection, const glm::mat4 &view, std::map<int, glm::mat4> &nodesTransform) const;

    private:
        unsigned int VAO, VBO, EBO;
        size_t nodeIndex;
        std::string name;
        std::vector<Glb::Vertex> vertices;
        std::vector<uint16_t> indices;
        std::vector<Glb::Joint> joints;
        Glb::GltfData data;

        // material
        std::string baseColorTexture;
        glm::vec4 baseColorFactor;
        glm::vec3 emissiveFactor;
        float metallicFactor;
        float roughnessFactor;
};