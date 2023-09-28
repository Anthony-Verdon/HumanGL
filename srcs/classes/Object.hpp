#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <map>
#include <cmath>
#include "../../includes/glad/glad.h"

class Object;

typedef std::vector<float> Vertex;
typedef std::vector<Vertex> Vertices;
typedef std::vector<int> Face;
typedef std::map<std::string, void (Object::*)(std::string)> ParsingFunctions;
typedef std::map<std::string, void (Object::*)(std::string)>::iterator ParsingFunctionsIterator;

class Object
{
    private:
        Object();
        float triangleArea(const Vertex &a, const Vertex &b, const Vertex &c) const;
        bool insideTriangle(const Vertex &p, const Vertex &a, const Vertex &b, const Vertex &c) const;
        void triangulate(Face &face);

        std::string name;
        Vertices vertices;
        std::vector<Face> faces;
        bool smoothShading;

        bool VAOInit;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        //Material array

        //position
        //rotation
        //scale ?
        // ... (everything link to matrices)

    public:
        Object(const std::string &name);
        Object(const Object &copy);
        Object &operator=(const Object &copy);
        ~Object();

        std::string getName() const;
        Vertices getVertices() const;
        float *getVerticesIntoArray() const;
        std::vector<Face> getFaces() const;
        unsigned int *getFacesIntoArray() const;
        bool getSmoothShading() const;

        unsigned int getVAO() const;
        void setName(const std::string &name);
        void setVertices(const Vertices &vertices);
        void setFaces(const std::vector<Face> &faces);
        void setSmoothShading(bool smoothShading);

        void defineVertex(std::string line);
        void defineFace(std::string line);
        void defineSmoothShading(std::string line);
        void defineMTL(std::string line);
        void useMTL(std::string line);

        void initVAO();

        static ParsingFunctions parsingFunctions;
};

std::ostream& operator << (std::ostream &os, const Object &instance);
#endif