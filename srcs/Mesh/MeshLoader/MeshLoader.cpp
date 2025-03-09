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
        data.SetID(-1);
        data.SetParent(nullptr);
        AlgOps::mat4 model;
        model.identity();
        data.SetLocalTransform(model);
        data.SetGlobalTransform(model);

        for (size_t nodeIndex: rootScene["nodes"])
            data.AddChild(LoadNode(gltfJson, binStr, nodeIndex, &data));
        
        return (data);
    }

    MeshData LoadNode(JsonParser::JsonValue &gltfJson, const std::string &binStr, size_t nodeIndex, MeshData *parent)
    {
        MeshData data;
        
        auto node = gltfJson["nodes"][nodeIndex];
        
        data.SetName(node["name"]);
        data.SetID(nodeIndex);
        data.SetParent(parent);
        data.SetLocalTransform(CalculateLocalTransform(node));
        data.SetGlobalTransform(parent->GetGlobalTransfrom() * data.GetLocalTransfrom());
        if (node.KeyExist("children"))
        {
            for (size_t child : node["children"])
                data.AddChild(LoadNode(gltfJson, binStr, child, &data));
        }

        if (node.KeyExist("mesh"))
            LoadMesh(data, gltfJson, binStr, node["mesh"]);

        if (node.KeyExist("skin"))
            LoadSkin(data, gltfJson, binStr, node["skin"]);
            
        return (data);
    }

    AlgOps::mat4 CalculateLocalTransform(JsonParser::JsonValue &node)
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
        
        return (model);
    }

    void LoadMesh(MeshData &data, JsonParser::JsonValue &gltfJson, const std::string &binStr, int meshIndex)
    {
        auto mesh = gltfJson["meshes"][meshIndex];
        auto primitives = mesh["primitives"][0];

        LoadVertices(data, gltfJson, binStr, primitives["attributes"]);
        LoadIndices(data, gltfJson, binStr, primitives["indices"]);
        if (primitives.KeyExist("material"))
            LoadMaterial(data, gltfJson, binStr, primitives["material"]);
    }

    void LoadVertices(MeshData &data, JsonParser::JsonValue &gltfJson, const std::string &binStr, JsonParser::JsonValue &attributes)
    {
        std::vector<float> positions;
        std::vector<float> textureCoords;
        std::vector<float> normals;
        std::vector<uint16_t> joints;
        std::vector<float> weights;

        for (auto it = attributes.begin(); it != attributes.end(); it++)
        {
            auto accessor = gltfJson["accessors"][it.value()];
            size_t bufferViewIndex = accessor["bufferView"];
            size_t count = accessor["count"];
            size_t componentType = accessor["componentType"];
            std::string type = accessor["type"];
            
            size_t nbFloat = 0;
            if (type == "VEC2")
                nbFloat = 2;
            else if (type == "VEC3")
                nbFloat = 3;
            else if (type == "VEC4")
                nbFloat = 4;
            else
                std::cerr << "type unknown: " << type << std::endl;
            
            auto bufferView = gltfJson["bufferViews"][bufferViewIndex];
            size_t byteOffset = bufferView["byteOffset"];
            void* buffer = (void*)(binStr.data() + byteOffset);

            size_t size = count * nbFloat;
            if (it.key() == "POSITION")
            {
                float *data = (float*)buffer;
                positions.reserve(size);
                for (size_t i = 0; i < size; i++)
                    positions.push_back(data[i]);
            }
            else if (it.key() == "TEXCOORD_0")
            {
                float *data = (float*)buffer;
                textureCoords.reserve(size);
                for (size_t i = 0; i < size; i++)
                    textureCoords.push_back(data[i]);
            }
            else if (it.key() == "NORMAL")
            {
                float *data = (float*)buffer;
                normals.reserve(size);
                for (size_t i = 0; i < size; i++)
                    normals.push_back(data[i]);
            }
            else if (it.key() == "JOINTS_0")
            {
                joints.reserve(size);
                if (componentType == GL_UNSIGNED_BYTE)
                {
                    uint8_t *data = (uint8_t*)buffer;
                    for (size_t i = 0; i < size; i++)
                        joints.push_back((uint16_t)data[i]);
                }
                else if (componentType == GL_UNSIGNED_SHORT)
                {
                    uint16_t *data = (uint16_t*)buffer;
                    for (size_t i = 0; i < size; i++)
                        joints.push_back(data[i]);
                }
            }
            else if (it.key() == "WEIGHTS_0")
            {
                float *data = (float*)buffer;
                weights.reserve(size);
                for (size_t i = 0; i < size; i++)
                    weights.push_back(data[i]);
            }
        }
        
        size_t count = positions.size() / 3;
        std::vector<VertexStruct> vertices(count);
        for (size_t i = 0; i < count; i++)
        {
            vertices[i].x = positions[i * nbFloatPerPosition + 0];
            vertices[i].y = positions[i * nbFloatPerPosition + 1];
            vertices[i].z = positions[i * nbFloatPerPosition + 2];
            if (textureCoords.size() != 0)
            {
                vertices[i].u = textureCoords[i * nbFloatPerTexCoord + 0];
                vertices[i].v = textureCoords[i * nbFloatPerTexCoord + 1];
            }
            if (normals.size() != 0)
            {
                vertices[i].nx = normals[i * nbFloatPerNormal + 0];
                vertices[i].ny = normals[i * nbFloatPerNormal + 1];
                vertices[i].nz = normals[i * nbFloatPerNormal + 2];
            }
            if (joints.size() != 0)
            {
                vertices[i].j1 = joints[i * nbFloatPerJoint + 0];
                vertices[i].j2 = joints[i * nbFloatPerJoint + 1];
                vertices[i].j3 = joints[i * nbFloatPerJoint + 2];
                vertices[i].j4 = joints[i * nbFloatPerJoint + 3];
            }
            if (weights.size() != 0)
            {
                vertices[i].w1 = weights[i * nbFloatPerWeight + 0];
                vertices[i].w2 = weights[i * nbFloatPerWeight + 1];
                vertices[i].w3 = weights[i * nbFloatPerWeight + 2];
                vertices[i].w4 = weights[i * nbFloatPerWeight + 3];
            }

        }
        data.SetVertices(vertices);
    }

    void LoadIndices(MeshData &data, JsonParser::JsonValue &gltfJson, const std::string &binStr, int indiceIndex)
    {
        auto accessor = gltfJson["accessors"][indiceIndex];
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

    void LoadMaterial(MeshData &data, JsonParser::JsonValue &gltfJson, const std::string &binStr, int materialIndex)
    {
        // need to complete parsing
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

    void LoadSkin(MeshData &data, JsonParser::JsonValue &gltfJson, const std::string &binStr, int skinIndex)
    {
        auto skin = gltfJson["skins"][skinIndex];
        size_t matrixIndex = skin["inverseBindMatrices"];

        auto accessor = gltfJson["accessors"][matrixIndex];
        size_t bufferViewIndex = accessor["bufferView"];
        size_t count = accessor["count"];
        
        auto bufferView = gltfJson["bufferViews"][bufferViewIndex];
        size_t byteOffset = bufferView["byteOffset"];
        float* buffer = (float*)(binStr.data() + byteOffset);
        
        size_t nbFloat = 16;
        std::vector<AlgOps::mat4> matrices;
        matrices.reserve(count);
        for (size_t i = 0; i < count; i++)
        {
            AlgOps::mat4 matrix;
            for (size_t j = 0; j < nbFloat; j++)
                matrix.setData(j % 4, j / 4, buffer[i * nbFloat + j]);
            matrices.push_back(matrix);
        }

        size_t i = 0;
        for (auto joint: skin["joints"])
        {
            data.AddJoint(joint, matrices[i]);
            i++;
        }
    }
}