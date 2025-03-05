#pragma once

#include "Mesh/MeshData/MeshData.hpp"

class MeshRenderer: public MeshData
{
    public:
    MeshRenderer(const MeshData &data);
    MeshRenderer(const MeshRenderer &instance);
    MeshRenderer &operator=(const MeshRenderer &instance);
    ~MeshRenderer();

    void Draw() const;
    
    private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        void InitRenderer();
        void DestroyRenderer();
};