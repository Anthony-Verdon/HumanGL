#include "Shader.hpp"
#include "Utils.hpp"

Shader::Shader()
{
    ID = 0;
}

Shader::Shader(const Shader& copy)
{
    *this = copy;
}

Shader& Shader::operator=(const Shader& copy)
{
     if (&copy != this)
    {
        ID = copy.getID();
    }
    return (*this);
}

Shader::~Shader()
{
    if (ID != 0)
        glDeleteProgram(ID);
}

const std::string Shader::readShaderFile(const std::string& shaderPath)
{
    std::ifstream shaderFile;
    std::stringstream shaderStream;
    std::string shaderCode;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    shaderFile.open(shaderPath);
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    shaderCode = shaderStream.str();

    return (shaderCode);
}

unsigned int Shader::compileShader(const char *shaderCode, const GLenum &shaderType)
{
    int success;
    char infoLogChar[512];
    std::string infoLogString;

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLogChar);
        infoLogString = infoLogChar;
        throw(Utils::Exception("SHADER::COMPILATION FAILED\n"
        "LOG => " + infoLogString));
    }
    return (shader);
}

void Shader::compileProgram(const unsigned int &vertex, const unsigned int &fragment)
{
    int success;
    char infoLogChar[512];
    std::string infoLogString;

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLogChar);
        infoLogString = infoLogChar;
        throw(Utils::Exception("SHADER::PROGRAM::LINKING FAILED\n"
        "LOG => " + infoLogString));
    }
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{

     const std::string vertexCode = readShaderFile(vertexPath);
    const std::string fragmentCode = readShaderFile(fragmentPath);
    const unsigned int vertex = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    const unsigned int fragment = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
    compileProgram(vertex, fragment);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setInt(const std::string &name, const int &value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name,  const float &value) const
{
   glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &vec) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

void Shader::setVec2(const std::string &name, const float &x, const float &y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

void Shader::setVec3(const std::string &name, const float &x, const float &y, const float &z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &vec) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

void Shader::setVec4(const std::string &name, const float &x, const float &y, const float &z, const float &w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

unsigned int Shader::getID() const
{
    return (ID);
}

