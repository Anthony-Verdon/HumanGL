#pragma once

#include "../ObjectData/ObjectData.hpp"
#include <iostream>

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

    void initVAO();
    bool isVAOInit() const;
    unsigned int getVAO() const;
};

std::ostream &operator<<(std::ostream &os, const Object &instance);