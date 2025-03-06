#include "Mesh/MeshLoader/MeshLoader.hpp"
#include "GlbParser/GlbParser.hpp"
#include "RessourceManager/RessourceManager.hpp"
#include "Utils/Utils.hpp"
#include "Matrix/Matrix.hpp"
#include <iostream>

namespace MeshLoader
{
    MeshRenderer LoadMesh(const std::string &path)
    {
        std::vector<MeshData> meshesData;
        if (Utils::checkExtension(path, ".glb"))
            return (LoadMeshDataFromGlb(path));
        
        throw(std::runtime_error("unknown extension"));
    }

    MeshData LoadMeshDataFromGlb(const std::string &path)
    {
        auto [gltfJson, binStr] = GlbParser::ParseFile(path, true);

        MeshData data;
        size_t rootSceneIndex = gltfJson["scene"];
        auto rootScene = gltfJson["scenes"][rootSceneIndex];
        data.SetName(rootScene["name"]);
        AlgOps::mat4 model;
        model.identity();
        data.SetLocalTransform(model);

        for (size_t nodeIndex: rootScene["nodes"])
            data.AddChild(LoadNode(gltfJson, binStr, nodeIndex));
        
        return (data);
    }

    MeshData LoadNode(JsonParser::JsonValue &gltfJson, const std::string &binStr, size_t nodeIndex)
    {
        MeshData data;
        
        auto node = gltfJson["nodes"][nodeIndex];
        data.SetName(node["name"]);
        {
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

            AlgOps::mat4 model;
            model.identity();
            model = AlgOps::translate(model, translate)
                    * rotate
                    * AlgOps::scale(model, scale);  
            
            data.SetLocalTransform(model);
        }

        if (node.KeyExist("children"))
        {
            for (size_t child : node["children"])
                data.AddChild(LoadNode(gltfJson, binStr, child));
        }

        if (node.KeyExist("mesh"))
            LoadMesh(data, gltfJson, binStr, node);
            
        return (data);
    }

    void LoadMesh(MeshData &data, JsonParser::JsonValue &gltfJson, const std::string &binStr, JsonParser::JsonValue &node)
    {
        size_t meshIndex = node["mesh"];
        auto mesh = gltfJson["meshes"][meshIndex];
        data.SetName(mesh["name"]);
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

        size_t materialIndex = mesh["material"];
        auto material = gltfJson["materials"][materialIndex];
        if (material["pbrMetallicRoughness"].KeyExist("baseColorTexture"))
        {
            size_t textureIndex = material["pbrMetallicRoughness"]["baseColorTexture"]["index"];
            size_t imageIndex = gltfJson["textures"][textureIndex]["source"];
            auto image = gltfJson["images"][imageIndex];
            
            std::string texture = image["name"];
            data.SetTexture(texture);
            if (!RessourceManager::TextureExist(texture))
            {
                size_t bufferViewIndex = image["bufferView"];
                auto bufferView = gltfJson["bufferViews"][bufferViewIndex];
                size_t byteOffset = bufferView["byteOffset"];
                size_t byteLength = bufferView["byteLength"];
                
                unsigned char* buffer = (unsigned char*)(binStr.data() + byteOffset);
                
                RessourceManager::AddTexture(image["name"], buffer, byteLength);
            }
        }
        else
        {
            data.SetTexture("");
        }
    }
}