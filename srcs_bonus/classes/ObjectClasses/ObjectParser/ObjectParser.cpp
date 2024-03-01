#include "ObjectParser.hpp"
#include "../../MaterialClasses/MaterialParser/MaterialParser.hpp"
#include "../../Utils/Utils.hpp"
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
MapObjectParsingMethods ObjectParser::parsingMethods = {
    {"v", &ObjectParser::defineVertex},    {"vt", &ObjectParser::defineTextureVertex},
    {"f", &ObjectParser::defineFace},      {"s", &ObjectParser::defineSmoothShading},
    {"mtllib", &ObjectParser::saveNewMTL}, {"usemtl", &ObjectParser::defineMTL}};

std::vector<Material> ObjectParser::materials;

std::vector<Object> ObjectParser::parseObjectFile(const std::string &path)
{
    if (!Utils::checkExtension(path, ".obj"))
        throw(Exception("PARSE_OBJECT_FILE", "INVALID_EXTENSION", path, 0));

    ObjectData objectData;
    std::vector<Object> objects;
    std::stringstream fileStream = Utils::readFile(path);
    fileStream = Utils::readFile(path);
    std::string line;
    unsigned int lineIndex = 1;
    while (std::getline(fileStream, line))
    {
        line = line.substr(0, line.find("#"));
        std::string symbol = line.substr(0, line.find(" "));
        auto it = parsingMethods.find(symbol);
        if (symbol == "o")
        {
            if (objectData.getFaces().size() > 0)
            {
                Object newObject(objectData);
                objects.push_back(newObject);
            }
            objectData.reset();
            defineName(objectData, line, lineIndex);
        }
        else if (it != parsingMethods.end())
            (it->second)(objectData, line, lineIndex);
        else if (symbol.length() != 0)
            throw(Exception("PARSE_OBJECT_FILE", "INVALID_SYMBOL", line, lineIndex));
        lineIndex++;
    }
    if (objectData.getFaces().size() > 0)
    {
        Object newObject(objectData);
        objects.push_back(newObject);
    }
    return (objects);
}

void ObjectParser::defineName(ObjectData &objectData, const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() != 2)
        throw(Exception("DEFINE_NAME", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));
    objectData.setName(words[1]);
}

