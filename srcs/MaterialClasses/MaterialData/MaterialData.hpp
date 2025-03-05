#pragma once

#include <array>
#include <vector>
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
    std::array<std::vector<float>, 3> getColors() const;
    std::vector<float> getColor(unsigned int colorIndex) const;
    float getSpecularExponent() const;
    float getRefractionIndex() const;
    float getOpacity() const;
    unsigned int getIllum() const;

    void reset();
    void setName(std::string name);
    void setColors(std::array<std::vector<float>, 3> colors);
    void setColor(unsigned int colorIndex, std::vector<float> color);
    void setSpecularExponent(float specularExponent);
    void setRefractionIndex(float refractionIndex);
    void setOpacity(float opacity);
    void setIllum(unsigned int illum);

  protected:
    std::string name;
    std::array<std::vector<float>, 3> colors;
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