#include "main.hpp"
#include <iostream>

/**
 * create the shader based on a type, referenced by an ID,
 * attach the shader source code to the shader
 * and compile it.
 * check if the compilation was successful
*/
unsigned int createShader(const char *source, GLenum shaderType)
{
    unsigned int shader;

    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    return (shader);
}

/**
 * create shaders by giving a type and a source code to createShader().
 * source code is GLSL code (OpenGL Shading Language). 
 * this one are really simple.
 * shaders could also be in argument.
 * 
 * create a shaderProgram,
 * attach all shaders to this program (could be in a loop),
 * link all shaders
 * and suppress them after.
 * check if the creation was successful
*/
unsigned int createShaderProgram()
{   
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    unsigned int vertexShader;
    vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
    
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";
    unsigned int fragmentShader;
    fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);


    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    return (shaderProgram);
}