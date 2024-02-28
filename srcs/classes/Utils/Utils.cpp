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
    return (roundf(angle * (M_PI / 180) * 100000) / 100000);
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