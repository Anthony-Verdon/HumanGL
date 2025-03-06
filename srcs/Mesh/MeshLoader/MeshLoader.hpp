#pragma once

#include <string>
#include "Mesh/MeshData/MeshData.hpp"
#include "Mesh/MeshRenderer/MeshRenderer.hpp"
#include "JsonParser/JsonValue.hpp"

namespace MeshLoader
{
    std::vector<MeshRenderer> LoadMesh(const std::string &path);
    std::vector<MeshData> LoadMeshDataFromGlb(const std::string &path);
    std::vector<MeshData> LoadNode(JsonParser::JsonValue &gltfJson, const std::string &binStr, size_t nodeIndex);
    MeshData LoadMesh(JsonParser::JsonValue &gltfJson, const std::string &binStr, JsonParser::JsonValue &node);
}