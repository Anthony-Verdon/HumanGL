#pragma once

#include <vector>


class MeshData
{
    public:
        MeshData();
        ~MeshData();

        void SetVertices(const std::vector<float> &vertices) { this->vertices = vertices; }
    
    protected:
        std::vector<float> vertices;
};