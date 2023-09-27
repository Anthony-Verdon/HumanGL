#include "Object.hpp"

ParsingFunctions Object::parsingFunctions = {
        {"s", &Object::defineSmoothShading},
        {"v", &Object::defineVertex},
        {"f", &Object::defineFace},
        {"mtllib", &Object::defineMTL},
        {"usemtl", &Object::useMTL}
};

Object::Object()
{
}

Object::Object(const std::string &name)
{
    this->name = name;
    smoothShading = false;
    VAOInit = false;
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
        name = copy.getName();
        smoothShading = copy.getSmoothShading();
    }
    return (*this);
}

Object::~Object()
{
    if (VAOInit)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

std::vector<std::vector<float> >Object::getVertices() const
{
    return (vertices);
}

float *Object::getVerticesIntoArray() const
{
    size_t j;
    float *array;

    j = 0;
    array = new float[vertices.size() * 4 * sizeof(float)];
    for (size_t i = 0; i < vertices.size(); i++)
    {
        array[j] = vertices[i][0];
        array[j + 1] = vertices[i][1];
        array[j + 2] = vertices[i][2];
        array[j + 3] = vertices[i][3];
        j += 4;
    }
    return (array);
}

unsigned int *Object::getFacesIntoArray() const
{
    size_t j;
    unsigned int *array;

    j = 0;
    array = new unsigned int[faces.size() * 3 * sizeof(unsigned int)];
    for (size_t i = 0; i < faces.size(); i++)
    {
        array[j] = faces[i][0];
        array[j + 1] = faces[i][1];
        array[j + 2] = faces[i][2];
        j += 3;
    }

    return (array);
}

std::vector<Face> Object::getFaces() const
{
    return (faces);
}

std::string Object::getName() const
{
    return (name);
}

bool Object::getSmoothShading() const
{
    return (smoothShading);
}

unsigned int Object::getVAO() const
{
    return (VAO);
}

void Object::setName(const std::string &name)
{
    this->name = name;
}

void Object::setVertices(const Vertices &vertices)
{
    this->vertices = vertices;
}

void Object::setFaces(const std::vector<Face> &faces)
{
    this->faces = faces;
}

void Object::setSmoothShading(const bool &smoothShading)
{
    this->smoothShading = smoothShading;
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

float Object::triangleArea(const Vertex &a, const Vertex &b, const Vertex &c) const
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

bool Object::insideTriangle(const Vertex &p, const Vertex &a, const Vertex &b, const Vertex &c) const
{
    float areaABC = triangleArea(a, b, c);
    float areaABP = triangleArea(a, b, p);
    float areaBCP = triangleArea(b, c, p);
    float areaACP = triangleArea(a, c, p);

    if (areaABC == areaABP + areaBCP + areaACP
        && areaABP != 0 && areaBCP != 0 && areaACP != 0)
        return (true);
    else
        return (false);
}

void Object::triangulate(Face &face)
{
    while (face.size() > 3)
    {
        for (size_t i = 1; i < face.size() - 1; i++)
        {
            Vertex a = vertices[face[i]];
            Vertex b = vertices[face[i - 1]];
            Vertex c = vertices[face[i + 1]];

            bool isEar = true;
            for (size_t j = 0; j < vertices.size(); j++)
            {
                Vertex p = vertices[j];
                if (p == a || p == b ||  p == c)
                    continue;
                if (insideTriangle(p, a, b, c))
                {
                    isEar = false;
                    break ;
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
                faces.push_back(newFace);
                break ;
            }
        }
    }
    faces.push_back(face);
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
        face.push_back(vertexID - 1);
    }
    #ifdef DEBUG
        for (size_t i = 0; i < face.size(); i++)
            std::cout << face[i] << std::endl;
    #endif
    triangulate(face);
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

void Object::initVAO()
{
    float *verticesArray;
    unsigned int *facesArray;

    verticesArray = getVerticesIntoArray();
    facesArray = getFacesIntoArray();

    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 4, verticesArray, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faces.size() * 3, facesArray, GL_STATIC_DRAW); 
    
    delete []verticesArray;
    delete []facesArray;
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    VAOInit = true;
}

std::ostream& operator << (std::ostream& os, const Object& instance)
{
    os << "name: " << instance.getName() << std::endl;
    os << "smooth shading: " << instance.getSmoothShading() << std::endl;
    Vertices objectVertices = instance.getVertices();
    os << "vertices: " << std::endl;
    for (size_t i = 0; i < objectVertices.size(); i++)
    {
        os << "vertex nb " << i + 1 << ": ";
        os << objectVertices[i][0] << " " << objectVertices[i][1] << " ";
        os << objectVertices[i][2] << " " << objectVertices[i][3]<< std::endl;
    }
    std::vector<Face> objectFace = instance.getFaces();
    os << "faces: " << std::endl;
    for (size_t i = 0; i < objectFace.size(); i++)
    {
        os << "face nb " << i + 1 << ": ";
        for (size_t j = 0; j < objectFace[i].size(); j++)
            os << objectFace[i][j] << " ";
        os << std::endl;
    }
    return (os);
}