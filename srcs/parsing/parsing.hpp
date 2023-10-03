#ifndef PARSING_HPP
#define PARSING_HPP

#include "../classes/Utils/Utils.hpp"
#include "../classes/Object/Object.hpp"
#include "../classes/Material/Material.hpp"
#include <vector>
#include <string>

std::vector<Object> parseObjFile(const std::string &objPath);
void parseMtlFile(const std::string &path);
#endif