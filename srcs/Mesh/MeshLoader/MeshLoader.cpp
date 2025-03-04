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
            std::vector<float> v;
            std::vector<float> vt;
            std::vector<float> vn;

            std::cout << mesh["name"] << std::endl;
            std::cout << mesh["primitives"][0]["attributes"] << std::endl;
            for (auto it = mesh["primitives"][0]["attributes"].begin(); it != mesh["primitives"][0]["attributes"].end(); it++)
            {
                
                std::cout << it.key() << std::endl;
                auto accessor = gltfJson["accessors"][it.value()];
                size_t bufferViewIndex = accessor["bufferView"];
                size_t count = accessor["count"];
                std::string type = accessor["type"];
                
                size_t nbFloat = 0;
                if (type == "VEC2")
                    nbFloat = 2;
                else if (type == "VEC3")
                    nbFloat = 3;
                else
                    std::cout << "type unknown:" << type << std::endl;
                
                auto bufferView = gltfJson["bufferViews"][bufferViewIndex];
                size_t byteOffset = bufferView["byteOffset"];
                float* buffer = (float*)(binStr.data() + byteOffset);

                std::vector<float> vector;
                vector.reserve(count * nbFloat);
                for (size_t i = 0; i < count; i++)
                {
                    for (size_t j = 0; j < nbFloat; j++)
                    {
                        vector.push_back(buffer[i * nbFloat + j]);
                    }
                }

                if (it.key() == "POSITION")
                    v = vector;
                else if (it.key() == "TEXCOORD_0")
                    vt = vector;
                else if (it.key() == "NORMAL")
                    vn = vector;
            }

            for (size_t i = 0; i < v.size(); i += 3)
                std::cout << "(" << v[i] << ", " << v[i + 1] << ", " << v[i + 2] << ")" << std::endl;
            for (size_t i = 0; i < vt.size(); i += 2)
                std::cout << "(" << vt[i] << ", " << vt[i + 1] << ")" << std::endl;
            for (size_t i = 0; i < vn.size(); i += 3)
                std::cout << "(" << vn[i] << ", " << vn[i + 1] << ", " << vn[i + 2] << ")" << std::endl;
        }
    }
}