void ObjectParser::defineVertex(ObjectData &objectData, const std::string &line, unsigned int lineIndex)
{
    Vertex vertex;
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() < 4 || words.size() > 5)
        throw(Exception("DEFINE_VERTEX", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    for (size_t i = 1; i < words.size(); i++)
    {
        if (!Utils::isFloat(words[i]))
            throw(Exception("DEFINE_VERTEX", "INVALID_ARGUMENT", line, lineIndex));
        vertex.push_back(std::stof(words[i]));
    }
    if (words.size() == 4)
        vertex.push_back(1.0f);

    if (vertex[3] == 0)
        throw(Exception("DEFINE_VERTEX", "INVALID_ARGUMENT", line, lineIndex));

    for (size_t i = 0; i < 3; i++)
        vertex[i] = vertex[i] / vertex[3];

    objectData.addVertex(vertex);
}

void ObjectParser::defineTextureVertex(ObjectData &objectData, const std::string &line, unsigned int lineIndex)
{
    Vertex textureVertex;
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() < 3 || words.size() > 4)
        throw(Exception("DEFINE_VERTEX_TEXTURE", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    for (size_t i = 1; i < words.size(); i++)
    {
        if (!Utils::isFloat(words[i]))
            throw(Exception("DEFINE_VERTEX_TEXTURE", "INVALID_ARGUMENT", line, lineIndex));
        float value = std::stof(words[i]);
        if (value < 0 || value > 1)
            throw(Exception("DEFINE_VERTEX_TEXTURE", "INVALID_ARGUMENT", line, lineIndex));
        textureVertex.push_back(value);
    }
    for (size_t i = textureVertex.size(); i < 3; i++)
        textureVertex.push_back(0);

    objectData.addTextureVertex(textureVertex);
}

void ObjectParser::defineFace(ObjectData &objectData, const std::string &line, unsigned int lineIndex)
{
    Face face;
    std::vector<std::string> words = Utils::splitLine(line, " ");
    if (words.size() < 4)
        throw(Exception("DEFINE_FACE", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    for (size_t i = 1; i < words.size(); i++)
    {
        int nbBackSlash = 0;
        for (size_t j = 0; j < words[i].size(); j++)
        {
            if (words[i][j] == '/')
                nbBackSlash++;
        }
        if (nbBackSlash != 1)
            throw(Exception("DEFINE_FACE", "INVALID_ARGUMENT", line, lineIndex));

        std::vector<std::string> vertices = Utils::splitLine(words[i], "/");
        if (vertices.size() != 2)
            throw(Exception("DEFINE_FACE", "INVALID_ARGUMENT", line, lineIndex));

        size_t vertexIndex = CalculateVertexIndex(objectData, vertices[0], CLASSIC, line, lineIndex);
        size_t textureVertexIndex = CalculateVertexIndex(objectData, vertices[1], TEXTURE, line, lineIndex);
        face.push_back(CombineVertices(objectData, vertexIndex, textureVertexIndex));
    }
    triangulate(objectData, face);
}

size_t ObjectParser::CalculateVertexIndex(ObjectData &objectData, const std::string &vertex, e_vertexType vertexType,
                                          const std::string &line, unsigned int lineIndex)
{
    int nbVertices;
    std::string errorMessage;

    if (vertexType == CLASSIC)
    {
        nbVertices = objectData.getVertices().size();
        errorMessage = "INVALID_VERTEX_INDEX";
    }
    else
    {
        nbVertices = objectData.getTextureVertices().size();
        errorMessage = "INVALID_TEXTURE_VERTEX_INDEX";
    }

    if (!Utils::isInt(vertex))
        throw(Exception("DEFINE_FACE", "INVALID_ARGUMENT", line, lineIndex));
    int vertexIndex = std::stoi(vertex);
    if (vertexIndex < -nbVertices || vertexIndex > nbVertices || vertexIndex == 0)
        throw(Exception("DEFINE_FACE", errorMessage, line, lineIndex));

    if (vertexIndex < 0)
        vertexIndex = nbVertices + 1 + vertexIndex;

    return (vertexIndex);
}

size_t ObjectParser::CombineVertices(ObjectData &objectData, size_t vertexIndex, size_t textureVertexIndex)
{
    Vertex vertex = objectData.getVertices()[vertexIndex - 1];
    Vertex textureVertex = objectData.getTextureVertices()[textureVertexIndex - 1];
    Vertex combinedVertex;

    for (size_t i = 0; i < 4; i++)
        combinedVertex.push_back((vertex[i]));
    for (size_t i = 0; i < 3; i++)
        combinedVertex.push_back((textureVertex[i]));

    std::vector<Vertex> combinedVertices = objectData.getCombinedVertices();
    auto it = std::find(combinedVertices.begin(), combinedVertices.end(), combinedVertex);
    if (it == combinedVertices.end())
        objectData.addCombinedVertex(combinedVertex);
    return (std::distance(combinedVertices.begin(), it));
}

void ObjectParser::triangulate(ObjectData &objectData, Face &face)
{
    while (face.size() > 3)
    {
        for (size_t i = 1; i < face.size() - 1; i++)
        {
            Vertex a = objectData.getCombinedVertices()[face[i]];
            Vertex b = objectData.getCombinedVertices()[face[i - 1]];
            Vertex c = objectData.getCombinedVertices()[face[i + 1]];

            bool isEar = true;
            for (size_t j = 0; j < objectData.getCombinedVertices().size(); j++)
            {
                Vertex p = objectData.getCombinedVertices()[j];
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
                objectData.addFace(newFace);
                break;
            }
        }
    }
    objectData.addFace(face);
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

void ObjectParser::defineSmoothShading(ObjectData &objectData, const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() != 2)
        throw(Exception("DEFINE_SMOOTH_SHADING", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    if (words[1] == "on" || words[1] == "1")
        objectData.setSmoothShading(true);
    else if (words[1] == "off" || words[1] == "0")
        objectData.setSmoothShading(false);
    else
        throw(Exception("DEFINE_SMOOTH_SHADING", "INVALID_ARGUMENT", line, lineIndex));
}

void ObjectParser::saveNewMTL(ObjectData &objectData, const std::string &line, unsigned int lineIndex)
{
    (void)objectData;
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() != 2)
        throw(Exception("CREATE_NEW_MTL", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    std::vector<Material> newMaterials = MaterialParser::parseMaterialFile(words[1]);
    materials.insert(materials.end(), newMaterials.begin(), newMaterials.end());
}

void ObjectParser::defineMTL(ObjectData &objectData, const std::string &line, unsigned int lineIndex)
{
    std::vector<std::string> words;

    words = Utils::splitLine(line, " ");
    if (words.size() != 2)
        throw(Exception("DEFINE_MTL", "INVALID_NUMBER_OF_ARGUMENTS", line, lineIndex));

    for (size_t i = 0; i < materials.size(); i++)
    {
        if (materials[i].getName() == words[1])
        {
            objectData.setMaterial(materials[i]);
            return;
        }
    }
    throw(Exception("DEFINE_MTL", "INVALID_ARGUMENT", line, lineIndex));
}

ObjectParser::Exception::Exception(const std::string &functionName, const std::string &errorMessage,
                                   const std::string &line, unsigned int lineIndex)
{
    this->errorMessage = "\nOBJECT_PARSER::" + functionName + "::" + errorMessage;
    this->errorMessage += "\n|\n| " + std::to_string(lineIndex) + ": " + line + "\n|";
}

const char *ObjectParser::Exception::what(void) const throw()
{
    return (errorMessage.c_str());
}