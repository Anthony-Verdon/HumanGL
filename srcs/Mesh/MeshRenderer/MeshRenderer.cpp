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
        joints = instance.GetJoints();
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
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexStruct) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), indices.data(), GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, nbFloatPerPosition, GL_FLOAT, GL_FALSE, sizeof(VertexStruct), (void *)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, nbFloatPerTexCoord, GL_FLOAT, GL_FALSE, sizeof(VertexStruct), (void *)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, nbFloatPerNormal, GL_FLOAT, GL_FALSE, sizeof(VertexStruct), (void *)(sizeof(float) * 5));
        glEnableVertexAttribArray(2);
        
        glVertexAttribPointer(3, nbFloatPerJoint, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(VertexStruct), (void *)(sizeof(float) * 8));
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, nbFloatPerWeight, GL_FLOAT, GL_FALSE,sizeof(VertexStruct), (void *)(sizeof(float) * 8 + sizeof(uint16_t) * 4));
        glEnableVertexAttribArray(4);
        
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

void ReverseMatrix(AlgOps::mat4 &matrix)
{
    AlgOps::mat4 tmp;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            tmp.setData(i, j, matrix.getData(j, i));
        }
    }
    matrix = tmp;
}

void MeshRenderer::Draw(const AlgOps::mat4 &rotation, const AlgOps::mat4 &projection, const AlgOps::mat4 &view, const AlgOps::mat4 &model) const
{
    AlgOps::mat4 globalPosJoint[2];
    globalPosJoint[0] = {1, 7.86255e-24, 2.0051e-23, 0,
         -7.86255e-24, 1, -2.63823e-16, 0, 
        -2.0051e-23, 2.63823e-16, 1, 0, 
        -1.13742e-22, 1, 3.28889e-15, 1};
    globalPosJoint[1] = {1, 8.78695e-16, 3.42285e-08, 0, 
        -1.46449e-15, 1, 1.71143e-08, 0, 
        -3.42285e-08, -1.71143e-08, 1, 0, 
        0, 0, 0, 1};

    ReverseMatrix(globalPosJoint[0]);
    ReverseMatrix(globalPosJoint[1]);
    globalPosJoint[1] = AlgOps::translate(globalPosJoint[1], AlgOps::vec3({1, 0, 0}));
    AlgOps::mat4 globalTransform = model * localTransform;
    if (indices.size() != 0)
    {
        auto shader = RessourceManager::GetShader("mesh_shader");
        shader->use();
        shader->setMat4("rotation", rotation);
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
        shader->setMat4("model", globalTransform);
        for (auto it = joints.begin(); it != joints.end(); it++)
            shader->setMat4("jointMat[" + std::to_string(it->first) + "]", globalPosJoint[it->first] * it->second);
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