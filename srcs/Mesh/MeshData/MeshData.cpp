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
        children = instance.GetChildren();
        texture = instance.GetTexture();
    }

    return (*this);
}

MeshData::~MeshData()
{
    
}