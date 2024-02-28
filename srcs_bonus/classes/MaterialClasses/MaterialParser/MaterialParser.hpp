#pragma once

#include "../Material/Material.hpp"
#include "../MaterialData/MaterialData.hpp"
#include <map>
#include <string>
#include <vector>

typedef std::map<std::string, void (*)(MaterialData &, const std::string &, unsigned int)> MapMaterialParsingMethods;

class MaterialParser
{
  private:
    static void defineName(MaterialData &materialData, const std::string &line, unsigned int lineIndex);

    static void defineAmbiantColor(MaterialData &materialData, const std::string &line, unsigned int lineIndex);
    static void defineSpecularColor(MaterialData &materialData, const std::string &line, unsigned int lineIndex);
    static void defineDiffuseColor(MaterialData &materialData, const std::string &line, unsigned int lineIndex);
    static void defineSpecularExponent(MaterialData &materialData, const std::string &line, unsigned int lineIndex);
    static void defineRefractionIndex(MaterialData &materialData, const std::string &line, unsigned int lineIndex);
    static void defineOpacity(MaterialData &materialData, const std::string &line, unsigned int lineIndex);
    static void defineIllum(MaterialData &materialData, const std::string &line, unsigned int lineIndex);
    static MapMaterialParsingMethods parsingMethods;

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
    static std::vector<Material> parseMaterialFile(const std::string &path);
};