#include <regex>
#include <iostream>

int main(int argc, char const *argv[])
{
    const std::string pattern[3] = {
        "v [0-9]+.[0-9]+ [0-9]+.[0-9]+( [0-9]+.[0-9]+)+",
        "vt( [0-9]+.[0-9]+)+",
        "f [0-9]/[0-9] [0-9]/[0-9]( [0-9]/[0-9])+"
    };

    const std::string str[] = {
        "v 0.5 0.5 0.5",
        "vt 1.0",
        "f 1/1 1/1 1/1",
    };
    const int patternUsed[] = {
        0,
        1,
        2,
    };
    const int nbString = sizeof(str) / sizeof(std::string);
    const int nbPattern = sizeof(patternUsed) / sizeof(int);
    if (nbString != nbPattern)
    {
        std::cerr << "nb str (" << nbString << ") != nb pattern (" << nbPattern << ")" << std::endl;
        return (1);
    }
    for (int i = 0; i < nbString; i++)
    {
        if (std::regex_match(str[i], std::regex(pattern[patternUsed[i]])))
            std::cout << "regex matched" << std::endl;
        else
            std::cout << "regex didn't matched" << std::endl;
    }
    return 0;
}
