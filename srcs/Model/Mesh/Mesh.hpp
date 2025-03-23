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
        void Draw(const ml::vec3 &camPos, const ml::vec3 &camDir, const Light lights[4], const ml::mat4 &projection, const ml::mat4 &view, std::map<int, ml::mat4> &nodesTransform);

    private:
        std::string name;
        unsigned int VAO;
        std::vector<unsigned int> VBOs;
        unsigned int EBO;
        std::vector<Glb::Primitive> primitives;
        std::map<int, Glb::Material> materials;
        std::map<int, std::string> baseColorTextures;
        std::vector<Glb::Joint> joints;
        
        size_t nodeIndex;
};