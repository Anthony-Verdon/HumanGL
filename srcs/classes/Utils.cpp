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

unsigned int Utils::convertRGBtoNum(unsigned char R, unsigned char G, unsigned char B)
{
    return ((R << 16) | (G <<  8) | B);
}

Utils::Exception::Exception(const std::string &errorMessage)
{
    this->errorMessage = errorMessage;
}

const char *Utils::Exception::what(void) const throw()
{
    return (errorMessage.c_str());
}