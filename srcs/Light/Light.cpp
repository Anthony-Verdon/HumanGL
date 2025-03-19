#include "Light/Light.hpp"
#include <glad/glad.h>

Light::Light()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    float vertices[] = {
        -0.5f, -0.5f, -2.0f, 
         0.5f, -0.5f, -2.0f,  
         0.5f,  0.5f, -2.0f,  
         0.5f,  0.5f, -2.0f,  
        -0.5f,  0.5f, -2.0f, 
        -0.5f, -0.5f, -2.0f, 

        
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    color = {1, 1, 1};
    pos = {0, 0, -3};
    scale = {0.5, 0.5, 0.5};
}

Light::~Light()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Light::Draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}