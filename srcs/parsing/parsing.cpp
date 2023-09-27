#include "parsing.hpp"

std::vector<std::string> splitLine(std::string line)
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

std::vector<Object> parseObjFile(const std::string &objPath)
{
    std::string word;
    std::string line;
    std::ifstream objFile;
    Vertex vertex;
    std::stringstream objStream;
    ParsingFunctionsIterator it;
    std::vector<Object> objects;
    Object object("default");
    
    objFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        objFile.open(objPath);
        objStream << objFile.rdbuf();
        objFile.close();
        while (std::getline(objStream, line))
        {
            line = line.substr(0, line.find("#"));
            word = line.substr(0, line.find(" "));
            it = Object::parsingFunctions.find(word);
            if (it != Object::parsingFunctions.end())
                (object.*it->second)(line);
            else if (word == "o")
            {
                std::vector<std::string> words = splitLine(line);
                if (words.size() != 2)
                    std::cerr << "nb argument invalid" << std::endl;
                if (object.getFaces().size() != 0)
                    objects.push_back(object);
                Object new_object(words[1]);
                object = new_object;
            }
            else if (word.length() != 0)
                std::cerr << "error on word " << word << std::endl;
                //throw error
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "error: " << e.what() << '\n';
    }
    objects.push_back(object);
    return (objects);
}