#include "Material.hpp"

MaterialParsingFunctions Material::parsingFunctions = {
        {"newmtl", &Material::ignore},
        {"Ka", &Material::ignore},
        {"Ks", &Material::ignore},
        {"Kd", &Material::ignore},
        {"Ns", &Material::ignore},
        {"Ni", &Material::ignore},
        {"d", &Material::ignore},
        {"illum", &Material::ignore},
};

Material::Material()
{
}

void Material::parseFile()
{
    std::stringstream fileStream;
    MaterialParsingFunctionsIterator it;
    std::string line;
    std::string word;
    unsigned int lineIndex;

    fileStream = Utils::readFile(name);
    lineIndex = 0;
    while (std::getline(fileStream, line))
    {
        line = line.substr(0, line.find("#"));
        word = line.substr(0, line.find(" "));
        it = Material::parsingFunctions.find(word);
        if (it != Material::parsingFunctions.end())
            (this->*it->second)(line, lineIndex);
        else if (line.length() != 0)
            throw(Utils::Exception("MATERIAL::PARSE_FILE::UNKNOWN_DESCRIPTOR"
            "\nLINE => " + line + "\n"
            "LINE INDEX => " + std::to_string(lineIndex)));
        lineIndex++;
    }
}

Material::Material(const std::string &name)
{
    this->name = name;
    for (size_t i = 0; i < 3; i++)
        colors[i] = 0;
    specularExponent = 0;
    opacity = 1;
    illum = 1;
    parseFile();
}

Material::Material(const Material &copy)
{
    *this = copy;
}

Material &Material::operator=(const Material &copy)
{
    if (this != &copy)
    {
        name = copy.getName();
        for (size_t i = 0; i < 3; i++)
            this->colors[i] = copy.getColor(i);
        specularExponent = copy.getSpecularExponent();
        opacity = copy.getOpacity();
        illum = copy.getIllum();
    }
    return (*this);
}

Material::~Material()
{
}

std::string Material::getName() const
{
    return (name);
}

const unsigned int *Material::getColors() const
{
    return (&colors[0]);
}

unsigned int Material::getColor(unsigned int index) const
{
    if (index >= 3)
        throw(Utils::Exception("MATERIAL::GET_COLOR::INVALID_INDEX"
        "\nINDEX => " + std::to_string(index)));
    return (colors[index]);
}

float Material::getSpecularExponent() const
{
    return (specularExponent);
}

float Material::getOpacity() const
{
    return (opacity);
}

float Material::getIllum() const
{
    return (illum);
}

void Material::setName(const std::string &name)
{
    this->name = name;
}

void Material::setColors(unsigned int colors[3])
{
    for (size_t i = 0; i < 3; i++)
        this->colors[i] = colors[i];
}

void Material::setColor(unsigned int color, unsigned int index)
{
    if (index >= 3)
        throw(Utils::Exception("MATERIAL::SET_COLOR::INVALID_INDEX"
        "\nINDEX => " + std::to_string(index)));
    this->colors[index] = color;
}
void Material::setSpecularExponent(float specularExponent)
{
    this->specularExponent = specularExponent;
}

void Material::setOpacity(float opacity)
{
    this->opacity = opacity;
}

void Material::setIllum(float illum)
{
    this->illum = illum;
}

#include <iostream>
void Material::use()
{
    std::cout << "use material " << name << std::endl;
}

void Material::ignore(std::string line, unsigned int lineIndex)
{
    (void)line, (void)lineIndex;
    std::cout << "ignored" << std::endl;
}