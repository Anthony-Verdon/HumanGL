#include "Object.hpp"
#include "../../../../libs/glad/glad.h"
#include <cstdlib>
#include <ctime>

Object::Object(const ObjectData &data)
{
    name = data.getName();
    vertices = data.getVertices();
    combinedVertices = data.getCombinedVertices();
    faces = data.getFaces();
    smoothShading = data.getSmoothShading();
    material = data.getMaterial();
    VAOInit = false;
}

Object::Object(const Object &copy)
{
    *this = copy;
}

Object &Object::operator=(const Object &copy)
{
    if (&copy != this)
    {
        vertices = copy.getVertices();
        combinedVertices = copy.getCombinedVertices();
        faces = copy.getFaces();
        name = copy.getName();
        smoothShading = copy.getSmoothShading();
        material = copy.getMaterial();
        VAOInit = false;
    }
    return (*this);
}

Object::~Object()
{
    if (VAOInit)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

/*
    VAO = Vertex Array Object, store the VBO and how the VBO is configurated
    VBO = Vertex Buffer Object, contains all the vertices of an object
    EBO = Element Buffer Object, allow to store once each vertex and define element with their index

    glGen* : generate what is asked
    glBind* : bind the element to tell which one is used for each Buffer, 0 = reinitialized
    glBufferData : store informations into the corresponding buffer
    glVertexAttribPointer : how the VBO is configured
    if a VBO is bind after a VAO, it's stored in the VAO

*/
void Object::initVAO()
{
    generateFacesColor();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    std::unique_ptr<float[]> verticesArray = getCombinedVerticesIntoArray();
    std::unique_ptr<unsigned int[]> facesArray = getFacesIntoArray();

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * combinedVertices.size() * 7, &verticesArray[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faces.size() * 3, &facesArray[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    VAOInit = true;
}

bool Object::isVAOInit() const
{
    return (VAOInit);
}
unsigned int Object::getVAO() const
{
    return (VAO);
}

std::ostream &operator<<(std::ostream &os, const Object &instance)
{
    os << "name: " << instance.getName() << std::endl;
    os << "smooth shading: " << instance.getSmoothShading() << std::endl;
    std::vector<Vertex> objectVertices = instance.getVertices();
    os << "vertices: " << std::endl;
    for (size_t i = 0; i < objectVertices.size(); i++)
    {
        os << "vertex nb " << i + 1 << ": ";
        os << objectVertices[i][0] << " " << objectVertices[i][1] << " ";
        os << objectVertices[i][2] << " " << objectVertices[i][3] << std::endl;
    }
    std::vector<Face> objectFace = instance.getFaces();
    os << "faces: " << std::endl;
    for (size_t i = 0; i < objectFace.size(); i++)
    {
        os << "face nb " << i + 1 << ": ";
        for (size_t j = 0; j < objectFace[i].size(); j++)
            os << objectFace[i][j] << " ";
        os << std::endl;
    }
    return (os);
}