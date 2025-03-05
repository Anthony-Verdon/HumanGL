#pragma once

#include <string>
#include "Mesh/MeshData/MeshData.hpp"

namespace MeshLoader
{
    std::vector<MeshData> LoadMesh(const std::string &path);
    std::vector<MeshData> LoadMeshFromGlb(const std::string &path);
}