#pragma once

#include "MaterialClasses/Material/Material.hpp"
#include "ObjectClasses/Object/Object.hpp"
#include "ObjectClasses/ObjectData/ObjectData.hpp"
#include <map>
#include <vector>

typedef enum vertexType
{
    CLASSIC,
    TEXTURE
} e_vertexType;

class ObjectParser;

typedef std::map<std::string, void (ObjectParser::*)(ObjectData &, const std::string &)> MapObjectParsingMethods;

class ObjectParser
{
  private:
    std::string path;
    unsigned int lineIndex;
    std::vector<Material> materials;
  
    size_t CalculateVertexIndex(ObjectData &objectData, const std::string &vertex, e_vertexType vertexType, const std::string &line);
    size_t CombineVertices(ObjectData &objectData, size_t vertexIndex, size_t textureVertexIndex);

    float triangleArea(const Vertex &a, const Vertex &b, const Vertex &c);
    bool insideTriangle(const Vertex &p, const Vertex &a, const Vertex &b, const Vertex &c);
    void triangulate(ObjectData &objectData, Face &face);

    void defineName(ObjectData &objectData, const std::string &line);
    void defineVertex(ObjectData &objectData, const std::string &line);
    void defineTextureVertex(ObjectData &objectData, const std::string &line);
    void defineNormalVertex(ObjectData &objectData, const std::string &line);
    void defineFace(ObjectData &objectData, const std::string &line);
    void defineSmoothShading(ObjectData &objectData, const std::string &line);
    void saveNewMTL(ObjectData &objectData, const std::string &line);
    void defineMTL(ObjectData &objectData, const std::string &line);
    MapObjectParsingMethods parsingMethods;

    // parsing time
    float positionVertexTime;
    float textureVertexTime;
    float normalVertexTime;
    float faceTime;


    std::string parseError(const std::string &functionName, const std::string &errorMessage, const std::string &line);

  public:
    ObjectParser();
    ~ObjectParser();

    std::vector<Object> parseObjectFile(const std::string &path);
};