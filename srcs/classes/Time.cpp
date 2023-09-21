#include "Time.hpp"

float Time::currentFrame = glfwGetTime();
float Time::lastFrame = currentFrame;
float Time::deltaTime = 0;

Time::Time()
{
}

Time::Time(const Time &copy)
{
    (void)copy;
}

Time &Time::operator=(const Time &copy)
{
    if (&copy != this)
    {
    }
    return (*this);
}

Time::~Time()
{
}

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