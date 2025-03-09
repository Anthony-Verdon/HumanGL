#include "Model/Model.hpp"

Model::Model()
{
}

Model::Model(const Glb::GltfData &data, size_t nodeIndex)
{
    AlgOps::mat4 transform;
    transform.identity();
    LoadMesh(data, nodeIndex, transform);
}

Model::~Model()
{

}

void Model::LoadMesh(const Glb::GltfData &data, size_t nodeIndex, const AlgOps::mat4 &parentTransform)
{
    auto node = data.nodes[nodeIndex];
    auto transform = parentTransform * node.transform;

    if (node.mesh != -1)
        meshes.push_back({data, data.meshes[node.mesh], transform});

    for (size_t i = 0; i < node.children.size(); i++)
        LoadMesh(data, node.children[i], transform);
}

void Model::Init()
{
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].Init();
}

void Model::Draw(const AlgOps::mat4 &projection, const AlgOps::mat4 &view) const
{
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].Draw(projection, view);
}
