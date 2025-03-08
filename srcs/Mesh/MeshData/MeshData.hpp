#pragma once

#include <vector>
#include "Matrix/Matrix.hpp"
#include <map>

struct VertexStruct
{
    float x,y,z;
    float u,v;
    float nx, ny, nz;
    uint16_t j1, j2, j3, j4;
    float w1, w2, w3, w4;
};

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

        void SetVertices(const std::vector<VertexStruct> &vertices) { this->vertices = vertices; }
        std::vector<VertexStruct> GetVertices() const { return (vertices); }

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

        void AddJoint(int ID, const AlgOps::mat4 &joint) { joints[ID] = joint; }
        std::map<int, AlgOps::mat4> GetJoints() const { return (joints); }
    
    protected:
        std::string name;
        std::vector<VertexStruct> vertices;
        std::vector<unsigned short> indices;
        AlgOps::mat4 localTransform;
        std::vector<MeshData> children;
        std::map<int, AlgOps::mat4> joints;
        std::string texture;
};