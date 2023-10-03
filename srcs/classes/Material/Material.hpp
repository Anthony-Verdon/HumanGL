#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <map>
#include "../Utils/Utils.hpp"

#define AMBIANT_COLOR 0
#define SPECULAR_COLOR 1
#define DIFFUSE_COLOR 2

class Material;

typedef std::map<std::string, void (Material::*)(std::string, unsigned int)> MaterialParsingFunctions;
typedef std::map<std::string, void (Material::*)(std::string, unsigned int)>::iterator MaterialParsingFunctionsIterator;

class Material
{
private:
    Material();
    void defineAmbiantColor(std::string line, unsigned int lineIndex);
    void defineSpecularColor(std::string line, unsigned int lineIndex);
    void defineDiffuseColor(std::string line, unsigned int lineIndex);
    void defineSpecularExponent(std::string line, unsigned int lineIndex);
    void defineRefractionIndex(std::string line, unsigned int lineIndex);
    void defineOpacity(std::string line, unsigned int lineIndex);
    void defineIllum(std::string line, unsigned int lineIndex);

    std::string name;
    float **colors;
    float specularExponent;
    float refractionIndex;
    float opacity;
    unsigned int illum;

public:
    Material(const std::string &name);
    Material &operator=(const Material &copy);
    Material(const Material &copy);
    ~Material();

    std::string getName() const;
    float **getColors() const;
    float *getColor(unsigned int index) const;
    float getSpecularExponent() const;
    float getRefractionIndex() const;
    float getOpacity() const;
    float getIllum() const;

    void setName(const std::string &name);
    void setColors(float colors[3][3]);
    void setColor(float color[3], unsigned int index);
    void setSpecularExponent(float specularExponent);
    void setRefractionIndex(float refractionIndex);
    void setOpacity(float opacity);
    void setIllum(float illum);

    static MaterialParsingFunctions parsingFunctions;
};

#endif