#pragma once

#include <string>

#include <glad/glad.h>
#include "Matrix/Matrix.hpp"

class Shader
{
  private:
    unsigned int ID;

    Shader();

    const std::string readShaderFile(const std::string &shaderPath);
    unsigned int compileShader(const char *shaderCode, const GLenum &shaderType);
    void compileProgram(unsigned int vertex, unsigned int fragment);

    class Exception : public std::exception
    {
      public:
        Exception(const std::string &functionName, const std::string &errorMessage, const std::string &infoLog);
        template<size_t R, size_t C>
        Exception(const std::string &functionName, const std::string &errorMessage, const AlgOps::Matrix<R, C> &vector);
        const char *what(void) const throw();

      private:
        std::string errorMessage;
    };

  public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    Shader(const Shader &copy);
    Shader &operator=(const Shader &copy);
    ~Shader();

    void use() const;

    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float x) const;
    void setVec2(const std::string &name, const AlgOps::vec2 &vector) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const AlgOps::vec3 &vector) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, const AlgOps::vec4 &vector) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setMat2(const std::string &name, const AlgOps::mat2 &matrix) const;
    void setMat3(const std::string &name, const AlgOps::mat3 &matrix) const;
    void setMat4(const std::string &name, const AlgOps::mat4 &matrix) const;

    unsigned int getID() const;
};
