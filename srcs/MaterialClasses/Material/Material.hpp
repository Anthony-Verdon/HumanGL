#pragma once

#include "MaterialClasses/MaterialData/MaterialData.hpp"

class Material : public MaterialData
{
  public:
    Material();
    Material(const MaterialData &materialData);
    Material &operator=(const Material &copy);
    Material(const Material &copy);
    ~Material();

    bool operator==(const Material &instance);
};

std::ostream &operator<<(std::ostream &os, const Material &instance);