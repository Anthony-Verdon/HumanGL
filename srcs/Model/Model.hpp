#pragma once

#include "GlbParser/GlbParser.hpp"
#include "Model/Animator/Animator.hpp"
#include "Model/Mesh/Mesh.hpp"

class Model;

struct NodeModel
{
    std::string name;
    ml::mat4 transform;
    std::vector<int> children;
    std::vector<int> models;
};

class Model
{
    public:
        Model();
        Model(const Glb::GltfData &data, size_t nodeIndex);
        ~Model();

        void Init();
        void Destroy();
        void Draw(const ml::vec3 &camPos, const ml::vec3 &camDir, const Light lights[4], const ml::mat4 &projection, const ml::mat4 &view, const ml::mat4 &initTransform);
        void DrawSubModels(size_t nodeIndex, const ml::vec3 &camPos, const ml::vec3 &camDir, const Light lights[4], const ml::mat4 &projection, const ml::mat4 &view, std::map<int, ml::mat4> &nodesTransform);
        
        std::vector<std::string> GetAnimationsName() const { return (animator.GetAnimationsName()); }
        void Play(const std::string &name) { animator.Play(name); }

        std::map<int, NodeModel> &GetNodes() { return (nodes); }
        size_t GetRootIndex() { return (nodeIndex); }
        
    private:
        void LoadMesh(const Glb::GltfData &data, size_t nodeIndex);
        void LoadAnimations(const Glb::GltfData &data);
        std::map<int, ml::mat4> CalculateNodeTransform(size_t nodeIndex, const ml::mat4 &parentTransform);

        size_t nodeIndex;
        std::map<int, NodeModel> nodes;

        std::vector<Mesh> meshes;
        Animator animator;

};