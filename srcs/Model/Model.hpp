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
        void Draw(const ml::vec3 &camPos, const Light &light, const ml::mat4 &projection, const ml::mat4 &view);
        std::vector<std::string> GetAnimations() const;
        void SetAnimation(const std::string &name);
    private:
        void LoadMesh(const Glb::GltfData &data, size_t nodeIndex);
        void LoadAnimations(const Glb::GltfData &data);
        std::map<int, ml::mat4> CalculateNodeTransform(const Glb::GltfData &data, size_t nodeIndex, const ml::mat4 &parentTransform) const; 

        Glb::GltfData data;
        size_t nodeIndex;

        std::vector<Mesh> meshes;
        Animator animator;

};