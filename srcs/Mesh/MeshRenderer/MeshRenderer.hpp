#pragma once

#include "Mesh/MeshData/MeshData.hpp"

class MeshRenderer: public MeshData
{
    public:
    MeshRenderer(const MeshData &data);
    ~MeshRenderer();

    void Draw() const;
    
    private:
        unsigned int VAO;
        unsigned int VBO;
};