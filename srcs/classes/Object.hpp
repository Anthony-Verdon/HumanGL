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
#include "Material.hpp"

class Object;

typedef std::vector<float> Vertex;
typedef std::vector<Vertex> Vertices;
typedef std::vector<int> Face;
typedef std::map<std::string, void (Object::*)(std::string, unsigned int)> ObjectParsingFunctions;
typedef std::map<std::string, void (Object::*)(std::string, unsigned int)>::iterator ObjectParsingFunctionsIterator;

class Object
{
    private:
        Object();
        float triangleArea(const Vertex &a, const Vertex &b, const Vertex &c) const;
        bool insideTriangle(const Vertex &p, const Vertex &a, const Vertex &b, const Vertex &c) const;
        void triangulate(Face &face);

        void defineVertex(std::string line, unsigned int lineIndex);
        void defineFace(std::string line, unsigned int lineIndex);
        void defineSmoothShading(std::string line, unsigned int lineIndex);
        void defineMTL(std::string line, unsigned int lineIndex);
        void useMTL(std::string line, unsigned int lineIndex);

        std::string name;
        Vertices vertices;
        std::vector<Face> faces;
        bool smoothShading;
        unsigned int materialIndex;
        
        bool VAOInit;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        static std::vector<Material> materials;

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
        unsigned int getMaterialIndex() const;
        unsigned int getVAO() const;

        void setName(const std::string &name);
        void setVertices(const Vertices &vertices);
        void setFaces(const std::vector<Face> &faces);
        void setSmoothShading(bool smoothShading);
        void setMaterialIndex(unsigned int index);

        void initVAO();

        static void addMaterial(Material material);
        static std::vector<Material> getMaterials();
        static Material getMaterial(unsigned int index);
        static ObjectParsingFunctions parsingFunctions;
};

std::ostream& operator << (std::ostream &os, const Object &instance);
#endif