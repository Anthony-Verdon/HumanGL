#pragma once

#include <string>
#include "GlbParser/GlbParser.hpp"
#include "Model/Model.hpp"

namespace ModelLoader
{
    Model LoadModel(const std::string &path);
    Model LoadModelFromGlb(const std::string &path);

    void PrintNode(const Glb::GltfData &data, int nodeIndex);
    void PrintSkin(const Glb::GltfData &data, int skinIndex);
    void PrintMesh(const Glb::GltfData &data, int meshIndex);
    void PrintMaterial(const Glb::GltfData &data, int materialIndex);
    void PrintImage(const Glb::GltfData &data, int imageIndex);

}