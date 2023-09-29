#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <map>
#include "Utils.hpp"

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
    void parseFile();

    std::string name;
    unsigned int colors[3];
    float specularExponent;
    float opacity;
    unsigned int illum;

    void ignore(std::string line, unsigned int lineIndex);
    static MaterialParsingFunctions parsingFunctions;

public:
    Material(const std::string &name);
    Material &operator=(const Material &copy);
    Material(const Material &copy);
    ~Material();

    std::string getName() const;
    const unsigned int *getColors() const;
    unsigned int getColor(unsigned int index) const;
    float getSpecularExponent() const;
    float getOpacity() const;
    float getIllum() const;

    void setName(const std::string &name);
    void setColors(unsigned int colors[3]);
    void setColor(unsigned int color, unsigned int index);
    void setSpecularExponent(float specularExponent);
    void setOpacity(float opacity);
    void setIllum(float illum);

    void use();
};

#endif