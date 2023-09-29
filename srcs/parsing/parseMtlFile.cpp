#include "../classes/Utils.hpp"
#include "../classes/Material.hpp"
#include "../classes/Object.hpp"

void parseMtlFile(std::string path)
{
    std::string word;
    std::string line;
    std::stringstream fileStream;
    MaterialParsingFunctionsIterator it;
    Material material("default");
    unsigned int lineIndex;
    
    fileStream = Utils::readFile(path);
    lineIndex = 0;
    while (std::getline(fileStream, line))
    {
        line = line.substr(0, line.find("#"));
        word = line.substr(0, line.find(" "));
        it = Material::parsingFunctions.find(word);
        if (it != Material::parsingFunctions.end())
            (material.*it->second)(line, lineIndex);
        else if (word == "newmtl")
        {
            std::vector<std::string> words = Utils::splitLine(line);
            if (words.size() != 2)
                throw(Utils::Exception("PARSING::DEFINE_OBJECT::INVALID_NUMBER_OF_ARGUMENTS"
                "\nLINE => " + line + "\n"
                "LINE INDEX => " + std::to_string(lineIndex)));
            Object::addMaterial(material);
            Material new_material(words[1]);
            material = new_material;
        }
        else if (word.length() != 0)
            throw(Utils::Exception("PARSING::UNKNOWN_DESCRIPTOR"
            "\nLINE => " + line + "\n"
            "LINE INDEX => " + std::to_string(lineIndex)));
        lineIndex++;
    }
}