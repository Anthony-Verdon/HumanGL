#pragma once

#include <string>
#include "Mesh/MeshData/MeshData.hpp"
#include "Mesh/MeshRenderer/MeshRenderer.hpp"
#include "JsonParser/JsonValue.hpp"

namespace MeshLoader
{
    MeshRenderer LoadMesh(const std::string &path);
    
    MeshData LoadMeshDataFromGlb(const std::string &path);
    MeshData LoadNode(JsonParser::JsonValue &gltfJson, const std::string &binStr, size_t nodeIndex);
    AlgOps::mat4 CalculateLocalTransform(JsonParser::JsonValue &node);
    void LoadMesh(MeshData &data, JsonParser::JsonValue &gltfJson, const std::string &binStr, int meshIndex);
    void LoadVertices(MeshData &data, JsonParser::JsonValue &gltfJson, const std::string &binStr, JsonParser::JsonValue &attributes);
    void LoadIndices(MeshData &data, JsonParser::JsonValue &gltfJson, const std::string &binStr, int indiceIndex);
    void LoadMaterial(MeshData &data, JsonParser::JsonValue &gltfJson, const std::string &binStr, int materialIndex);
    void LoadSkin(MeshData &data, JsonParser::JsonValue &gltfJson, const std::string &binStr, int skinIndex);
}