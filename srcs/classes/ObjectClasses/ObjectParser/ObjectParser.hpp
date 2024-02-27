#pragma once

#include "../../MaterialClasses/Material/Material.hpp"
#include "../Object/Object.hpp"
#include "../ObjectData/ObjectData.hpp"
#include <map>
#include <vector>

typedef std::map<std::string, void (*)(const std::string &, unsigned int)> MapObjectParsingMethods;

class ObjectParser
{
  private:
    static float triangleArea(const Vertex &a, const Vertex &b, const Vertex &c);
    static bool insideTriangle(const Vertex &p, const Vertex &a, const Vertex &b, const Vertex &c);
    static void triangulate(Face &face);

    static void defineName(const std::string &line, unsigned int lineIndex);

    static void defineVertex(const std::string &line, unsigned int lineIndex);
    static void defineFace(const std::string &line, unsigned int lineIndex);
    static void defineSmoothShading(const std::string &line, unsigned int lineIndex);
    static void saveNewMTL(const std::string &line, unsigned int lineIndex);
    static void defineMTL(const std::string &line, unsigned int lineIndex);
    static MapObjectParsingMethods parsingMethods;

    static ObjectData objectData;
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