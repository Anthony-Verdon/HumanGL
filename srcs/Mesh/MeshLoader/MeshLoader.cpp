#include "Mesh/MeshLoader/MeshLoader.hpp"
#include "GlbParser/GlbParser.hpp"
#include "Utils/Utils.hpp"
#include "Matrix/Matrix.hpp"
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
        size_t rootSceneIndex = gltfJson["scene"];
        auto rootScene = gltfJson["scenes"][rootSceneIndex];

        for (size_t nodeIndex: rootScene["nodes"])
        {
            auto node = gltfJson["nodes"][nodeIndex];
            size_t meshIndex = node["mesh"];
            auto mesh = gltfJson["meshes"][meshIndex];
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

            {
                size_t indicesIndex = mesh["primitives"][0]["indices"];
                auto accessor = gltfJson["accessors"][indicesIndex];
                size_t bufferViewIndex = accessor["bufferView"];
                size_t count = accessor["count"];
                
                auto bufferView = gltfJson["bufferViews"][bufferViewIndex];
                size_t byteOffset = bufferView["byteOffset"];
                unsigned short* buffer = (unsigned short*)(binStr.data() + byteOffset);
                std::vector<unsigned short> indices;
                for (size_t i = 0; i < count; i++)
                    indices.push_back(buffer[i]);
                data.SetIndices(indices);
            }

            AlgOps::vec3 scale;
            scale.uniform(1);
            if (node.KeyExist("scale"))
                scale = {node["scale"][0], node["scale"][1], node["scale"][2]};

            AlgOps::vec3 translate;
            translate.uniform(0);
            if (node.KeyExist("translation"))
                translate = {node["translation"][0], node["translation"][1], node["translation"][2]};

            AlgOps::vec4 quat;
            quat.uniform(0);
            if (node.KeyExist("rotation"))
                quat = {node["rotation"][0], node["rotation"][1], node["rotation"][2], node["rotation"][3]};

            float roll = atan2(2 * (quat.getW() * quat.getX() + quat.getY() * quat.getZ()), 1 - 2 * (pow(quat.getX(), 2) + pow(quat.getX(), 2)));
            float pitch = asin(2 * (quat.getW() * quat.getY() - quat.getZ() * quat.getX()));
            float yaw = atan2(2 * (quat.getW() * quat.getZ() + quat.getX() * quat.getY()), 1 - 2 * (pow(quat.getY(), 2) + pow(quat.getZ(), 2)));

            AlgOps::vec3 axis[3];
            axis[0] = {1, 0, 0};
            axis[1] = {0, 1, 0};
            axis[2] = {0, 0, 1};
            
            AlgOps::mat4 rotate;
            rotate.uniform(1);
            rotate = AlgOps::rotate(rotate, roll, axis[0]) *
                    AlgOps::rotate(rotate, pitch, axis[1]) *
                    AlgOps::rotate(rotate, yaw, axis[2]);

            data.SetInitialRotation(rotate);

            size_t count = v.size() / 3;
            std::vector<float> vector;
            vector.reserve(count * (3 + 2 + 3));
            for (size_t i = 0; i < count; i++)
            {
                vector.push_back(v[i * 3 + 0] * scale.getX() + translate.getX());
                vector.push_back(v[i * 3 + 1] * scale.getY() + translate.getY());
                vector.push_back(v[i * 3 + 2] * scale.getZ() + translate.getY());
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