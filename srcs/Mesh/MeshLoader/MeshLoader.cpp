#include "Mesh/MeshLoader/MeshLoader.hpp"
#include "GlbParser/GlbParser.hpp"
#include "Utils/Utils.hpp"
#include <iostream>

namespace MeshLoader
{
    MeshData LoadMesh(const std::string &path)
    {
        if (Utils::checkExtension(path, ".glb"))
            LoadMeshFromGlb(path);
        return (MeshData());
    }

    void LoadMeshFromGlb(const std::string &path)
    {
        auto [gltfJson, binStr] = GlbParser::ParseFile(path);

        for (auto mesh: gltfJson["meshes"])
        {
            std::cout << mesh["name"] << std::endl;
            std::cout << mesh["primitives"][0]["attributes"] << std::endl;
            for (auto it = mesh["primitives"][0]["attributes"].begin(); it != mesh["primitives"][0]["attributes"].end(); it++)
            {
                std::cout << it.key() << std::endl;
                auto accessor = gltfJson["accessors"][it.value()];
                size_t bufferViewIndex = accessor["bufferView"];
                size_t count = accessor["count"];
                std::string type = accessor["type"];

                auto bufferView = gltfJson["bufferViews"][bufferViewIndex];
                size_t byteOffset = bufferView["byteOffset"];

                float* buffer = (float*)(binStr.data() + byteOffset);

                if (type == "VEC3")
                {
                    for (size_t i = 0; i < count; ++i)
                        std::cout << "(" << buffer[i * 3] << ", " << buffer[i * 3  + 1] << ", " << buffer[i * 3 + 2] << ")" << std::endl;
                }
                else if (type == "VEC2")
                {
                    for (size_t i = 0; i < count; ++i)
                        std::cout << "(" << buffer[i * 2] << ", " << buffer[i * 2 + 1] << ")" << std::endl;
                }
                else
                {
                    std::cout << "type unknown:" << type << std::endl;
                }
                std::cout << std::endl;
            }
        }
    }
}