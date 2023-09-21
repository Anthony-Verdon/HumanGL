#include "Object.hpp"

Object::Object()
{
}
ParsingFunctions Object::parsingFunctions = {
        {"#", &Object::ignore},
        {"o", &Object::ignore},
        {"s", &Object::defineSmoothShading},
        {"v", &Object::defineVertex},
        {"f", &Object::defineFace},
        {"mtllib", &Object::defineMTL},
        {"usemtl", &Object::useMTL}
};

Object::Object(const std::string &objPath)
{
    size_t index;
    std::string word;
    std::string line;
    std::ifstream objFile;
    Vertex vertex;
    std::stringstream objStream;
    ParsingFunctionsIterator it;

    objFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        objFile.open(objPath);
        objStream << objFile.rdbuf();
        objFile.close();
        while (std::getline(objStream, line))
        {
            index = line.find(" ");
            word = line.substr(0, index);
            line = line.substr(index + 1, line.length());
            it = parsingFunctions.find(word);
            if (it != parsingFunctions.end())
                (this->*it->second)(line);
            else
                std::cerr << "error on word " << word << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "error: " << e.what() << '\n';
    }
}


Object::Object(const Object &copy)
{
    *this = copy;
}

Object &Object::operator=(const Object &copy)
{
    if (&copy != this)
    {
        vertices = copy.getVertices();
        faces = copy.getFaces();
    }
    return (*this);
}

Object::~Object()
{
}

std::vector<std::vector<float> >Object::getVertices() const
{
    return (vertices);
}

Faces Object::getFaces() const
{
    return (faces);
}

void Object::setVertices(std::vector<std::vector<float> > vertices)
{
    this->vertices = vertices;
}

void Object::setFaces(Faces faces)
{
    this->faces = faces;
}

void Object::ignore(std::string line)
{
    (void)line;
}

void Object::defineVertex(std::string line)
{
    (void)line;
}

void Object::defineFace(std::string line)
{
    (void)line;
}

void Object::defineMTL(std::string line)
{
    (void)line;
}

void Object::defineSmoothShading(std::string line)
{
    (void)line;
}

void Object::useMTL(std::string line)
{
    (void)line;
}