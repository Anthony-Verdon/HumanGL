#include "RessourceManager/RessourceManager.hpp"

std::map<std::string, std::shared_ptr<Texture>> RessourceManager::textures;
std::map<std::string,  std::shared_ptr<Shader>> RessourceManager::shaders;

void RessourceManager::AddTexture(const std::string &name, const std::string &texturePath)
{
    textures[name] = std::make_shared<Texture>(texturePath);
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