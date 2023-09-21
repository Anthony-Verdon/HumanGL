#ifndef TIME_HPP
#define TIME_HPP

#include "../../includes/glad/glad.h"
#include <GLFW/glfw3.h>

class Time
{
    private:
        Time();
        Time(const Time &copy);
        Time &operator=(const Time &copy);
        ~Time();

        static float currentFrame;
        static float lastFrame;
        static float deltaTime;

    public:
        static void updateTime();
        static float getDeltaTime();
        static float getTime();
};

#endif