#include "Texture.hpp"
#include <glad/glad.h>
#include "Toolbox.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

bool Texture::textureInit = false;

Texture::Texture(const std::string &texturePath)
{
    if (!textureInit)
        initTexParameter();

    ID = 0;
    loadImage(texturePath);

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(const unsigned char *buffer, size_t length)
{
    if (!textureInit)
        initTexParameter();

    int width;
    int height;
    int nrChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load_from_memory(buffer, length, &width, &height, &nrChannels, 0);

    unsigned int format = 0;
    if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, &data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::Texture(const Texture &copy)
{
    *this = copy;
}

Texture &Texture::operator=(const Texture &copy)
{
    if (&copy != this)
    {
        this->ID = copy.getID();
    }
    return (*this);
}

Texture::~Texture()
{
}

void Texture::checkHeader(const std::string &line, unsigned int nbLine)
{
    std::vector<std::string> words;

    switch (nbLine)
    {
    case 0:
        if (line != "P3")
            throw(Exception("CHECK_HEADER", "INVALID_LINE", line, nbLine + 1));
        break;
    case 1: {
        words = Toolbox::splitLine(line, " ");
        if (words.size() != 2)
            throw(Exception("CHECK_HEADER", "INVALID_LINE", line, nbLine + 1));

        width = std::stoi(words[0]);
        height = std::stoi(words[1]);
        data = std::make_unique<unsigned char[]>(width * height * 3);
        break;
    }
    case 2: {
        words = Toolbox::splitLine(line, " ");
        if (words.size() != 1)
            throw(Exception("CHECK_HEADER", "INVALID_LINE", line, nbLine + 1));

        valueMax = std::stoi(words[0]);
        break;
    }
    default:
        break;
    }
}

void Texture::loadImage(const std::string &texturePath)
{
    std::string line;
    std::vector<std::string> words;
    unsigned int pixel;
    unsigned int nbLine;

    pixel = 0;
    nbLine = 0;
    std::string file = Toolbox::readFile(texturePath);
    std::vector<std::string> lines = Toolbox::splitLine(file, "\n");
    for (size_t i = 0; i < lines.size(); i++)
    {
        line = lines[i].substr(0, lines[i].find("#"));
        if (line.length() == 0)
            continue;
        else if (nbLine < 3)
            checkHeader(line, nbLine);
        else
        {
            words = Toolbox::splitLine(line, " ");
            for (size_t j = 0; j < words.size(); j++)
            {
                data[pixel] = std::stoi(words[j]);
                pixel++;
            }
        }
        nbLine++;
    }
}

void Texture::initTexParameter()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textureInit = true;
}

unsigned int Texture::getID() const
{
    return (ID);
}

Texture::Exception::Exception(const std::string &functionName, const std::string &errorMessage, const std::string &line,
                              unsigned int lineIndex)
{
    this->errorMessage = "TEXTURE::" + functionName + "::" + errorMessage;
    this->errorMessage += "\n|\n| " + std::to_string(lineIndex) + ": " + line + "\n|";
}

const char *Texture::Exception::what(void) const throw()
{
    return (errorMessage.c_str());
}