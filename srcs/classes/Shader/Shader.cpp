#include "Shader.hpp"
#include "../Utils/Utils.hpp"

Shader::Shader()
{
    ID = 0;
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{

    const std::string vertexCode = Utils::readFile(vertexPath).str();
    const std::string fragmentCode = Utils::readFile(fragmentPath).str();
    const unsigned int vertex = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    const unsigned int fragment = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
    compileProgram(vertex, fragment);
}

Shader::Shader(const Shader &copy)
{
    *this = copy;
}

Shader &Shader::operator=(const Shader &copy)
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
        throw(Exception("COMPILE_SHADER", "COMPILATION_FAILED", infoLogString));
    }
    return (shader);
}

void Shader::compileProgram(unsigned int vertex, unsigned int fragment)
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
        throw(Exception("COMPILE_PROGRAM", "LINKAGE_FAILED", infoLogString));
    }
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const Matrix &vector) const
{
    if (vector.getRows() != 2 || vector.getColumns() != 1)
        throw(Exception("SET_VEC2", "INVALID_SIZE", vector));
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, vector.getData());
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const Matrix &vector) const
{
    if (vector.getRows() != 3 || vector.getColumns() != 1)
        throw(Exception("SET_VEC3", "INVALID_SIZE", vector));
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, vector.getData());
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const Matrix &vector) const
{
    if (vector.getRows() != 4 || vector.getColumns() != 1)
        throw(Exception("SET_VEC4", "INVALID_SIZE", vector));
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, vector.getData());
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const Matrix &matrix) const
{
    if (matrix.getRows() != 2 || matrix.getColumns() != 2)
        throw(Exception("SET_MAT2", "INVALID_SIZE", matrix));
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, matrix.getData());
}

void Shader::setMat3(const std::string &name, const Matrix &matrix) const
{
    if (matrix.getRows() != 3 || matrix.getColumns() != 3)
        throw(Exception("SET_MAT3", "INVALID_SIZE", matrix));
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, matrix.getData());
}

void Shader::setMat4(const std::string &name, const Matrix &matrix) const
{
    if (matrix.getRows() != 4 || matrix.getColumns() != 4)
        throw(Exception("SET_MAT4", "INVALID_SIZE", matrix));
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, matrix.getData());
}

unsigned int Shader::getID() const
{
    return (ID);
}

Shader::Exception::Exception(const std::string &functionName, const std::string &errorMessage,
                             const std::string &infoLog)
{
    this->errorMessage = "SHADER::" + functionName + "::" + errorMessage;
    this->errorMessage += "\n|\n| ";
    this->errorMessage += "infoLog: " + infoLog;
    this->errorMessage += "\n|";
}

Shader::Exception::Exception(const std::string &functionName, const std::string &errorMessage, const Matrix &matrix)
{
    this->errorMessage = "SHADER::" + functionName + "::" + errorMessage;
    this->errorMessage += "\n|\n| ";
    if (errorMessage.find("VEC"))
        this->errorMessage += "vector: ";
    else if (errorMessage.find("MAT"))
        this->errorMessage += "matrix: ";
    this->errorMessage += std::to_string(matrix.getRows()) + " * " + std::to_string(matrix.getColumns());
    this->errorMessage += "\n|";
}
const char *Shader::Exception::what(void) const throw()
{
    return (errorMessage.c_str());
}