#ifndef UTILS_HPP
#define UTILS_HPP

#include "../Matrix/Matrix.hpp"
#include <sstream>
#include <string>
#include <vector>

class Utils
{
  public:
    static std::vector<std::string> splitLine(std::string line);
    static float DegToRad(float angle);
    static std::stringstream readFile(const std::string &path);
    static unsigned int convertRGBtoNum(unsigned char R, unsigned char G, unsigned char B);
    static Matrix convertNumToRGB(unsigned int number);
    static bool checkExtension(const std::string &path, const std::string &extension);
};

#endif
