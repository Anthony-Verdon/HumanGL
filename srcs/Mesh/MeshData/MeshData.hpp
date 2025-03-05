#pragma once

#include <vector>


class MeshData
{
    public:
        MeshData();
        ~MeshData();

        void SetVertices(const std::vector<float> &vertices) { this->vertices = vertices; }
    private:
        std::vector<float> vertices;
};