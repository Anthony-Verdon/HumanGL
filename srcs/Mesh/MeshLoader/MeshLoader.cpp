#include "Mesh/MeshLoader/MeshLoader.hpp"
#include "GlbParser/GlbParser.hpp"
#include "Utils/Utils.hpp"
#include <iostream>

namespace MeshLoader
{
    std::vector<MeshRenderer> LoadMesh(const std::string &path)
    {
        std::vector<MeshData> meshesData;
        if (Utils::checkExtension(path, ".glb"))
            meshesData = LoadMeshDataFromGlb(path);
        
        std::vector<MeshRenderer> meshesRenderer;
        for (size_t i = 0; i < meshesData.size(); i++)
            meshesRenderer.push_back(MeshRenderer(meshesData[i]));

        return (meshesRenderer);
    }

    std::vector<MeshData> LoadMeshDataFromGlb(const std::string &path)
    {
        auto [gltfJson, binStr] = GlbParser::ParseFile(path, true);

        std::vector<MeshData> meshes;
        for (auto mesh: gltfJson["meshes"])
        {
            std::vector<float> v;
            std::vector<float> vt;
            std::vector<float> vn;

            for (auto it = mesh["primitives"][0]["attributes"].begin(); it != mesh["primitives"][0]["attributes"].end(); it++)
            {
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

            MeshData data;
            size_t count = v.size() / 3;
            std::vector<float> vector;
            vector.reserve(count * (3 + 2 + 3));
            for (size_t i = 0; i < count; i++)
            {
                vector.push_back(v[i * 3 + 0]);
                vector.push_back(v[i * 3 + 1]);
                vector.push_back(v[i * 3 + 2]);
                vector.push_back(vt[i * 2 + 0]);
                vector.push_back(vt[i * 2 + 1]);
                vector.push_back(vn[i * 3 + 0]);
                vector.push_back(vn[i * 3 + 1]);
                vector.push_back(vn[i * 3 + 2]);
            }
            data.SetVertices(vector);
            meshes.push_back(data);
        }

        return (meshes);
    }
}