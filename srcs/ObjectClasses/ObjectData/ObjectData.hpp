#pragma once

#include "MaterialClasses/Material/Material.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

typedef std::vector<float> Vertex;
typedef std::array<int, 3> TriangleFace;
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
    Vertex getVertex(unsigned int index) const;
    std::unique_ptr<float[]> getVerticesIntoArray() const;

    std::vector<Vertex> getTextureVertices() const;
    Vertex getTextureVertex(unsigned int index) const;
    std::unique_ptr<float[]> getTexturesVerticesIntoArray() const;

    std::vector<Vertex> getNormalVertices() const;
    Vertex getNormalVertex(unsigned int index) const;
    std::unique_ptr<float[]> getNormalVerticesIntoArray() const;

    std::vector<Vertex> getCombinedVertices() const;
    Vertex getCombinedVertex(unsigned int index) const;
    std::unique_ptr<float[]> getCombinedVerticesIntoArray() const;
    size_t getCombinedVerticesSize() const { return combinedVertices.size(); };
    bool CombinedVertexExist(const Vertex &v) const;

    std::vector<TriangleFace> getFaces() const { return faces; }
    std::unique_ptr<unsigned int[]> getFacesIntoArray() const;
    unsigned int getSmoothShadingGroup() const;
    unsigned int getVAO() const;
    Material getMaterial() const;

    void reset();
    void setName(const std::string &name);
    void setVertices(const std::vector<Vertex> &vertices);
    void setTexturesVertices(const std::vector<Vertex> &vertices);
    void setNormalVertices(const std::vector<Vertex> &vertices);
    void setCombinedVertices(const std::vector<Vertex> &vertices);
    void setFaces(const std::vector<TriangleFace> &faces) { this->faces = faces; }
    void setSmoothShadingGroup(unsigned int smoothShading);
    void setMaterial(const Material &material);

    void addVertex(const Vertex &vertex);
    void addTextureVertex(const Vertex &vertex);
    void addNormalVertex(const Vertex &vertex);
    void addCombinedVertex(const Vertex &vertex);
    void addFace(const TriangleFace &face);

  protected:
    std::optional<std::string> name;
    std::vector<Vertex> vertices;
    std::vector<Vertex> textureVertices;
    std::vector<Vertex> normalVertices;
    std::vector<Vertex> combinedVertices;
    std::vector<TriangleFace> faces;
    unsigned smoothShadingGroup;
    std::optional<Material> material;
};