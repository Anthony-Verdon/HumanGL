#include "ObjectData.hpp"
#include <algorithm>
#include <ctime>
ObjectData::ObjectData()
{
    reset();
}

ObjectData::~ObjectData()
{
}

std::vector<std::vector<float>> ObjectData::getVertices() const
{
    return (vertices);
}

std::unique_ptr<float[]> ObjectData::getVerticesIntoArray() const
{
    size_t j;
    std::unique_ptr<float[]> array;

    j = 0;
    array = std::make_unique<float[]>(vertices.size() * 4);
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (int k = 0; k < 4; k++)
            array[j + k] = vertices[i][k];
        j += 4;
    }
    return (array);
}

std::vector<std::vector<float>> ObjectData::getCombinedVertices() const
{
    return (combinedVertices);
}

std::unique_ptr<float[]> ObjectData::getCombinedVerticesIntoArray() const
{
    size_t j;
    std::unique_ptr<float[]> array;

    j = 0;
    array = std::make_unique<float[]>(combinedVertices.size() * 7);
    for (size_t i = 0; i < combinedVertices.size(); i++)
    {
        for (int k = 0; k < 7; k++)
            array[j + k] = combinedVertices[i][k];
        j += 7;
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

std::vector<Face> ObjectData::getFaces() const
{
    return (faces);
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
bool ObjectData::getSmoothShading() const
{
    return (smoothShading);
}

void ObjectData::reset()
{
    name.reset();
    vertices.clear();
    faces.clear();
    smoothShading = false;
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

void ObjectData::setFaces(const std::vector<Face> &faces)
{
    this->faces = faces;
}

void ObjectData::setSmoothShading(bool smoothShading)
{
    this->smoothShading = smoothShading;
}

void ObjectData::setMaterial(const Material &material)
{
    this->material = material;
}
void ObjectData::addVertex(const Vertex &vertex)
{
    vertices.push_back(vertex);
}

void ObjectData::addFace(const Face &face)
{
    faces.push_back(face);
}

void ObjectData::generateFacesColor()
{
    srand(time(NULL));

    std::vector<Face> newFaces;
    for (size_t i = 0; i < faces.size(); i++)
    {
        float randomColor = static_cast<float>(rand() % 10000) / 10000;
        Face newFace;

        for (int j = 0; j < 3; j++)
            newFace.push_back(CombineVertexWithColor(faces[i][j], randomColor));
        newFaces.push_back(newFace);
    }
    faces = newFaces;
}

int ObjectData::CombineVertexWithColor(size_t vertexIndex, float color)
{
    Vertex combinedVertex;

    for (size_t j = 0; j < 4; j++)
        combinedVertex.push_back((vertices[vertexIndex][j]));
    for (size_t j = 0; j < 3; j++)
        combinedVertex.push_back(color);

    auto it = std::find(combinedVertices.begin(), combinedVertices.end(), combinedVertex);
    if (it == combinedVertices.end())
    {
        combinedVertices.push_back(combinedVertex);
        return (combinedVertices.size() - 1);
    }
    return (std::distance(combinedVertices.begin(), it));
}