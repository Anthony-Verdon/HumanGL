#include "ObjectClasses/Object/Object.hpp"
#include <glad/glad.h>

Object::Object(const ObjectData &data)
{
    name = data.getName();
    vertices = data.getVertices();
    textureVertices = data.getTextureVertices();
    normalVertices = data.getNormalVertices();
    combinedVertices = data.getCombinedVertices();
    faces = data.getFaces();
    smoothShadingGroup = data.getSmoothShadingGroup();
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
        textureVertices = copy.getTextureVertices();
        normalVertices = copy.getNormalVertices();
        combinedVertices = copy.getCombinedVertices();
        faces = copy.getFaces();
        name = copy.getName();
        smoothShadingGroup = copy.getSmoothShadingGroup();
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

bool Object::isVAOInit() const
{
    return (VAOInit);
}
unsigned int Object::getVAO() const
{
    return (VAO);
}

void Object::initVAO()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    std::unique_ptr<float[]> verticesArray = getCombinedVerticesIntoArray();
    std::unique_ptr<unsigned int[]> facesArray = getFacesIntoArray();

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * combinedVertices.size() * 13, &verticesArray[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faces.size() * 3, &facesArray[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 13 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(float), (void *)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(float), (void *)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(float), (void *)(10 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    VAOInit = true;
}

std::ostream &operator<<(std::ostream &os, const Object &instance)
{
    os << "name: " << instance.getName() << std::endl;
    os << "smooth shading: " << instance.getSmoothShadingGroup() << std::endl;
    std::vector<Vertex> objectVertices = instance.getCombinedVertices();
    os << "vertices: " << std::endl;
    for (size_t i = 0; i < objectVertices.size(); i++)
    {
        os << "vertex nb " << i + 1 << ": ";
        for (int j = 0; j < 7; j++)
            os << objectVertices[i][j] << " ";
        os << std::endl;
    }
    std::vector<TriangleFace> objectFace = instance.getFaces();
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