#include "parsing.hpp"

std::vector<std::unique_ptr<Object>> parseObjFile(const std::string &path)
{
    std::string word;
    std::string line;
    Vertex vertex;
    std::stringstream fileStream;
    ObjectParsingFunctionsIterator it;
    std::vector<std::unique_ptr<Object>> objects;
    std::unique_ptr<Object> object;
    unsigned int lineIndex;
    
    fileStream = Utils::readFile(path);
    lineIndex = 0;
    object = NULL;
    while (std::getline(fileStream, line))
    {
        line = line.substr(0, line.find("#"));
        word = line.substr(0, line.find(" "));
        it = Object::parsingFunctions.find(word);
        if (it != Object::parsingFunctions.end())
        {
            if (object != NULL)
                ((*object).*it->second)(line, lineIndex);
        }
        else if (word == "o")
        {
            std::vector<std::string> words = Utils::split(line, " ");
            if (words.size() != 2)
                throw(Utils::Exception("PARSING::DEFINE_OBJECT::INVALID_NUMBER_OF_ARGUMENTS"
                "\nLINE => " + line + "\n"
                "LINE INDEX => " + std::to_string(lineIndex)));
            if (object != NULL && object->getFaces().size() != 0)
                objects.push_back(std::move(object));
            object = std::make_unique<Object>(words[1]);
        }
        else if (word.length() != 0)
            throw(Utils::Exception("PARSING::UNKNOWN_DESCRIPTOR"
            "\nLINE => " + line + "\n"
            "LINE INDEX => " + std::to_string(lineIndex)));
        lineIndex++;
    }
    if (object != NULL && object->getFaces().size() != 0)
        objects.push_back(std::move(object));
    return (objects);
}
