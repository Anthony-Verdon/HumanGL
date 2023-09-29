#ifndef SHADER_HPP
#define SHADER_HPP

#include "../../includes/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Matrix.hpp"

class Shader
{
    private:
        unsigned int ID;

        Shader();

        const std::string readShaderFile(const std::string &shaderPath);
        unsigned int compileShader(const char *shaderCode, const GLenum &shaderType);
        void compileProgram(unsigned int vertex, unsigned int fragment);

    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath);
        Shader(const Shader &copy);
        Shader &operator=(const Shader &copy);
        ~Shader();

        void use();

        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float x) const;
        void setVec2(const std::string &name, const Matrix &mat) const;
        void setVec2(const std::string &name, float x, float y) const;
        void setVec3(const std::string &name, const Matrix &mat) const;
        void setVec3(const std::string &name, float x, float y, float z) const;
        void setVec4(const std::string &name, const Matrix &mat) const;
        void setVec4(const std::string &name, float x, float y, float z,float w) const;
        void setMat2(const std::string &name, const Matrix &mat) const;
        void setMat3(const std::string &name, const Matrix &mat) const;
        void setMat4(const std::string &name, const Matrix &mat) const;

        unsigned int getID() const;
};

#endif
