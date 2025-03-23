#include "Model/Mesh/Mesh.hpp"
#include "RessourceManager/RessourceManager.hpp"
#include "Toolbox.hpp"
#include <cmath>
#include <glad/glad.h>

Mesh::Mesh(const Glb::GltfData &data, size_t nodeIndex)
{
    this->nodeIndex = nodeIndex;
    auto node = data.nodes[nodeIndex];
    auto mesh = data.meshes[node.mesh];
    name = mesh.name;
    primitives = mesh.primitives;
    for (size_t i = 0; i < primitives.size(); i++)
    {
        int materialIndex = primitives[i].material;
        if (materialIndex == -1)
            continue;

        auto material = data.materials[materialIndex];
        materials[materialIndex] = material;
        
        int imageIndex = material.pbr.baseColorTexture;
        if (imageIndex == -1)
            continue;

        auto image = data.images[imageIndex];
        baseColorTextures[imageIndex] = image.name;
        if (!RessourceManager::TextureExist(image.name))
            RessourceManager::AddTexture(image.name, image.buffer, image.bufferLength);

    }
    if (node.skin != -1)
    {   
        auto skin = data.skins[node.skin];
        joints = skin.joints;
    }

    VAO = 0;
    EBO = 0;
}

Mesh::~Mesh()
{
}


void Mesh::Init()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    size_t totalNbIndices = 0;
    for (size_t i = 0; i < primitives.size(); i++)
    {
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(Glb::Vertex) * primitives[i].vertices.size(), primitives[i].vertices.data(), GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, Glb::nbFloatPerPosition, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, Glb::nbFloatPerTexCoord, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, Glb::nbFloatPerNormal, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 5));
        glEnableVertexAttribArray(2);
        
        glVertexAttribPointer(3, Glb::nbFloatPerJoint, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 8));
        glEnableVertexAttribArray(3);
        
        glVertexAttribPointer(4, Glb::nbFloatPerWeight, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 8 + sizeof(uint16_t) * 4));
        glEnableVertexAttribArray(4);

        VBOs.push_back(VBO);
        totalNbIndices += primitives[i].indices.size();
    }
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * totalNbIndices, nullptr, GL_STATIC_DRAW);
    totalNbIndices = 0;
    for (size_t i = 0; i < primitives.size(); i++)
    {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * totalNbIndices, sizeof(unsigned short) * primitives[i].indices.size(), primitives[i].indices.data());
        totalNbIndices += primitives[i].indices.size();
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Destroy()
{
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(VBOs.size(), VBOs.data());
        glDeleteBuffers(1, &EBO);
        VAO = 0;
    }
}

void Mesh::Draw(const ml::vec3 &camPos, const ml::vec3 &camDir, const Light lights[4], const ml::mat4 &projection, const ml::mat4 &view, std::map<int, ml::mat4> &nodesTransform)
{
    auto shader = RessourceManager::GetShader("mesh_shader");
    shader->use();
    // vs
    shader->setMat4("uProjection", projection);
    shader->setMat4("uView", view);
    shader->setMat4("uModel", nodesTransform[nodeIndex]);
    shader->setInt("uUseJoints", (joints.size() != 0));
    for (size_t i = 0; i < joints.size(); i++)
        shader->setMat4("uJointMat[" + std::to_string(i) + "]", nodesTransform[joints[i].nodeIndex] * joints[i].inverseBindMatrix);
    
    // fs
    shader->setVec3("uCamPos", camPos);
    for (size_t i = 0; i < 4; i++)
    {
        shader->setVec3("uPointLights[" + std::to_string(i) + "].pos", lights[i].GetPos());
        shader->setVec3("uPointLights[" + std::to_string(i) + "].color", lights[i].GetColor());
        shader->setFloat("uPointLights[" + std::to_string(i) + "].intensity", lights[i].GetIntensity());
    }
    shader->setVec3("uDirectionalLight.direction", ml::vec3(0, -1, 0));
    shader->setVec3("uDirectionalLight.color", ml::vec3(1, 1, 0));
    shader->setFloat("uDirectionalLight.intensity", 3);

    shader->setVec3("uSpotLight.position", camPos);
    shader->setVec3("uSpotLight.direction", camDir);
    shader->setFloat("uSpotLight.cutOff", cosf(Toolbox::DegToRad(12.5f)));
    shader->setFloat("uSpotLight.outerCutOff", cosf(Toolbox::DegToRad(17.5f)));
    shader->setVec3("uSpotLight.color", ml::vec3(0, 1, 1));
    shader->setFloat("uSpotLight.intensity", 15);

    for (size_t i = 0; i < materials.size(); i++)
    {
        shader->setVec4("uMaterials[" + std::to_string(i) + "].baseColorFactor", materials[i].pbr.baseColorFactor);
        shader->setVec3("uMaterials[" + std::to_string(i) + "].emissiveFactor", materials[i].emissiveFactor);
        shader->setFloat("uMaterials[" + std::to_string(i) + "].metallicFactor", materials[i].pbr.metallicFactor);
        shader->setFloat("uMaterials[" + std::to_string(i) + "].roughnessFactor", materials[i].pbr.roughnessFactor);
        shader->setFloat("uMaterials[" + std::to_string(i) + "].ambientOcclusion", 1.0);
    }
    
    glBindVertexArray(VAO);
    size_t totalNbIndices = 0;
    for (size_t i = 0; i < primitives.size(); i++)
    {
        int materialIndex = primitives[i].material;
        if (materialIndex != -1)
        {
            shader->setInt("uMaterialIndex", materialIndex);
            int baseColorTextureIndex = materials[materialIndex].pbr.baseColorTexture;
            bool useBaseColorTexture = (baseColorTextureIndex != -1);
            shader->setInt("uUseBaseColorTexture", useBaseColorTexture);
            if (useBaseColorTexture)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, RessourceManager::GetTexture(baseColorTextures[baseColorTextureIndex])->getID());
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);

        glVertexAttribPointer(0, Glb::nbFloatPerPosition, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, Glb::nbFloatPerTexCoord, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, Glb::nbFloatPerNormal, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 5));
        glEnableVertexAttribArray(2);
        
        glVertexAttribPointer(3, Glb::nbFloatPerJoint, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 8));
        glEnableVertexAttribArray(3);
        
        glVertexAttribPointer(4, Glb::nbFloatPerWeight, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 8 + sizeof(uint16_t) * 4));
        glEnableVertexAttribArray(4);

        glDrawElements(GL_TRIANGLES, primitives[i].indices.size(), GL_UNSIGNED_SHORT, (void *)(totalNbIndices * sizeof(unsigned short)));
        totalNbIndices += primitives[i].indices.size();
    }
}