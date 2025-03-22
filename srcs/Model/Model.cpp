#include "Model/Model.hpp"
#include "ModelManager/ModelManager.hpp"

Model::Model()
{
}

Model::Model(const Glb::GltfData &data, size_t nodeIndex)
{
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

    nodes[nodeIndex] = {};
    nodes[nodeIndex].name = node.name;
    for (size_t i = 0; i < node.children.size(); i++)
    {
        nodes[nodeIndex].children.push_back(node.children[i]);
        LoadMesh(data, node.children[i]);
    }
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

void Model::Destroy()
{
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].Destroy();
}

void Model::Draw(const ml::vec3 &camPos, const ml::vec3 &camDir, const Light lights[4], const ml::mat4 &projection, const ml::mat4 &view, const ml::mat4 &initTransform)
{
    animator.Update();
    auto nodesTransform = CalculateNodeTransform(nodeIndex, initTransform);
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].Draw(camPos, camDir, lights, projection, view, nodesTransform);
    DrawSubModels(nodeIndex, camPos, camDir, lights, projection, view, nodesTransform);
}

std::map<int, ml::mat4> Model::CalculateNodeTransform(size_t nodeIndex, const ml::mat4 &parentTransform)
{
    auto node = nodes[nodeIndex];
    auto transform = parentTransform * animator.GetNodeTransform(nodeIndex);
    std::map<int, ml::mat4> nodesTransform;
    nodesTransform[nodeIndex] = transform;

    for (size_t i = 0; i < node.children.size(); i++)
        nodesTransform.merge(CalculateNodeTransform(node.children[i], transform));

    return (nodesTransform);
}

void Model::DrawSubModels(size_t nodeIndex, const ml::vec3 &camPos, const ml::vec3 &camDir, const Light lights[4], const ml::mat4 &projection, const ml::mat4 &view, std::map<int, ml::mat4> &nodesTransform)
{
    auto node = nodes[nodeIndex];
    for (size_t i = 0; i < node.children.size(); i++)
        DrawSubModels(node.children[i], camPos, camDir, lights, projection, view, nodesTransform);
    for (size_t i = 0; i < node.models.size(); i++)
        ModelManager::GetModel(node.models[i]).Draw(camPos, camDir, lights, projection, view, nodesTransform[nodeIndex]);
}

std::vector<std::string> Model::GetAnimations() const
{
    return (animator.GetAnimationsName());
}

void Model::SetAnimation(const std::string &name)
{
    animator.Play(name);
}
