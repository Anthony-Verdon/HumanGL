#include "ObjectParser.hpp"
#include "../../Utils/Utils.hpp"
#include <cmath>
#include <string>
#include <vector>
MapParsingMethods ObjectParser::parsingMethods = {{"v", &ObjectParser::defineVertex},
                                                  {"f", &ObjectParser::defineFace},
                                                  {"s", &ObjectParser::defineSmoothShading},
                                                  {"usemtl", &ObjectParser::defineMTL}};

ObjectData ObjectParser::objectValue;

std::tuple<std::vector<Object>, std::vector<Material>> ObjectParser::parseObjectFile(const std::string &path)
{
    std::tuple<std::vector<Object>, std::vector<Material>> data;

    std::stringstream fileStream = Utils::readFile(path);
    fileStream = Utils::readFile(path);
    std::string line;
    unsigned int lineIndex = 1;
    while (std::getline(fileStream, line))
    {
        line = line.substr(0, line.find("#"));
        std::string symbol = line.substr(0, line.find(" "));
        auto it = ObjectParser::parsingMethods.find(symbol);
        if (symbol == "o")
        {
            Object newObject(objectValue);
            std::get<0>(data).push_back(newObject);
            objectValue.reset();
            defineName(line, lineIndex);
        }
        else if (symbol == "mtllib")
        {
            std::vector<Material> newMaterials = createNewMTL(line, lineIndex);
            std::get<1>(data).insert(std::get<1>(data).end(), newMaterials.begin(), newMaterials.end());
        }
        else if (it != ObjectParser::parsingMethods.end())
            (it->second)(line, lineIndex);
        else if (symbol.length() != 0)
            throw(Exception("PARSE_OBJECT_FILE", "INVALID_SYMBOL", line, lineIndex));
        lineIndex++;
    }

    return (data);
}

void ObjectParser::defineName(const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line);
    if (words.size() != 2)
        throw(Exception("DEFINE_NAME", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));
    objectValue.setName(words[1]);
}

void ObjectParser::defineVertex(const std::string &line, unsigned int lineIndex)
{
    Vertex vertex;
    std::vector<std::string> words;

    words = Utils::splitLine(line);
    if (words.size() < 4 || words.size() > 5)
        throw(Exception("DEFINE_VERTEX", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    for (size_t i = 1; i < words.size(); i++)
        vertex.push_back(std::stof(words[i]));
    if (words.size() == 4)
        vertex.push_back(1.0f);

    objectValue.addVertex(vertex);
}

void ObjectParser::defineFace(const std::string &line, unsigned int lineIndex)
{
    Face face;
    int vertexID;
    const int nbVertices = static_cast<int>(objectValue.getVertices().size());
    std::vector<std::string> words;

    words = Utils::splitLine(line);
    if (words.size() < 4)
        throw(Exception("DEFINE_FACE", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    for (size_t i = 1; i < words.size(); i++)
    {
        vertexID = std::stoi(words[i]);
        if (vertexID < -nbVertices || vertexID > nbVertices)
            throw(Exception("DEFINE_FACE", "INVALID_VERTEX_INDEX", line, lineIndex));

        if (vertexID < 0)
            vertexID = nbVertices + 1 + vertexID;
        face.push_back(vertexID - 1);
    }
    triangulate(face);
}

void ObjectParser::triangulate(Face &face)
{
    while (face.size() > 3)
    {
        for (size_t i = 1; i < face.size() - 1; i++)
        {
            Vertex a = objectValue.getVertices()[face[i]];
            Vertex b = objectValue.getVertices()[face[i - 1]];
            Vertex c = objectValue.getVertices()[face[i + 1]];

            bool isEar = true;
            for (size_t j = 0; j < objectValue.getVertices().size(); j++)
            {
                Vertex p = objectValue.getVertices()[j];
                if (p == a || p == b || p == c)
                    continue;
                if (insideTriangle(p, a, b, c))
                {
                    isEar = false;
                    break;
                }
            }
            if (isEar)
            {
                Face newFace;

                newFace.push_back(face[i - 1]);
                newFace.push_back(face[i]);
                newFace.push_back(face[i + 1]);
                for (std::vector<int>::iterator it = face.begin(); it != face.end(); it++)
                {
                    if (*it == static_cast<int>(face[i]))
                    {
                        face.erase(it);
                        break;
                    }
                }
                objectValue.addFace(newFace);
                break;
            }
        }
    }
    objectValue.addFace(face);
}

bool ObjectParser::insideTriangle(const Vertex &p, const Vertex &a, const Vertex &b, const Vertex &c)
{
    float areaABC = triangleArea(a, b, c);
    float areaABP = triangleArea(a, b, p);
    float areaBCP = triangleArea(b, c, p);
    float areaACP = triangleArea(a, c, p);

    if (areaABC == areaABP + areaBCP + areaACP && areaABP != 0 && areaBCP != 0 && areaACP != 0)
        return (true);
    else
        return (false);
}

float ObjectParser::triangleArea(const Vertex &a, const Vertex &b, const Vertex &c)
{
    Vertex AB;
    AB.push_back(b[0] - a[0]);
    AB.push_back(b[1] - a[1]);
    AB.push_back(b[2] - a[2]);

    Vertex AC;
    AC.push_back(c[0] - a[0]);
    AC.push_back(c[1] - a[1]);
    AC.push_back(c[2] - a[2]);

    float dotProduct = AB[0] * AC[0] + AB[1] * AC[1] + AB[2] * AC[2];
    float magnitudeAB = sqrt(pow(AB[0], 2) + pow(AB[1], 2) + pow(AB[2], 2));
    float magnitudeAC = sqrt(pow(AC[0], 2) + pow(AC[1], 2) + pow(AC[2], 2));
    float cosAngle = dotProduct / (magnitudeAB * magnitudeAC);
    float angle = acos(cosAngle);
    float area = 0.5f * magnitudeAB * magnitudeAC * sin(angle);
    return (area);
}

void ObjectParser::defineSmoothShading(const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line);
    if (words.size() != 2)
        throw(Exception("DEFINE_SMOOTH_SHADING", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    if (words[1] == "on" || words[1] == "1")
        objectValue.setSmoothShading(true);
    else if (words[1] == "off" || words[1] == "0")
        objectValue.setSmoothShading(false);
    else
        throw(Exception("DEFINE_SMOOTH_SHADING", "INVALID_ARGUMENT", line, lineIndex));
}

std::vector<Material> ObjectParser::createNewMTL(const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line);
    if (words.size() != 2)
        throw(Exception("CREATE_NEW_MTL", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    // parseMtlFile(words[1]);
    std::vector<Material> tmp;
    return (tmp); // tmp
}

void ObjectParser::defineMTL(const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line);
    if (words.size() != 2)
        throw(Exception("DEFINE_MTL", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    // objectValue.material = words[1];

    // def it like that but need to add a check when using material to see if they exist
    // or add a check at the end of the parsing
}

ObjectParser::Exception::Exception(const std::string &functionName, const std::string &errorMessage,
                                   const std::string &line, unsigned int lineIndex)
{
    this->errorMessage = "OBJECT_PARSER::" + functionName + "::" + errorMessage;
    this->errorMessage += "\n|\n| " + std::to_string(lineIndex) + ": " + line + "\n|";
}

const char *ObjectParser::Exception::what(void) const throw()
{
    return (errorMessage.c_str());
}