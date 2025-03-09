#pragma once

#include "GlbParser/GlbParser.hpp"
#include "Model/Mesh.hpp"

class Model
{
    public:
        Model();
        Model(const Glb::GltfData &data, size_t nodeIndex);
        ~Model();

        void Init();
        void LoadMesh(const Glb::GltfData &data, size_t nodeIndex);
        void Draw(const AlgOps::mat4 &projection, const AlgOps::mat4 &view) const;
    
    private:
        Glb::GltfData data;
        Glb::Node node;

        std::vector<Mesh> meshes;

};