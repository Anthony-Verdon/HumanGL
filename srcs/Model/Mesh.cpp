#include "Model/Mesh.hpp"
#include "RessourceManager/RessourceManager.hpp"
#include <glad/glad.h>

Mesh::Mesh(const Glb::GltfData &data, size_t nodeIndex)
{
    this->data = data;
    this->nodeIndex = nodeIndex;
    auto node = data.nodes[nodeIndex];
    auto mesh = data.meshes[node.mesh];
    vertices = mesh.vertices;
    name = mesh.name;
    indices = mesh.indices;
    baseColorTexture = "";
    baseColorFactor = glm::vec4(1, 1, 1, 1);
    metallicFactor = 1;
    roughnessFactor = 1;
    if (mesh.material != -1)
    {
        auto material = data.materials[mesh.material];
        int imageIndex = material.pbr.baseColorTexture;
        if (imageIndex != -1)
        {
            auto image = data.images[imageIndex];
            baseColorTexture = image.name;
            if (!RessourceManager::TextureExist(image.name))
                RessourceManager::AddTexture(image.name, image.buffer, image.bufferLength);
        }
        baseColorFactor = material.pbr.baseColorFactor;
        metallicFactor = material.pbr.metallicFactor;
        roughnessFactor = material.pbr.roughnessFactor;
        emissiveFactor = material.emissiveFactor;

    }
    if (node.skin != -1)
    {   
        auto skin = data.skins[node.skin];
        joints = skin.joints;
    }
    VAO = 0;
    VBO = 0;
    EBO = 0;
}

void Mesh::Init()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Glb::Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, Glb::nbFloatPerPosition, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, Glb::nbFloatPerTexCoord, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, Glb::nbFloatPerNormal, GL_FLOAT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 5));
    glEnableVertexAttribArray(2);
    
    glVertexAttribPointer(3, Glb::nbFloatPerJoint, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Glb::Vertex), (void *)(sizeof(float) * 8));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, Glb::nbFloatPerWeight, GL_FLOAT, GL_FALSE,sizeof(Glb::Vertex), (void *)(sizeof(float) * 8 + sizeof(uint16_t) * 4));
    glEnableVertexAttribArray(4);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

void Mesh::Draw(const glm::vec3 &camPos, const Light &light, const glm::mat4 &projection, const glm::mat4 &view, std::map<int, glm::mat4> &nodesTransform) const
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
    shader->setVec3("uLightPos", light.GetPos());
    shader->setVec3("uLightColor", light.GetColor());
    shader->setFloat("uLightIntensity", 15);
    shader->setVec3("uBaseColor", baseColorFactor);
    shader->setVec3("uEmissiveColor", emissiveFactor);
    shader->setFloat("uMetallic", metallicFactor);
    shader->setFloat("uRoughness", roughnessFactor);
    shader->setFloat("uAmbientOcclusion", 1.0);
    bool useBaseColorTexture = (baseColorTexture != "");
    shader->setInt("uUseBaseColorTexture", useBaseColorTexture);
    if (useBaseColorTexture)
    {
        glActiveTexture(GL_TEXTURE0);    
        glBindTexture(GL_TEXTURE_2D, RessourceManager::GetTexture(baseColorTexture)->getID());
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
}