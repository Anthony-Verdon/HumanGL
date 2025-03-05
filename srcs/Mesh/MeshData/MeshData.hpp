#pragma once

#include <vector>
#include "Matrix/Matrix.hpp"

class MeshData
{
    public:
        MeshData();
        MeshData(const MeshData &instance);
        MeshData &operator=(const MeshData &instance);
        ~MeshData();

        void SetVertices(const std::vector<float> &vertices) { this->vertices = vertices; }
        std::vector<float> GetVertices() const { return (vertices); }

        void SetIndices(const std::vector<unsigned short> &indices) { this->indices = indices; }
        std::vector<unsigned short> GetIndices() const { return (indices); }
    
        AlgOps::vec4 quat;
    protected:
        std::vector<float> vertices;
        std::vector<unsigned short> indices;
};