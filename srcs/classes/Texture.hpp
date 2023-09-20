#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../../includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../../includes/stb_image/stb_image.h"

#include <iostream>
#include <string>
class Texture
{
private:
    Texture();

    bool textureExtension(const std::string &texturePath, const std::string &extension) const;
    unsigned int ID;

public:
    Texture(const std::string &texturePath);
    Texture(const Texture &copy);
    Texture &operator=(const Texture &copy);
    ~Texture();

    static void initTexParameter();

    unsigned int getID() const;
};

#endif