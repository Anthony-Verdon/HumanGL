#include "ObjectClasses/ObjectData/ObjectData.hpp"
#include <algorithm>

ObjectData::ObjectData()
{
    reset();
}

ObjectData::~ObjectData()
{
}

std::vector<Vertex> ObjectData::getVertices() const
{
    return (vertices);
}

Vertex ObjectData::getVertex(unsigned int index) const
{
    return (vertices[index]);
}

std::unique_ptr<float[]> ObjectData::getVerticesIntoArray() const
{
    const size_t vertexSize = 4;
    size_t j;
    std::unique_ptr<float[]> array;

    j = 0;
    array = std::make_unique<float[]>(vertices.size() * vertexSize);
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t k = 0; k < vertexSize; k++)
            array[j + k] = vertices[i][k];
        j += vertexSize;
    }
    return (array);
}

std::vector<Vertex> ObjectData::getTextureVertices() const
{
    return (textureVertices);
}

Vertex ObjectData::getTextureVertex(unsigned int index) const
{
    return (textureVertices[index]);
}

std::unique_ptr<float[]> ObjectData::getTexturesVerticesIntoArray() const
{
    const size_t textureVertexSize = 3;
    size_t j;
    std::unique_ptr<float[]> array;

    j = 0;
    array = std::make_unique<float[]>(textureVertices.size() * textureVertexSize);
    for (size_t i = 0; i < textureVertices.size(); i++)
    {
        for (size_t k = 0; k < textureVertexSize; k++)
            array[j + k] = textureVertices[i][k];
        j += textureVertexSize;
    }
    return (array);
}

std::vector<Vertex> ObjectData::getCombinedVertices() const
{
    return (combinedVertices);
}

Vertex ObjectData::getCombinedVertex(unsigned int index) const
{
    return (combinedVertices[index]);
}

bool ObjectData::CombinedVertexExist(const Vertex &v) const
{
    return (std::find(combinedVertices.begin(), combinedVertices.end(), v) != combinedVertices.end());
}

std::unique_ptr<float[]> ObjectData::getCombinedVerticesIntoArray() const
{
    const size_t combinedVertexSize = 10;
    size_t j;
    std::unique_ptr<float[]> array;

    j = 0;
    array = std::make_unique<float[]>(combinedVertices.size() * combinedVertexSize);
    for (size_t i = 0; i < combinedVertices.size(); i++)
    {
        for (size_t k = 0; k < combinedVertexSize; k++)
            array[j + k] = combinedVertices[i][k];
        j += combinedVertexSize;
    }
    return (array);
}

std::unique_ptr<unsigned int[]> ObjectData::getFacesIntoArray() const
{
    size_t j;
    std::unique_ptr<unsigned int[]> array;

    j = 0;
    array = std::make_unique<unsigned int[]>(faces.size() * 3);
    for (size_t i = 0; i < faces.size(); i++)
    {
        array[j] = faces[i][0];
        array[j + 1] = faces[i][1];
        array[j + 2] = faces[i][2];
        j += 3;
    }

    return (array);
}

std::string ObjectData::getName() const
{
    if (name.has_value())
        return (name.value());
    else
        return ("");
}

Material ObjectData::getMaterial() const
{
    if (material.has_value())
        return (material.value());
    else
    {
        return (Material());
    }
}
unsigned int ObjectData::getSmoothShadingGroup() const
{
    return (smoothShadingGroup);
}

void ObjectData::reset()
{
    name.reset();
    vertices.clear();
    textureVertices.clear();
    combinedVertices.clear();
    faces.clear();
    smoothShadingGroup = 0;
    material.reset();
}

void ObjectData::setName(const std::string &name)
{
    this->name = name;
}

void ObjectData::setVertices(const std::vector<Vertex> &vertices)
{
    this->vertices = vertices;
}

void ObjectData::setTexturesVertices(const std::vector<Vertex> &vertices)
{
    this->textureVertices = vertices;
}
void ObjectData::setCombinedVertices(const std::vector<Vertex> &vertices)
{
    this->combinedVertices = vertices;
}

void ObjectData::setSmoothShadingGroup(unsigned int smoothShadingGroup)
{
    this->smoothShadingGroup = smoothShadingGroup;
}

void ObjectData::setMaterial(const Material &material)
{
    this->material = material;
}

void ObjectData::addVertex(const Vertex &vertex)
{
    vertices.push_back(vertex);
}

void ObjectData::addTextureVertex(const Vertex &vertex)
{
    textureVertices.push_back(vertex);
}

void ObjectData::addCombinedVertex(const Vertex &vertex)
{
    combinedVertices.push_back(vertex);
}

void ObjectData::addFace(const TriangleFace &face)
{
    faces.push_back(face);
}
