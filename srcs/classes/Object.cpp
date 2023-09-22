#include "Object.hpp"

ParsingFunctions Object::parsingFunctions = {
        {"o", &Object::defineObject},
        {"s", &Object::defineSmoothShading},
        {"v", &Object::defineVertex},
        {"f", &Object::defineFace},
        {"mtllib", &Object::defineMTL},
        {"usemtl", &Object::useMTL}
};

Object::Object()
{
}

Object::Object(const std::string &objPath)
{
    std::string word;
    std::string line;
    std::ifstream objFile;
    Vertex vertex;
    std::stringstream objStream;
    ParsingFunctionsIterator it;

    name = "default";
    smoothShading = false;
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
            it = parsingFunctions.find(word);
            if (it != parsingFunctions.end())
                (this->*it->second)(line);
            else if (word.length() != 0)
                std::cerr << "error on word " << word << std::endl;
                //throw error
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

std::vector<Face> Object::getFaces() const
{
    return (faces);
}

void Object::setVertices(std::vector<std::vector<float> > vertices)
{
    this->vertices = vertices;
}

void Object::setFaces(std::vector<Face> faces)
{
    this->faces = faces;
}

std::vector<std::string> Object::splitLine(std::string line)
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
void Object::defineVertex(std::string line)
{
    Vertex vertex;
    std::vector<std::string> words;

    #ifdef DEBUG
        std::cout << "definition of a vertex: " << line << std::endl;
    #endif

    words = splitLine(line);
    if (words.size() < 4 || words.size() > 5)
        std::cerr << "number of parameter invalid" << std::endl;
    for (size_t i = 1; i < words.size(); i++)
        vertex.push_back(std::stof(words[i]));
    if (words.size() == 4)
        vertex.push_back(1.0f);

    #ifdef DEBUG
        for (size_t i = 0; i < vertex.size(); i++)
            std::cout << vertex[i] << std::endl;
    #endif
    vertices.push_back(vertex);
}

void Object::defineFace(std::string line)
{
    Face face;
    int vertexID;
    const int nbVertices = static_cast<int>(vertices.size());
    std::vector<std::string> words;

    #ifdef DEBUG
        std::cout << "definition of a face: " << line << std::endl;
    #endif

    words = splitLine(line);
    if (words.size() < 4)
        std::cerr << "number of parameter invalid" << std::endl;
    for (size_t i = 1; i < words.size(); i++)
    {
        vertexID = std::stoi(words[i]);
        if (vertexID < -nbVertices || vertexID > nbVertices)
            std::cerr << "vertex ID invalid" << std::endl;
        if (vertexID < 0)
            vertexID = nbVertices + 1 + vertexID;
        face.push_back(vertexID);
    }
    #ifdef DEBUG
        for (size_t i = 0; i < face.size(); i++)
            std::cout << face[i] << std::endl;
    #endif

    faces.push_back(face);
}

void Object::defineObject(std::string line)
{
    std::cout << "definition of an object: " << line << std::endl;
}

void Object::defineSmoothShading(std::string line)
{
    std::vector<std::string> words;

    #ifdef DEBUG
        std::cout << "definition of a smooth parameter: " << line << std::endl;
    #endif

    words = splitLine(line);
    if (words.size() != 2)
        std::cerr << "number of parameter invalid" << std::endl;

    if (words[1] == "on" || words[1] == "1")
        smoothShading = true;
    else if (words[1] == "off" || words[1] == "0")
        smoothShading = false;
    else
        std::cerr << "parameter value invalid" << std::endl;
}

void Object::defineMTL(std::string line)
{
    std::vector<std::string> words;

    #ifdef DEBUG
        std::cout << "definition of a mtllib to use: " << line << std::endl;
    #endif

    words = splitLine(line);
    if (words.size() != 2)
        std::cerr << "number of parameter invalid" << std::endl;
    
    //if file exist and is valid
        //add to material array

}

void Object::useMTL(std::string line)
{
    std::vector<std::string> words;

    #ifdef DEBUG
        std::cout << "definition of a material to use: " << line << std::endl;
    #endif

    words = splitLine(line);
    if (words.size() != 2)
        std::cerr << "number of parameter invalid" << std::endl;
    
    //if material exist
        //define it for the object
}
