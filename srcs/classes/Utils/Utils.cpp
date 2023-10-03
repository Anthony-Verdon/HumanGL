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
    unsigned int extensionLength;
    unsigned int pathLength;

    extensionLength = extension.length();
    pathLength = path.length();
    if (pathLength > extensionLength && path.compare(pathLength - extensionLength, extensionLength, extension) == 0)
        return (true);
    else
        return (false);
}

Utils::Exception::Exception(const std::string &errorMessage)
{
    this->errorMessage = errorMessage;
}

const char *Utils::Exception::what(void) const throw()
{
    return (errorMessage.c_str());
}