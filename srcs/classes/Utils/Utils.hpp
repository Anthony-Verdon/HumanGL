#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include "../Matrix/Matrix.hpp"
class Utils
{
    public:
        static std::vector<std::string> splitLine(std::string line);
        static float DegToRad(float angle);
        static std::stringstream readFile(const std::string &path);
        static unsigned int convertRGBtoNum(unsigned char R, unsigned char G, unsigned char B);
        static Matrix convertNumToRGB(unsigned int number);
        static bool checkExtension(const std::string &path, const std::string &extension);

        class Exception : public std::exception
        {
            public:
                Exception(const std::string &errorMessage);
                const char *what(void) const throw();

            private:
                std::string errorMessage;
        };
    
};

#endif

