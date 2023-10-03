#include "Texture.hpp"

Texture::Texture()
{
    ID = 0;
}

Texture::Texture(const std::string &texturePath)
{
    ID = 0;
    loadImage(texturePath);
    
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    delete []data;
}

Texture::Texture(const Texture &copy)
{
    *this = copy;
}

Texture& Texture::operator=(const Texture &copy)
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

    if (nbLine == 0 && line != "P3")
        throw(Utils::Exception("TEXTURE::INVALID_LINE\n"
        "LINE => " + line + "\n"
        "LINE INDEX => NEED TO ADD PARAM"));
    else if (nbLine == 1)
    {
        words = Utils::splitLine(line);
        if (words.size() != 2)
            throw(Utils::Exception("TEXTURE::INVALID_LINE\n"
            "LINE => " + line + "\n"
            "LINE INDEX => NEED TO ADD PARAM"));
        width = std::stoi(words[0]);
        height = std::stoi(words[1]);
        data = new unsigned char[width * height * 3];
    }
    else if (nbLine == 2)
    {
        words = Utils::splitLine(line);
        if (words.size() != 1)
            throw(Utils::Exception("TEXTURE::INVALID_LINE\n"
            "LINE => " + line + "\n"
            "LINE INDEX => NEED TO ADD PARAM"));
        valueMax = std::stoi(words[0]);
    }
}

void Texture::loadImage(const std::string &texturePath)
{
    std::ifstream textureFile;
    std::stringstream textureStream;
    std::string line;
    std::vector<std::string> words;
    unsigned int nbLine;
    unsigned int pixel;
    
    nbLine = 0;
    pixel = 0;
    textureFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    textureFile.open(texturePath);
    textureStream << textureFile.rdbuf();
    textureFile.close();
    while (std::getline(textureStream, line))
    {
        line = line.substr(0, line.find("#"));
        if (line.length() == 0)
            continue ;
        else if (nbLine < 3)
            checkHeader(line, nbLine);
        else
        {
            words = Utils::splitLine(line);
            for (size_t i = 0; i < words.size(); i++)
            {
                data[pixel] = std::stoi(words[i]);
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
}

unsigned int Texture::getID() const
{
    return (ID);
}