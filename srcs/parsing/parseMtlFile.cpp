#include "parsing.hpp"

void parseMtlFile(const std::string &path)
{
    std::string word;
    std::string line;
    std::stringstream fileStream;
    MaterialParsingFunctionsIterator it;
    std::unique_ptr<Material> material;
    unsigned int lineIndex;
    
    fileStream = Utils::readFile(path);
    lineIndex = 0;
    material = NULL;
    while (std::getline(fileStream, line))
    {
        line = line.substr(0, line.find("#"));
        word = line.substr(0, line.find(" "));
        it = Material::parsingFunctions.find(word);
        if (it != Material::parsingFunctions.end())
        {
            if (material)
                ((*material).*it->second)(line, lineIndex);
        }
        else if (word == "newmtl")
        {
            std::vector<std::string> words = Utils::splitLine(line);
            if (words.size() != 2)
                throw(Utils::Exception("PARSING::DEFINE_OBJECT::INVALID_NUMBER_OF_ARGUMENTS"
                "\nLINE => " + line + "\n"
                "LINE INDEX => " + std::to_string(lineIndex)));
            if (material != NULL)
                Object::addMaterial(std::move(material));
            material = std::make_unique<Material>(words[1]);
        }
        else if (word.length() != 0)
            throw(Utils::Exception("PARSING::UNKNOWN_DESCRIPTOR"
            "\nLINE => " + line + "\n"
            "LINE INDEX => " + std::to_string(lineIndex)));
        lineIndex++;
    }
    if (material != NULL)
        Object::addMaterial(material);
}