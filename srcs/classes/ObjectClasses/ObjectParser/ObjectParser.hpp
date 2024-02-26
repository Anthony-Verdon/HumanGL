#pragma once

#include "../../MaterialClasses/Material/Material.hpp"
#include "../Object/Object.hpp"
#include "../ObjectData/ObjectData.hpp"
#include <tuple>
#include <vector>

class ObjectParser;

typedef std::map<std::string, void (*)(const std::string &, unsigned int)> MapParsingMethods;

class ObjectParser
{
  private:
    static float triangleArea(const Vertex &a, const Vertex &b, const Vertex &c);
    static bool insideTriangle(const Vertex &p, const Vertex &a, const Vertex &b, const Vertex &c);
    static void triangulate(Face &face);

    static void defineName(const std::string &line, unsigned int lineIndex);
    static std::vector<Material> createNewMTL(const std::string &line, unsigned int lineIndex);

    static void defineVertex(const std::string &line, unsigned int lineIndex);
    static void defineFace(const std::string &line, unsigned int lineIndex);
    static void defineSmoothShading(const std::string &line, unsigned int lineIndex);
    static void defineMTL(const std::string &line, unsigned int lineIndex);
    static MapParsingMethods parsingMethods;

    static ObjectData objectValue;

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
    static std::tuple<std::vector<Object>, std::vector<Material>> parseObjectFile(const std::string &path);
};