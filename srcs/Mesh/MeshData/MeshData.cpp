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
        vertices = instance.GetVertices();
        indices = instance.GetIndices();
        quat = instance.quat;
        for (int i = 0; i < 4; i++)
        {
            std::cout << quat.getData(i, 0) << std::endl;
        }
    }

    return (*this);
}

MeshData::~MeshData()
{
    
}