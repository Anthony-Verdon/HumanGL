#include "Shader.hpp"

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
    ID = copy.getID();
    return (*this);
}

Shader::~Shader()
{
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
    char infoLog[512];

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED" << std::endl;
        std::cerr << infoLog << std::endl;
    }
    return (shader);
}

void Shader::compileProgram(const unsigned int &vertex, const unsigned int &fragment)
{
    int success;
    char infoLog[512];

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr <<  "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl;
        std::cerr << infoLog << std::endl;
        ID = 0;
        return ;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    try
    {
        const std::string vertexCode = readShaderFile(vertexPath);
        const std::string fragmentCode = readShaderFile(fragmentPath);
        const unsigned int vertex = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
        const unsigned int fragment = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
        compileProgram(vertex, fragment);
    }
    catch(const std::exception& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        std::cerr << e.what() << std::endl;
        ID = 0;
    }
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

unsigned int Shader::getID() const
{
    return (ID);
}

