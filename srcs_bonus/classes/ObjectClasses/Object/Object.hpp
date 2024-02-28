#pragma once

#include "../ObjectData/ObjectData.hpp"
#include <iostream>
#include <memory>

// possibility to create a sub class named "OpenGLObject" containing VAO
class Object : public ObjectData
{
  private:
    Object() = delete;

    bool VAOInit;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

  public:
    Object(const ObjectData &data);
    Object(const Object &copy);
    Object &operator=(const Object &copy);
    ~Object();

    bool isVAOInit() const;
    unsigned int getVAO() const;

    void initVAO();
    std::unique_ptr<float[]> convertEBOintoVBO();
};

std::ostream &operator<<(std::ostream &os, const Object &instance);