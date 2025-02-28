#include "MaterialClasses/MaterialParser/MaterialParser.hpp"
#include "Utils/Utils.hpp"
#include "MaterialClasses/Material/Material.hpp"

MapMaterialParsingMethods MaterialParser::parsingMethods = {
    {"Ka", &MaterialParser::defineAmbiantColor},    {"Ks", &MaterialParser::defineSpecularColor},
    {"Kd", &MaterialParser::defineDiffuseColor},    {"Ns", &MaterialParser::defineSpecularExponent},
    {"Ni", &MaterialParser::defineRefractionIndex}, {"d", &MaterialParser::defineOpacity},
    {"illum", &MaterialParser::defineIllum},        {"Ke", &MaterialParser::defineEmissiveCoeff},
};

std::vector<Material> MaterialParser::parseMaterialFile(const std::string &path)
{
    if (!Utils::checkExtension(path, ".mtl"))
        throw(Exception("PARSE_MATERIAL_FILE", "INVALID_EXTENSION", path, 0));

    MaterialData materialData;
    bool firstMaterial = true;
    std::vector<Material> materials;

    std::stringstream fileStream = Utils::readFile(path);
    std::string line;
    unsigned int lineIndex = 1;
    while (std::getline(fileStream, line))
    {
        line = line.substr(0, line.find("#"));
        std::string symbol = line.substr(0, line.find(" "));
        auto it = parsingMethods.find(symbol);
        if (symbol == "newmtl")
        {
            if (!firstMaterial)
            {
                Material newMaterial(materialData);
                materials.push_back(newMaterial);
                materialData.reset();
            }
            else
                firstMaterial = false;
            defineName(materialData, line, lineIndex);
        }
        else if (it != parsingMethods.end())
            (it->second)(materialData, line, lineIndex);
        else if (symbol.length() != 0)
            throw(Exception("PARSE_MATERIAL_FILE", "INVALID_SYMBOL", line, lineIndex));
        lineIndex++;
    }

    Material newMaterial(materialData);
    materials.push_back(newMaterial);
    return (materials);
}

void MaterialParser::defineName(MaterialData &materialData, const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() != 2)
        throw(Exception("DEFINE_NAME", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    materialData.setName(words[1]);
}

void MaterialParser::defineAmbiantColor(MaterialData &materialData, const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() != 4)
        throw(Exception("DEFINE_AMBIANT_COLOR", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    std::array<float, 3> ambiantColor;
    for (size_t i = 0; i < 3; i++)
    {
        if (!Utils::isFloat(words[i + 1]))
            throw(Exception("DEFINE_AMBIANT_COLOR", "INVALID_ARGUMENT", line, lineIndex));
        ambiantColor[i] = std::stof(words[i + 1]);
        if (ambiantColor[i] < 0 || ambiantColor[i] > 1)
            throw(Exception("DEFINE_AMBIANT_COLOR", "INVALID_ARGUMENT", line, lineIndex));
    }
    materialData.setColor(AMBIANT_COLOR, ambiantColor);
}

void MaterialParser::defineSpecularColor(MaterialData &materialData, const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() != 4)
        throw(Exception("DEFINE_SPECULAR_COLOR", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    std::array<float, 3> specularColor;
    for (size_t i = 0; i < 3; i++)
    {
        if (!Utils::isFloat(words[i + 1]))
            throw(Exception("DEFINE_SPECULAR_COLOR", "INVALID_ARGUMENT", line, lineIndex));
        specularColor[i] = std::stof(words[i + 1]);
        if (specularColor[i] < 0 || specularColor[i] > 1)
            throw(Exception("DEFINE_SPECULAR_COLOR", "INVALID_ARGUMENT", line, lineIndex));
    }
    materialData.setColor(SPECULAR_COLOR, specularColor);
}

void MaterialParser::defineDiffuseColor(MaterialData &materialData, const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() != 4)
        throw(Exception("DEFINE_DIFFUSE_COLOR", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    std::array<float, 3> diffuseColor;
    for (size_t i = 0; i < 3; i++)
    {
        if (!Utils::isFloat(words[i + 1]))
            throw(Exception("DEFINE_DIFFUSE_COLOR", "INVALID_ARGUMENT", line, lineIndex));
        diffuseColor[i] = std::stof(words[i + 1]);
        if (diffuseColor[i] < 0 || diffuseColor[i] > 1)
            throw(Exception("DEFINE_DIFFUSE_COLOR", "INVALID_ARGUMENT", line, lineIndex));
    }
    materialData.setColor(DIFFUSE_COLOR, diffuseColor);
}

void MaterialParser::defineSpecularExponent(MaterialData &materialData, const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() != 2)
        throw(Exception("DEFINE_SPECULAR_EXPONENT", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    if (!Utils::isFloat(words[1]))
        throw(Exception("DEFINE_SPECULAR_EXPONENT", "INVALID_ARGUMENT", line, lineIndex));
    float specularExponent = std::stof(words[1]);
    if (specularExponent < 0 || specularExponent > 1000)
        throw(Exception("DEFINE_SPECULAR_EXPONENT", "INVALID_ARGUMENT", line, lineIndex));
    materialData.setSpecularExponent(specularExponent);
}

void MaterialParser::defineRefractionIndex(MaterialData &materialData, const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() != 2)
        throw(Exception("DEFINE_REFRACTION_INDEX", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    if (!Utils::isFloat(words[1]))
        throw(Exception("DEFINE_REFRACTION_INDEX", "INVALID_ARGUMENT", line, lineIndex));
    float refractionIndex = std::stof(words[1]);
    if (refractionIndex < 0 || refractionIndex > 10)
        throw(Exception("DEFINE_REFRACTION_INDEX", "INVALID_ARGUMENT", line, lineIndex));
    materialData.setRefractionIndex(refractionIndex);
}

void MaterialParser::defineOpacity(MaterialData &materialData, const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() != 2)
        throw(Exception("DEFINE_OPACITY", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    if (!Utils::isFloat(words[1]))
        throw(Exception("DEFINE_OPACITY", "INVALID_ARGUMENT", line, lineIndex));
    float opacity = std::stof(words[1]);
    if (opacity < 0 || opacity > 1)
        throw(Exception("DEFINE_OPACITY", "INVALID_ARGUMENT", line, lineIndex));
    materialData.setOpacity(opacity);
}

void MaterialParser::defineIllum(MaterialData &materialData, const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() != 2)
        throw(Exception("DEFINE_ILLUM", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    if (!Utils::isInt(words[1]))
        throw(Exception("DEFINE_ILLUM", "INVALID_ARGUMENT", line, lineIndex));
    int illum = std::stoi(words[1]);
    if (illum < 0 || illum > 10)
        throw(Exception("DEFINE_ILLUM", "INVALID_ARGUMENT", line, lineIndex));
    materialData.setIllum(illum);
}

void MaterialParser::defineEmissiveCoeff(MaterialData &materialData, const std::string &line, unsigned int lineIndex)
{
    (void)materialData;
    (void)line;
    (void)lineIndex;
}

MaterialParser::Exception::Exception(const std::string &functionName, const std::string &errorMessage,
                                     const std::string &line, unsigned int lineIndex)
{
    this->errorMessage = "MATERIAL_PARSER::" + functionName + "::" + errorMessage;
    this->errorMessage += "\n|\n| " + std::to_string(lineIndex) + ": " + line + "\n|";
}

const char *MaterialParser::Exception::what(void) const throw()
{
    return (errorMessage.c_str());
}