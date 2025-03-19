#include "ModelLoader/ModelLoader.hpp"
#include "GlbParser/GlbParser.hpp"
#include "RessourceManager/RessourceManager.hpp"
#include "Toolbox.hpp"
#include <iostream>

namespace ModelLoader
{
    std::vector<Model> LoadModel(const std::string &path)
    {
        if (Toolbox::checkExtension(path, ".glb"))
            return (LoadModelFromGlb(path));
        
        throw(std::runtime_error("unknown extension"));
    }

    std::vector<Model> LoadModelFromGlb(const std::string &path)
    {
        auto [gltfJson, binStr] = Glb::LoadBinaryFile(path, true);

        Glb::GltfData data = Glb::LoadGltf(gltfJson, binStr);
        Glb::Scene scene = data.scenes[data.rootScene];

        std::vector<Model> models;
        for (size_t i = 0; i < scene.nodes.size(); i++)
            models.push_back({data, (size_t)scene.nodes[i]});

        return (models);
    }

    void PrintNode(const Glb::GltfData &data, int nodeIndex)
    {
        auto node = data.nodes[nodeIndex];
        std::cout << nodeIndex << " " << node.name << std::endl;
        //std::cout << "transform: " << node.transform << std::endl;
        std::cout << "skin: " << node.skin << std::endl;
        if (node.skin != -1)
            PrintSkin(data, node.skin);
        std::cout << "mesh: " << node.mesh << std::endl;
        if (node.mesh != -1)
            PrintMesh(data, node.mesh);
        for (size_t i = 0; i < node.children.size(); i++)
            PrintNode(data, node.children[i]);
    }

    void PrintSkin(const Glb::GltfData &data, int skinIndex)
    {
        auto skin = data.skins[skinIndex];
        std::cout << skin.name << std::endl;
        std::cout << skin.joints.size() << std::endl;
    }

    void PrintMesh(const Glb::GltfData &data, int meshIndex)
    {
        auto mesh = data.meshes[meshIndex];
        std::cout << mesh.name << std::endl;
        std::cout << "nb vertices: " << mesh.vertices.size() << std::endl;
        std::cout << "nb indices: " << mesh.indices.size() << std::endl;
        std::cout << "material: " << mesh.material << std::endl;
        if (mesh.material != -1)
            PrintMaterial(data, mesh.material);
    }

    void PrintMaterial(const Glb::GltfData &data, int materialIndex)
    {
        auto material = data.materials[materialIndex];
        std::cout << material.name << std::endl;
        std::cout << "image: " << material.pbr.baseColorTexture << std::endl;
        if (material.pbr.baseColorTexture != -1)
            PrintImage(data, material.pbr.baseColorTexture);
    }

    void PrintImage(const Glb::GltfData &data, int imageIndex)
    {
        auto image = data.images[imageIndex];
        std::cout << image.name << std::endl;
    }
}