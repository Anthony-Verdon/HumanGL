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
        unsigned int calculateNbIndices() const;
        float triangleArea(Vertex a, Vertex b, Vertex c) const;
        bool insideTriangle(Vertex x, Vertex a, Vertex b, Vertex c) const;

        std::string name;
        Vertices vertices;
        std::vector<Face> faces;
        bool smoothShading;
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

        Vertices getVertices() const;
        std::vector<Face> getFaces() const;
        float *getVerticesIntoArray() const;
        unsigned int *getFacesIntoArray() const;
        std::string getName() const;
        bool getSmoothShading() const;

        void setVertices(Vertices vertices);
        void setFaces(std::vector<Face> faces);

        std::vector<std::string> splitLine(std::string line);
        void defineVertex(std::string line);
        void triangulate(Face face);
        void defineFace(std::string line);
        void defineObject(std::string line);
        void defineSmoothShading(std::string line);
        void defineMTL(std::string line);
        void useMTL(std::string line);

        static ParsingFunctions parsingFunctions;
};

std::ostream& operator << (std::ostream& os, const Object& instance);
#endif