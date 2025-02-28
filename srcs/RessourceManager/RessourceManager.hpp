#pragma once

#include "Shader/Shader.hpp"
#include "Texture/Texture.hpp"

#include <map>
#include <string>
#include <memory>

class RessourceManager
{
    private:
        RessourceManager() = delete;
        ~RessourceManager() = delete;
    
        static std::map<std::string, std::shared_ptr<Texture>> textures;
        static std::map<std::string, std::shared_ptr<Shader>> shaders;
    
    public:
        static void AddTexture(const std::string &name, const std::string &texturePath);
        static std::shared_ptr<Texture> GetTexture(const std::string &name);

        static void AddShader(const std::string &name, const std::string &vertexPath, const std::string &fragmentPath);
        static std::shared_ptr<Shader> GetShader(const std::string &name);
        static std::map<std::string, std::shared_ptr<Shader>> GetShaders();
};