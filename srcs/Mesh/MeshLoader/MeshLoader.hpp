#pragma once

#include <string>
#include "Mesh/MeshData/MeshData.hpp"
#include "Mesh/MeshRenderer/MeshRenderer.hpp"
#include "JsonParser/JsonValue.hpp"
#include "GlbParser/GlbParser.hpp"

namespace MeshLoader
{
    MeshRenderer LoadMesh(const std::string &path);
    
    MeshData LoadMeshDataFromGlb(const std::string &path);

    void PrintNode(const Glb::GltfData &data, int nodeIndex);
    void PrintSkin(const Glb::GltfData &data, int skinIndex);
    void PrintMesh(const Glb::GltfData &data, int meshIndex);
    void PrintMaterial(const Glb::GltfData &data, int materialIndex);
    void PrintImage(const Glb::GltfData &data, int imageIndex);

}