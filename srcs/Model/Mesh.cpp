#include "Model/Mesh.hpp"
#include "RessourceManager/RessourceManager.hpp"
#include <glad/glad.h>

Mesh::Mesh(const std::vector<Glb::Vertex> &vertices, const std::vector<uint16_t> &indices)
{
    this->vertices = vertices;
    this->indices = indices;
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

void Mesh::Draw(const AlgOps::mat4 &projection, const AlgOps::mat4 &view) const
{
    auto shader = RessourceManager::GetShader("mesh_shader");
    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    AlgOps::mat4 model;
    model.identity();
    shader->setMat4("model", model);
    /*
    shader->setMat4("rotation", rotation);
    shader->setMat4("model", globalTransform);
    for (auto it = joints.begin(); it != joints.end(); it++)
    {
        AlgOps::mat4 matrix = GetRoot()->GetNode(it->first)->GetGlobalTransfrom();
        AlgOps::mat4 matrix2 = it->second;
        ReverseMatrix(matrix2);
        shader->setMat4("jointMat[" + std::to_string(it->first) + "]", matrix * matrix2);
    }
    bool useTexCoord = (texture != "");
    */
    shader->setInt("useTexCoord", false);
    /*
    if (useTexCoord)
    {
        glActiveTexture(GL_TEXTURE0);    
        glBindTexture(GL_TEXTURE_2D, RessourceManager::GetTexture(texture)->getID()); 
    }
    */
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
}