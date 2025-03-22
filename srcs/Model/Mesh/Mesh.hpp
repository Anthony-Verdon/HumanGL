#pragma once

#include "GlbParser/GlbParser.hpp"
#include "Light/Light.hpp"

class Mesh
{
    public:
        Mesh(const Glb::GltfData &data, size_t nodeIndex);
        ~Mesh();

        void Init();
        void Destroy();
        void Draw(const ml::vec3 &camPos, const ml::vec3 &camDir, const Light lights[4], const ml::mat4 &projection, const ml::mat4 &view, std::map<int, ml::mat4> &nodesTransform) const;

    private:
        unsigned int VAO, VBO, EBO;
        size_t nodeIndex;
        std::string name;
        std::vector<Glb::Vertex> vertices;
        std::vector<uint16_t> indices;
        std::vector<Glb::Joint> joints;

        // material
        std::string baseColorTexture;
        ml::vec4 baseColorFactor;
        ml::vec3 emissiveFactor;
        float metallicFactor;
        float roughnessFactor;
};