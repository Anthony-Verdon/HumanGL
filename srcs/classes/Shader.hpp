#ifndef SHADER_HPP
#define SHADER_HPP

#include "../../includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shader
{
    private:
        unsigned int ID;

        Shader();

        const std::string readShaderFile(const std::string& shaderPath);
        unsigned int compileShader(const char *shaderCode, const GLenum &shaderType);
        void compileProgram(const unsigned int &vertex, const unsigned int &fragment);

    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        Shader(const Shader& copy);
        Shader &operator=(const Shader& copy);
        ~Shader();

        void use();

        void setInt(const std::string &name,  const int &value) const;
        void setFloat(const std::string &name,  const float &value) const;

        unsigned int getID() const;
};

#endif
