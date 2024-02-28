#include "Time.hpp"

float Time::currentFrame = glfwGetTime();
float Time::lastFrame = currentFrame;
float Time::deltaTime = 0;

void Time::updateTime()
{
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

float Time::getDeltaTime()
{
    return (deltaTime);
}

float Time::getTime()
{
    return (currentFrame);
}