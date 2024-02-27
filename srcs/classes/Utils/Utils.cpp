#include "Utils.hpp"
#include <cmath>
#include <fstream>

std::vector<std::string> Utils::splitLine(std::string line)
{
    size_t index;
    std::string word;
    std::vector<std::string> words;

    index = 0;
    while (index != std::string::npos)
    {
        index = line.find(" ");
        word = line.substr(0, index);
        if (word.length() > 0)
            words.push_back(word);
        line = line.substr(index + 1, std::string::npos);
    }
    return words;
}

float Utils::DegToRad(float angle)
{
    return (angle * (M_PI / 180));
}

std::stringstream Utils::readFile(const std::string &path)
{
    std::ifstream file;
    std::stringstream stream;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path);
    stream << file.rdbuf();
    file.close();
    return (stream);
}

unsigned int Utils::convertRGBtoNum(unsigned char R, unsigned char G, unsigned char B)
{
    return ((R << 16) | (G << 8) | B);
}

Matrix Utils::convertNumToRGB(unsigned int number)
{
    Matrix color(3, 1);

    color.setData(0, 0, (number & 0xff0000) % 255);
    color.setData(1, 0, (number & 0x00ff00) % 255);
    color.setData(2, 0, (number & 0x0000ff) % 255);
    return (color);
}

bool Utils::checkExtension(const std::string &path, const std::string &extension)
{
    std::string filename = path.substr(path.find_last_of("/") + 1, path.length());
    if (extension.size() >= filename.size())
        return (false);
    for (size_t i = 0; i < extension.size(); i++)
    {
        if (filename[filename.size() - extension.size() + i] != extension[i])
            return (false);
    }
    return (true);
}