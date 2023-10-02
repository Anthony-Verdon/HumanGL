#include "Material.hpp"

MaterialParsingFunctions Material::parsingFunctions = {
        {"Ka", &Material::defineAmbiantColor},
        {"Ks", &Material::defineSpecularColor},
        {"Kd", &Material::defineDiffuseColor},
        {"Ns", &Material::defineSpecularExponent},
        {"Ni", &Material::defineRefractionIndex},
        {"d", &Material::defineOpacity},
        {"illum", &Material::defineIllum},
};

Material::Material()
{
}

Material::Material(const std::string &name)
{
    this->name = name;
    for (size_t i = 0; i < 3; i++)
        colors[i] = 0;
    specularExponent = 0;
    refractionIndex = 0;
    opacity = 1;
    illum = 1;
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

void Material::defineAmbiantColor(std::string line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line);
    if (words.size() != 4)
        throw(Utils::Exception("MATERIAL::DEFINE_AMBIANT_COLOR::INVALID_NUMBER_OF_ARGUMENTS"
        "\nLINE => " + line + "\n"
        "LINE INDEX => " + std::to_string(lineIndex)));

    colors[AMBIANT_COLOR] = Utils::convertRGBtoNum(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
}

void Material::defineSpecularColor(std::string line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line);
    if (words.size() != 4)
        throw(Utils::Exception("MATERIAL::DEFINE_SPECULAR_COLOR::INVALID_NUMBER_OF_ARGUMENTS"
        "\nLINE => " + line + "\n"
        "LINE INDEX => " + std::to_string(lineIndex)));

    colors[SPECULAR_COLOR] = Utils::convertRGBtoNum(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
}

void Material::defineDiffuseColor(std::string line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line);
    if (words.size() != 4)
        throw(Utils::Exception("MATERIAL::DEFINE_DIFFUSE_COLOR::INVALID_NUMBER_OF_ARGUMENTS"
        "\nLINE => " + line + "\n"
        "LINE INDEX => " + std::to_string(lineIndex)));

    colors[DIFFUSE_COLOR] = Utils::convertRGBtoNum(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
}

void Material::defineSpecularExponent(std::string line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line);
    if (words.size() != 2)
        throw(Utils::Exception("MATERIAL::DEFINE_SPECULAR_EXPONENT::INVALID_NUMBER_OF_ARGUMENTS"
        "\nLINE => " + line + "\n"
        "LINE INDEX => " + std::to_string(lineIndex)));

   specularExponent = std::stof(words[1]);
   if (specularExponent < 0 || specularExponent > 1000)
        throw(Utils::Exception("MATERIAL::DEFINE_SPECULAR_EXPONENT::INVALID_ARGUMENT"
        "\nLINE => " + line + "\n"
        "LINE INDEX => " + std::to_string(lineIndex)));
}

void Material::defineRefractionIndex(std::string line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line);
    if (words.size() != 2)
        throw(Utils::Exception("MATERIAL::DEFINE_REFRACTION_INDEX::INVALID_NUMBER_OF_ARGUMENTS"
        "\nLINE => " + line + "\n"
        "LINE INDEX => " + std::to_string(lineIndex)));

   refractionIndex = std::stof(words[1]);
   if (refractionIndex < 0 || refractionIndex > 10)
        throw(Utils::Exception("MATERIAL::DEFINE_REFRACTION_INDEX::INVALID_ARGUMENT"
        "\nLINE => " + line + "\n"
        "LINE INDEX => " + std::to_string(lineIndex)));
}

void Material::defineOpacity(std::string line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line);
    if (words.size() != 2)
        throw(Utils::Exception("MATERIAL::DEFINE_OPACITY::INVALID_NUMBER_OF_ARGUMENTS"
        "\nLINE => " + line + "\n"
        "LINE INDEX => " + std::to_string(lineIndex)));

   refractionIndex = std::stof(words[1]);
   if (refractionIndex < 0 || refractionIndex > 1)
        throw(Utils::Exception("MATERIAL::DEFINE_OPACITY::INVALID_ARGUMENT"
        "\nLINE => " + line + "\n"
        "LINE INDEX => " + std::to_string(lineIndex)));
}

void Material::defineIllum(std::string line, unsigned int lineIndex)
{
    (void)line, (void)lineIndex;
}