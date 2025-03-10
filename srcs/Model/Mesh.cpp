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
    texture = "";
    if (mesh.material != -1)
    {
        int imageIndex = data.materials[mesh.material].image;
        if (imageIndex != -1)
        {
            auto image = data.images[imageIndex];
            texture = image.name;
            if (!RessourceManager::TextureExist(texture))
                RessourceManager::AddTexture(texture, image.buffer, image.bufferLength);
        }
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

AlgOps::mat4 ReverseMatrix(const AlgOps::mat4 &matrix)
{
    AlgOps::mat4 newMatrix;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            newMatrix.setData(i, j, matrix.getData(j, i));
        }
    }

    return (newMatrix);
}

void Mesh::Draw(const AlgOps::mat4 &projection, const AlgOps::mat4 &view, std::map<int, AlgOps::mat4> &nodesTransform) const
{
    auto shader = RessourceManager::GetShader("mesh_shader");
    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", nodesTransform[nodeIndex]);
    shader->setInt("useJoints", (joints.size() != 0));
    for (size_t i = 0; i < joints.size(); i++)
        shader->setMat4("jointMat[" + std::to_string(i) + "]", nodesTransform[joints[i].nodeIndex] * ReverseMatrix(joints[i].inverseBindMatrix));
    bool useTexCoord = (texture != "");
    shader->setInt("useTexCoord", useTexCoord);
    if (useTexCoord)
    {
        glActiveTexture(GL_TEXTURE0);    
        glBindTexture(GL_TEXTURE_2D, RessourceManager::GetTexture(texture)->getID()); 
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
}