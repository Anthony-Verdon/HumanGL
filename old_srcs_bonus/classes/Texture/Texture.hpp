#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <memory>

#include "../Utils/Utils.hpp"
#include "../../../includes/glad/glad.h"

class Texture
{
private:
    Texture();
    void checkHeader(const std::string &line, unsigned int nbLine);
    void loadImage(const std::string &texturePath);

    unsigned int ID;
    int width;
    int height;
    unsigned int valueMax;
    std::unique_ptr<unsigned char[]> data;

public:
    Texture(const std::string &texturePath);
    Texture(const Texture &copy);
    Texture &operator=(const Texture &copy);
    ~Texture();

    static void initTexParameter();

    unsigned int getID() const;
};

#endif