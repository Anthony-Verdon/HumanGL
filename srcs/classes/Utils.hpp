#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <cmath>

class Utils
{
    public:
        static std::vector<std::string> splitLine(std::string line);
        static float DegToRad(float angle);

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

