#include "Utils.hpp"

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
        words.push_back(word);
        line = line.substr(index + 1, std::string::npos);
    }
    return words;
}

float Utils::DegToRad(float angle)
{
    return (angle * (M_PI / 180));
}

#include <iostream>
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

unsigned int Utils::convertRGBtoNum(float R, float G, float B)
{
    unsigned int rInt = static_cast<unsigned int>(R * 255);
    unsigned int gInt = static_cast<unsigned int>(G * 255);
    unsigned int bInt = static_cast<unsigned int>(B * 255);
    return ((rInt << 16) | (gInt <<  8) | bInt);
}

Matrix Utils::convertNumToRGB(unsigned int number)
{
    Matrix color(3, 1);

    color.setData(0, 0, static_cast<float>((number & 0xff0000) % 255) / 255);
    color.setData(1, 0, static_cast<float>((number & 0x00ff00) % 255) / 255);
    color.setData(2, 0, static_cast<float>((number & 0x0000ff) % 255) / 255);
    return (color);
}
Utils::Exception::Exception(const std::string &errorMessage)
{
    this->errorMessage = errorMessage;
}

const char *Utils::Exception::what(void) const throw()
{
    return (errorMessage.c_str());
}