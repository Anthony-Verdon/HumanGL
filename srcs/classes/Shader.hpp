#ifndef SHADER_HPP
#define SHADER_HPP

#include "../../includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../../includes/glm/glm/glm.hpp"
#include "../../includes/glm/glm/gtc/matrix_transform.hpp"
#include "../../includes/glm/glm/gtc/type_ptr.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shader
{
    private:
        unsigned int ID;

        Shader();

        const std::string readShaderFile(const std::string &shaderPath);
        unsigned int compileShader(const char *shaderCode, const GLenum &shaderType);
        void compileProgram(const unsigned int &vertex, const unsigned int &fragment);

    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath);
        Shader(const Shader &copy);
        Shader &operator=(const Shader &copy);
        ~Shader();

        void use();

        void setInt(const std::string &name, const int &value) const;
        void setFloat(const std::string &name, const float &x) const;
        void setVec2(const std::string &name, const glm::vec2 &vec) const;
        void setVec2(const std::string &name, const float &x, const float &y) const;
        void setVec3(const std::string &name, const glm::vec3 &vec) const;
        void setVec3(const std::string &name, const float &x, const float &y, const float &z) const;
        void setVec4(const std::string &name, const glm::vec4 &vec) const;
        void setVec4(const std::string &name, const float &x, const float &y, const float &z, const float &w) const;
        void setMat2(const std::string &name, const glm::mat2 &mat) const;
        void setMat3(const std::string &name, const glm::mat3 &mat) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;

        unsigned int getID() const;
};

#endif
