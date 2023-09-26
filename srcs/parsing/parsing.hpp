#ifndef PARSING_HPP
#define PARSING_HPP

#include "../classes/Object.hpp"
#include <vector>
#include <string>

std::vector<Object> parseObjFile(const std::string &objPath);
#endif