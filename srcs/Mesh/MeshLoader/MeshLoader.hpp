#pragma once

#include <string>
#include "Mesh/MeshData/MeshData.hpp"

namespace MeshLoader
{
    MeshData LoadMesh(const std::string &path);
    void LoadMeshFromGlb(const std::string &path);
}