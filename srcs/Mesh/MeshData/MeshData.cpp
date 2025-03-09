#include "Mesh/MeshData/MeshData.hpp"

MeshData::MeshData()
{

}

MeshData::MeshData(const MeshData &instance)
{
    *this = instance;
}

MeshData &MeshData::operator=(const MeshData &instance)
{
    if (this != &instance)
    {
        name = instance.GetName();
        vertices = instance.GetVertices();
        indices = instance.GetIndices();
        localTransform = instance.GetLocalTransfrom();
        globalTransform = instance.GetGlobalTransfrom();
        children = instance.GetChildren();
        texture = instance.GetTexture();
        joints = instance.GetJoints();
        ID = instance.GetID();
        for (size_t i = 0; i < children.size(); i++)
            children[i].SetParent(this);
    }

    return (*this);
}

MeshData::~MeshData()
{
    
}