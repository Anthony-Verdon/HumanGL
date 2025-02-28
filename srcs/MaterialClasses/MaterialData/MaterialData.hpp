#pragma once

#include <array>
#include <string>

typedef enum colors
{
    AMBIANT_COLOR,
    SPECULAR_COLOR,
    DIFFUSE_COLOR
} e_colors;

class MaterialData
{
  public:
    MaterialData();
    MaterialData(const MaterialData &copy);
    MaterialData &operator=(const MaterialData &copy);
    ~MaterialData();

    std::string getName() const;
    std::array<std::array<float, 3>, 3> getColors() const;
    std::array<float, 3> getColor(unsigned int colorIndex) const;
    float getSpecularExponent() const;
    float getRefractionIndex() const;
    float getOpacity() const;
    unsigned int getIllum() const;

    void reset();
    void setName(std::string name);
    void setColors(std::array<std::array<float, 3>, 3> colors);
    void setColor(unsigned int colorIndex, std::array<float, 3> color);
    void setSpecularExponent(float specularExponent);
    void setRefractionIndex(float refractionIndex);
    void setOpacity(float opacity);
    void setIllum(unsigned int illum);

  protected:
    std::string name;
    std::array<std::array<float, 3>, 3> colors;
    float specularExponent;
    float refractionIndex;
    float opacity;
    unsigned int illum;

  private:
    class Exception : public std::exception
    {
      public:
        Exception(const std::string &functionName, const std::string &errorMessage, unsigned int index);
        const char *what(void) const throw();

      private:
        std::string errorMessage;
    };
};