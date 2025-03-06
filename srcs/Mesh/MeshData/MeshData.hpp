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

        void SetLocalTransform(const AlgOps::mat4 &localTransform) { this->localTransform = localTransform; }
        AlgOps::mat4 GetLocalTransfrom() const { return (localTransform); }

        void SetTexture(const std::string &texture) { this->texture = texture; }
        std::string GetTexture() const { return (texture); }
    
    protected:
        std::vector<float> vertices;
        std::vector<unsigned short> indices;
        AlgOps::mat4 localTransform;
        
        // material
        std::string texture;
};