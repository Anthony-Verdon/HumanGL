#pragma once

#include <vector>
#include "Matrix/Matrix.hpp"

constexpr int nbFloatPerPosition = 3;
constexpr int nbFloatPerTexCoord = 2;
constexpr int nbFloatPerNormal = 3;
constexpr int nbFloatPerJoint = 4;
constexpr int nbFloatPerWeight = 4;
constexpr int nbFloatPerVertex = nbFloatPerPosition + nbFloatPerTexCoord + nbFloatPerNormal + nbFloatPerJoint + nbFloatPerWeight;

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

        void AddChild(const MeshData &child) { children.push_back(child); }
        std::vector<MeshData> GetChildren() const { return (children); }

        void SetTexture(const std::string &texture) { this->texture = texture; }
        std::string GetTexture() const { return (texture); }

        void SetName(const std::string &name) { this->name = name; }
        std::string GetName() const { return (name); }
    
    protected:
        std::string name;
        std::vector<float> vertices;
        std::vector<unsigned short> indices;
        AlgOps::mat4 localTransform;
        std::vector<MeshData> children;

        // material
        std::string texture;
};