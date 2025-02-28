#include "MaterialClasses/Material/Material.hpp"
#include <iostream>

Material::Material()
{
    reset();
}

Material::Material(const MaterialData &materialData)
{
    name = materialData.getName();
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
            colors[i][j] = materialData.getColor(i)[j];
    }
    specularExponent = materialData.getSpecularExponent();
    opacity = materialData.getOpacity();
    illum = materialData.getIllum();
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

Material::~Material()
{
}

std::ostream &operator<<(std::ostream &os, const Material &instance)
{
    os << "name: " << instance.getName() << std::endl;
    os << "ambiant color: ";
    std::array<float, 3> color = instance.getColor(AMBIANT_COLOR);
    for (size_t i = 0; i < 3; i++)
        os << color[i] << " ";
    os << std::endl;
    return (os);
}