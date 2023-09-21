#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

class Object;

typedef std::vector<float> Vertex;
typedef std::vector<Vertex> Vertices;
typedef std::vector<Vertices> Faces;
typedef std::map<std::string, void (Object::*)(std::string)> ParsingFunctions;
typedef std::map<std::string, void (Object::*)(std::string)>::iterator ParsingFunctionsIterator;

class Object
{
    private:
        Object();

        static ParsingFunctions parsingFunctions;
        Vertices vertices;
        Faces faces;

        //position
        //rotation
        //scale ?
        // ... (everything link to matrices)

    public:
        Object (const std::string &objPath);
        Object(const Object &copy);
        Object &operator=(const Object &copy);
        ~Object();

        Vertices getVertices() const;
        Faces getFaces() const;

        void setVertices(Vertices vertices);
        void setFaces(Faces faces);

        void ignore(std::string line);
        void defineVertex(std::string line);
        void defineFace(std::string line);
        void defineMTL(std::string line);
        void defineSmoothShading(std::string line);
        void useMTL(std::string line);
};

#endif