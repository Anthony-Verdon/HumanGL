#ifndef TIME_HPP
#define TIME_HPP

#include <GLFW/glfw3.h>

class Time
{
    private:
        static float currentFrame;
        static float lastFrame;
        static float deltaTime;

    public:
        static void updateTime();
        static float getDeltaTime();
        static float getTime();
};

#endif