#include "Mesh/MeshRenderer/MeshRenderer.hpp"
#include "RessourceManager/RessourceManager.hpp"
#include <glad/glad.h>
#include <iostream>
MeshRenderer::MeshRenderer(const MeshData &data): MeshData(data)
{
    VAO = 0;
}

MeshRenderer::MeshRenderer(const MeshRenderer &instance)
{
    VAO = 0;
    *this = instance;
}

MeshRenderer &MeshRenderer::operator=(const MeshRenderer &instance)
{
    if (this != &instance)
    {
        name = instance.GetName();
        vertices = instance.GetVertices();
        indices = instance.GetIndices();
        localTransform = instance.GetLocalTransfrom();
        children = instance.GetChildren();
        texture = instance.GetTexture();
    }

    return (*this);
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::InitRenderer()
{
    if (indices.size() != 0)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), indices.data(), GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    for (size_t i = 0; i < children.size(); i++)
        childrenRenderer.push_back(children[i]);
    for (size_t i = 0; i < childrenRenderer.size(); i++)
        childrenRenderer[i].InitRenderer();
}

void MeshRenderer::DestroyRenderer()
{
    if (VAO != 0)
    {
        for (size_t i = 0; i < childrenRenderer.size(); i++)
            childrenRenderer[i].DestroyRenderer();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        VAO = 0;
    }
}
void MeshRenderer::Draw(const AlgOps::mat4 &rotation, const AlgOps::mat4 &projection, const AlgOps::mat4 &view, const AlgOps::mat4 &model) const
{
    AlgOps::mat4 globalTransform = model * localTransform;
    if (indices.size() != 0)
    {
        auto shader = RessourceManager::GetShader("mesh_shader");
        shader->use();
        shader->setMat4("rotation", rotation);
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
        shader->setMat4("model", globalTransform);
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
    for (size_t i = 0; i < childrenRenderer.size(); i++)
        childrenRenderer[i].Draw(rotation, projection, view, globalTransform);
}