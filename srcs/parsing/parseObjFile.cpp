#include "parsing.hpp"

std::vector<Object> parseObjFile(const std::string &path)
{
    std::string word;
    std::string line;
    Vertex vertex;
    std::stringstream fileStream;
    ObjectParsingFunctionsIterator it;
    std::vector<Object> objects;
    Object object("default");
    unsigned int lineIndex;
    
    fileStream = Utils::readFile(path);
    lineIndex = 0;
    while (std::getline(fileStream, line))
    {
        line = line.substr(0, line.find("#"));
        word = line.substr(0, line.find(" "));
        it = Object::parsingFunctions.find(word);
        if (it != Object::parsingFunctions.end())
            (object.*it->second)(line, lineIndex);
        else if (word == "o")
        {
            std::vector<std::string> words = Utils::splitLine(line);
            if (words.size() != 2)
                throw(Utils::Exception("PARSING::DEFINE_OBJECT::INVALID_NUMBER_OF_ARGUMENTS"
                "\nLINE => " + line + "\n"
                "LINE INDEX => " + std::to_string(lineIndex)));
            if (object.getFaces().size() != 0)
                objects.push_back(object);
            Object new_object(words[1]);
            object = new_object;
        }
        else if (word.length() != 0)
            throw(Utils::Exception("PARSING::UNKNOWN_DESCRIPTOR"
            "\nLINE => " + line + "\n"
            "LINE INDEX => " + std::to_string(lineIndex)));
        lineIndex++;
    }
    objects.push_back(object);
    return (objects);
}
