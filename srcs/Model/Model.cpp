#include "Model/Model.hpp"

Model::Model()
{
}

Model::Model(const Glb::GltfData &data, size_t nodeIndex)
{
    this->data = data;
    this->nodeIndex = nodeIndex;
    LoadMesh(data, nodeIndex);
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

void Model::Init()
{
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].Init();
}

void Model::Draw(const AlgOps::mat4 &projection, const AlgOps::mat4 &view) const
{
    AlgOps::mat4 transform;
    transform.identity();
    auto nodesTransform = CalculateNodeTransform(data, nodeIndex, transform);
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].Draw(projection, view, nodesTransform);
}

std::map<int, AlgOps::mat4> Model::CalculateNodeTransform(const Glb::GltfData &data, size_t nodeIndex, const AlgOps::mat4 &parentTransform) const
{
    auto node = data.nodes[nodeIndex];
    auto transform = parentTransform * node.transform;
    std::map<int, AlgOps::mat4> nodesTransform;
    nodesTransform[nodeIndex] = transform;

    for (size_t i = 0; i < node.children.size(); i++)
        nodesTransform.merge(CalculateNodeTransform(data, node.children[i], transform));

    return (nodesTransform);
}