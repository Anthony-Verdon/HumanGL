#include "ObjectData.hpp"
#include <stdexcept>

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
        array[j] = vertices[i][0];
        array[j + 1] = vertices[i][1];
        array[j + 2] = vertices[i][2];
        array[j + 3] = vertices[i][3];
        j += 4;
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
        return ("undefined");
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

void ObjectData::addVertex(const Vertex &vertex)
{
    vertices.push_back(vertex);
}

void ObjectData::addFace(const Face &face)
{
    faces.push_back(face);
}