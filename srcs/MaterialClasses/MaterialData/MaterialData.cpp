#include "MaterialClasses/MaterialData/MaterialData.hpp"
#include <string>

MaterialData::MaterialData()
{
    reset();
}

MaterialData::MaterialData(const MaterialData &copy)
{
    *this = copy;
}

MaterialData &MaterialData::operator=(const MaterialData &copy)
{
    if (this != &copy)
    {
        name = copy.getName();
        for (size_t i = 0; i < 3; i++)
        {
            for (size_t j = 0; j < 3; j++)
                colors[i][j] = copy.getColor(i)[j];
        }
        specularExponent = copy.getSpecularExponent();
        opacity = copy.getOpacity();
        illum = copy.getIllum();
    }
    return (*this);
}

MaterialData::~MaterialData()
{
}

std::string MaterialData::getName() const
{
    return (name);
}

std::array<std::array<float, 3>, 3> MaterialData::getColors() const
{
    return (colors);
}
std::array<float, 3> MaterialData::getColor(unsigned int colorIndex) const
{
    if (colorIndex >= 3)
        throw(Exception("GET_COLOR", "INVALID_INDEX", colorIndex));
    return (colors[colorIndex]);
}
float MaterialData::getSpecularExponent() const
{
    return (specularExponent);
}
float MaterialData::getRefractionIndex() const
{
    return (refractionIndex);
}
float MaterialData::getOpacity() const
{
    return (opacity);
}
unsigned int MaterialData::getIllum() const
{
    return (illum);
}

void MaterialData::reset()
{
    name = "";
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
            colors[i][j] = 0;
    }
    specularExponent = 0;
    refractionIndex = 0;
    opacity = 1;
    illum = 1;
}

void MaterialData::setName(std::string name)
{
    this->name = name;
}
void MaterialData::setColors(std::array<std::array<float, 3>, 3> colors)
{
    this->colors = colors;
}
void MaterialData::setColor(unsigned int colorIndex, std::array<float, 3> color)
{
    if (colorIndex >= 3)
        throw(Exception("sET_COLOR", "INVALID_INDEX", colorIndex));
    this->colors[colorIndex] = color;
}
void MaterialData::setSpecularExponent(float specularExponent)
{
    this->specularExponent = specularExponent;
}
void MaterialData::setRefractionIndex(float refractionIndex)
{
    this->refractionIndex = refractionIndex;
}
void MaterialData::setOpacity(float opacity)
{
    this->opacity = opacity;
}
void MaterialData::setIllum(unsigned int illum)
{
    this->illum = illum;
}

MaterialData::Exception::Exception(const std::string &functionName, const std::string &errorMessage,
                                   unsigned int colorIndex)
{
    this->errorMessage = "MATERIAL_DATA::" + functionName + "::" + errorMessage;
    this->errorMessage += "\n|\n| color index ; " + std::to_string(colorIndex) + "\n|";
}

const char *MaterialData::Exception::what(void) const throw()
{
    return (errorMessage.c_str());
}