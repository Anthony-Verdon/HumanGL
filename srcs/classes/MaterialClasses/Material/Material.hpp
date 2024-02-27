#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <array>
#include <map>
#include <string>

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
    std::array<std::array<float, 3>, 3> colors;
    float specularExponent;
    float refractionIndex;
    float opacity;
    unsigned int illum;

    class Exception : public std::exception
    {
      public:
        Exception(const std::string &functionName, const std::string &errorMessage);
        const char *what(void) const throw();

      private:
        std::string errorMessage;
    };

  public:
    Material(const std::string &name);
    Material &operator=(const Material &copy);
    Material(const Material &copy);
    ~Material();

    std::string getName() const;
    std::array<std::array<float, 3>, 3> getColors() const;
    std::array<float, 3> getColor(unsigned int index) const;
    float getSpecularExponent() const;
    float getRefractionIndex() const;
    float getOpacity() const;
    float getIllum() const;

    void setName(const std::string &name);
    void setColors(const std::array<std::array<float, 3>, 3> &colors);
    void setColor(const std::array<float, 3> &color, unsigned int index);
    void setSpecularExponent(float specularExponent);
    void setRefractionIndex(float refractionIndex);
    void setOpacity(float opacity);
    void setIllum(float illum);

    static MaterialParsingFunctions parsingFunctions;
};

std::ostream &operator<<(std::ostream &os, const Material &instance);

#endif