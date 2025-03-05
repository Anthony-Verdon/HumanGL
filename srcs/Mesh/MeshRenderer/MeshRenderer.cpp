#include "Mesh/MeshRenderer/MeshRenderer.hpp"
#include <glad/glad.h>
#include <iostream>
MeshRenderer::MeshRenderer(const MeshData &data): MeshData(data)
{
    VAO = 0;
    InitRenderer();
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
        DestroyRenderer();
        vertices = instance.GetVertices();
        indices = instance.GetIndices();
        InitRenderer();
    }

    return (*this);
}

MeshRenderer::~MeshRenderer()
{
    DestroyRenderer();
}

void MeshRenderer::InitRenderer()
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

void MeshRenderer::DestroyRenderer()
{
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        VAO = 0;
    }
}
void MeshRenderer::Draw() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
}