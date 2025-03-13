#pragma once

#include "GlbParser/GlbParser.hpp"
#include "Model/Animator.hpp"
#include "Model/Mesh.hpp"

class Model
{
    public:
        Model();
        Model(const Glb::GltfData &data, size_t nodeIndex);
        ~Model();

        void Init();
        void Draw(const glm::vec3 &camPos, const Light &light, const glm::mat4 &projection, const glm::mat4 &view);
        
    private:
        void LoadMesh(const Glb::GltfData &data, size_t nodeIndex);
        void LoadAnimations(const Glb::GltfData &data);
        std::map<int, glm::mat4> CalculateNodeTransform(const Glb::GltfData &data, size_t nodeIndex, const glm::mat4 &parentTransform) const; 

        Glb::GltfData data;
        size_t nodeIndex;

        std::vector<Mesh> meshes;
        Animator animator;

};