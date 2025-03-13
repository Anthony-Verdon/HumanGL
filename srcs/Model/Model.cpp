#include "Model/Model.hpp"

Model::Model()
{
}

Model::Model(const Glb::GltfData &data, size_t nodeIndex)
{
    this->data = data;
    this->nodeIndex = nodeIndex;
    LoadMesh(data, nodeIndex);
    LoadAnimations(data);
    animator.Play("Idle_Combat");
}

Model::~Model()
{

}

void Model::LoadMesh(const Glb::GltfData &data, size_t nodeIndex)
{
    auto node = data.nodes[nodeIndex];

    if (node.mesh != -1)
        meshes.push_back({data, nodeIndex});

    for (size_t i = 0; i < node.children.size(); i++)
        LoadMesh(data, node.children[i]);
}

void Model::LoadAnimations(const Glb::GltfData &data)
{
    for (size_t i = 0; i < data.animations.size(); i++)
        animator.LoadAnimation(data.animations[i]);
}

void Model::Init()
{
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].Init();
}

void Model::Draw(const glm::vec3 &camPos, const Light &light, const glm::mat4 &projection, const glm::mat4 &view)
{
    animator.Update();
    glm::mat4 transform(1.0f);
    auto nodesTransform = CalculateNodeTransform(data, nodeIndex, transform);
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].Draw(camPos, light, projection, view, nodesTransform);
}

std::map<int, glm::mat4> Model::CalculateNodeTransform(const Glb::GltfData &data, size_t nodeIndex, const glm::mat4 &parentTransform) const
{
    auto node = data.nodes[nodeIndex];
    auto transform = parentTransform * animator.GetNodeTransform(nodeIndex);
    std::map<int, glm::mat4> nodesTransform;
    nodesTransform[nodeIndex] = transform;

    for (size_t i = 0; i < node.children.size(); i++)
        nodesTransform.merge(CalculateNodeTransform(data, node.children[i], transform));

    return (nodesTransform);
}