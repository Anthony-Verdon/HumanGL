#pragma once

#include "../../MaterialClasses/Material/Material.hpp"
#include "../Object/Object.hpp"
#include "../ObjectData/ObjectData.hpp"
#include <map>
#include <vector>

typedef enum vertexType
{
    CLASSIC,
    TEXTURE
} e_vertexType;

typedef std::map<std::string, void (*)(ObjectData &, const std::string &, unsigned int)> MapObjectParsingMethods;

class ObjectParser
{
  private:
    static size_t CalculateVertexIndex(ObjectData &objectData, const std::string &vertex, e_vertexType vertexType,
                                       const std::string &line, unsigned int lineIndex);
    static size_t CombineVertices(ObjectData &objectData, size_t vertexIndex, size_t textureVertexIndex);

    static float triangleArea(const Vertex &a, const Vertex &b, const Vertex &c);
    static bool insideTriangle(const Vertex &p, const Vertex &a, const Vertex &b, const Vertex &c);
    static void triangulate(ObjectData &objectData, Face &face);

    static void defineName(ObjectData &objectData, const std::string &line, unsigned int lineIndex);

    static void defineVertex(ObjectData &objectData, const std::string &line, unsigned int lineIndex);
    static void defineTextureVertex(ObjectData &objectData, const std::string &line, unsigned int lineIndex);
    static void defineFace(ObjectData &objectData, const std::string &line, unsigned int lineIndex);
    static void defineSmoothShading(ObjectData &objectData, const std::string &line, unsigned int lineIndex);
    static void saveNewMTL(ObjectData &objectData, const std::string &line, unsigned int lineIndex);
    static void defineMTL(ObjectData &objectData, const std::string &line, unsigned int lineIndex);
    static MapObjectParsingMethods parsingMethods;

    static std::vector<Material> materials;

    class Exception : public std::exception
    {
      public:
        Exception(const std::string &functionName, const std::string &errorMessage, const std::string &line,
                  unsigned int lineIndex);
        const char *what(void) const throw();

      private:
        std::string errorMessage;
    };

  public:
    static std::vector<Object> parseObjectFile(const std::string &path);
};