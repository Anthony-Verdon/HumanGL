#include "Texture.hpp"

Texture::Texture()
{
}

bool Texture::textureExtension(const std::string &texturePath, const std::string &extension) const
{
    unsigned int extensionLength;
    unsigned int texturePathLength;

    extensionLength = extension.length();
    texturePathLength = texturePath.length();
    if (texturePathLength > extensionLength && texturePath.compare(texturePathLength - extensionLength, extensionLength, extension) == 0)
        return (true);
    else
        return (false);
}

Texture::Texture(const std::string &texturePath)
{
    int width;
    int height;
    int nrChannels;
    unsigned char *data;
    
    data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data == NULL)
    {
        std::cerr << "ERROR::TEXTURE::LOADING_FAILDED" << std::endl;
        ID = 0;
        return ;
    }
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    if (textureExtension(texturePath, ".jpg"))
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (textureExtension(texturePath, ".png"))
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

Texture::Texture(const Texture &copy)
{
    *this = copy;
}

Texture& Texture::operator=(const Texture &copy)
{
    this->ID = copy.getID();
    return (*this);
}

Texture::~Texture()
{
}

void Texture::initTexParameter()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
}

unsigned int Texture::getID() const
{
    return (ID);
}