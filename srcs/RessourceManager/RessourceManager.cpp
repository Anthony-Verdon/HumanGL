#include "RessourceManager/RessourceManager.hpp"

std::map<std::string, std::shared_ptr<Texture>> RessourceManager::textures;
std::map<std::string,  std::shared_ptr<Shader>> RessourceManager::shaders;

void RessourceManager::AddTexture(const std::string &name, const std::string &texturePath)
{
    textures[name] = std::make_shared<Texture>(texturePath);
}

void RessourceManager::AddTexture(const std::string &name, const unsigned char *buffer, size_t length)
{
    textures[name] = std::make_shared<Texture>(buffer, length);
}

bool RessourceManager::TextureExist(const std::string &name)
{
    return (textures.find(name) != textures.end());
}

std::shared_ptr<Texture> RessourceManager::GetTexture(const std::string &name)
{
    return (textures[name]);
}

void RessourceManager::AddShader(const std::string &name, const std::string &vertexPath, const std::string &fragmentPath)
{
    shaders[name] = std::make_shared<Shader>(vertexPath, fragmentPath);
}

std::shared_ptr<Shader> RessourceManager::GetShader(const std::string &name)
{
    return (shaders[name]);
}

std::map<std::string, std::shared_ptr<Shader>> RessourceManager::GetShaders()
{
    return (shaders);
}