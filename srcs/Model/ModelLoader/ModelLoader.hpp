#pragma once

#include <string>
#include "GlbParser/GlbParser.hpp"
#include "Model/Model.hpp"

namespace ModelLoader
{
    std::vector<Model> LoadModel(const std::string &path);
    std::vector<Model> LoadModelFromGlb(const std::string &path);
    std::vector<Model> LoadModelFromGltf(const std::string &path);
}