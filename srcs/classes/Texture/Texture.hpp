#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../../../includes/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../Utils/Utils.hpp"

class Texture
{
private:
    Texture();
    bool textureExtension(const std::string &texturePath, const std::string &extension) const;
    void loadImage(const std::string &texturePath);

    unsigned int ID;
    int width;
    int height;
    unsigned int valueMax;
    unsigned char *data;

public:
    Texture(const std::string &texturePath);
    Texture(const Texture &copy);
    Texture &operator=(const Texture &copy);
    ~Texture();

    static void initTexParameter();

    unsigned int getID() const;
};

#endif