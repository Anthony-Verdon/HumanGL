#pragma once

#include "../../MaterialClasses/Material/Material.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

typedef std::vector<float> Vertex;
typedef std::vector<int> Face;

class ObjectData
{
  public:
    ObjectData();
    ObjectData(const ObjectData &copy);
    ObjectData &operator=(const ObjectData &copy);
    ~ObjectData();

    std::string getName() const;
    std::vector<Vertex> getVertices() const;
    std::unique_ptr<float[]> getVerticesIntoArray() const;
    std::vector<Vertex> getTextureVertices() const;
    std::unique_ptr<float[]> getTexturesVerticesIntoArray() const;
    std::vector<Vertex> getCombinedVertices() const;
    std::unique_ptr<float[]> getCombinedVerticesIntoArray() const;
    std::vector<Face> getFaces() const;
    std::unique_ptr<unsigned int[]> getFacesIntoArray() const;
    unsigned int getSmoothShadingGroup() const;
    unsigned int getVAO() const;
    Material getMaterial() const;

    void reset();
    void setName(const std::string &name);
    void setVertices(const std::vector<Vertex> &vertices);
    void setTexturesVertices(const std::vector<Vertex> &vertices);
    void setCombinedVertices(const std::vector<Vertex> &vertices);
    void setFaces(const std::vector<Face> &faces);
    void setSmoothShadingGroup(unsigned int smoothShading);
    void setMaterial(const Material &material);

    void addVertex(const Vertex &vertex);
    void addTextureVertex(const Vertex &vertex);
    void addCombinedVertex(const Vertex &vertex);
    void addFace(const Face &face);

    void GenerateFacesColor();
    int CombineVertexWithColor(size_t vertexIndex, float color);

    void centerObject();

  protected:
    std::optional<std::string> name;
    std::vector<Vertex> vertices;
    std::vector<Vertex> textureVertices;
    std::vector<Vertex> combinedVertices;
    std::vector<Face> faces;
    unsigned smoothShadingGroup;
    std::optional<Material> material;
};