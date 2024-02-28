#pragma once

#include <sstream>
#include <string>
#include <vector>

class Utils
{
  public:
    static std::vector<std::string> splitLine(std::string line);
    static float DegToRad(float angle);
    static std::stringstream readFile(const std::string &path);
    static bool checkExtension(const std::string &path, const std::string &extension);
};
