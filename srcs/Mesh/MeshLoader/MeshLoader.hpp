#pragma once

#include <string>
#include "Mesh/MeshData/MeshData.hpp"
#include "Mesh/MeshRenderer/MeshRenderer.hpp"

namespace MeshLoader
{
    std::vector<MeshRenderer> LoadMesh(const std::string &path);
    std::vector<MeshData> LoadMeshDataFromGlb(const std::string &path);
}