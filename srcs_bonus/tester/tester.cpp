#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../classes/Utils/Utils.hpp"

// allow to use private methods only on this file
#define private public

#include "../../libs/doctest.h"
#include "../classes/MaterialClasses/MaterialParser/MaterialParser.hpp"
#include "../classes/ObjectClasses/ObjectParser/ObjectParser.hpp"

// maybe test texture too
TEST_CASE("test useful functions")
{
    SUBCASE("test the splitLine function")
    {
        std::string line1 = "hi ! How are you ?";
        std::vector<std::string> lineSplit1 = {"hi", "!", "How", "are", "you", "?"};
        CHECK_MESSAGE(Utils::splitLine(line1, " ") == lineSplit1, "line: ", line1);
        std::string line2 = "     multiples     spaces     ";
        std::vector<std::string> lineSplit2 = {"multiples", "spaces"};
        CHECK_MESSAGE(Utils::splitLine(line2, " ") == lineSplit2, "line: ", line2);
        std::string line3 = "";
        std::vector<std::string> lineSplit3 = {};
        CHECK_MESSAGE(Utils::splitLine(line3, " ") == lineSplit3, "line: {nothing}");
        std::string line4 = "            ";
        std::vector<std::string> lineSplit4 = {};
        CHECK_MESSAGE(Utils::splitLine(line4, " ") == lineSplit4, "line: {only space}");
    }

    // didn't test degToRad function because comparaison of float is always false

    SUBCASE("test the if a path has the good extension")
    {
        CHECK(Utils::checkExtension("object.obj", ".obj"));
        CHECK(Utils::checkExtension("ressources/object.obj", ".obj"));
        CHECK(Utils::checkExtension("..obj", ".obj"));
        CHECK_FALSE(Utils::checkExtension("material.mtl", ".obj"));
        CHECK_FALSE(Utils::checkExtension(".obj", ".obj"));
        CHECK_FALSE(Utils::checkExtension("ressources/.obj", ".obj"));
    }

    SUBCASE("testing if a string can be converted to an int")
    {
        CHECK(Utils::isInt("0123456789"));
        CHECK(Utils::isInt("-1"));
        CHECK_FALSE(Utils::isInt("0.1"));
        CHECK_FALSE(Utils::isInt("1-"));
        CHECK_FALSE(Utils::isInt("-100-1"));
        CHECK_FALSE(Utils::isInt("1a"));
        CHECK_FALSE(Utils::isInt("a1"));
    }

    SUBCASE("testing if a string can be converted to a float")
    {
        CHECK(Utils::isFloat("0123456789"));
        CHECK(Utils::isFloat("0.01"));
        CHECK(Utils::isFloat("-0.01"));
        CHECK_FALSE(Utils::isFloat(".1"));
        CHECK_FALSE(Utils::isFloat("0..1"));
        CHECK_FALSE(Utils::isFloat("-.1"));
        CHECK_FALSE(Utils::isFloat("a.1"));
        CHECK_FALSE(Utils::isFloat("1-"));
        CHECK_FALSE(Utils::isFloat("-100-1"));
        CHECK_FALSE(Utils::isFloat("1a"));
        CHECK_FALSE(Utils::isFloat("a1"));
    }
}

TEST_CASE("test the definition of an object")
{
    SUBCASE("testing the parsing of a file with an invalid extension")
    {
        CHECK_NOTHROW(ObjectParser::parseObjectFile("srcs_bonus/tester/ressources/object.obj"));
        CHECK_NOTHROW(ObjectParser::parseObjectFile("srcs_bonus/tester/ressources/..obj"));
        CHECK_THROWS(ObjectParser::parseObjectFile("objectWrongExtension.obja"));
        CHECK_THROWS(ObjectParser::parseObjectFile("objectWrongExtension.o"));
        CHECK_THROWS(ObjectParser::parseObjectFile("objectWrongExtension.ob"));
        CHECK_THROWS(ObjectParser::parseObjectFile("objectWrongExtensionobj"));
        CHECK_THROWS(ObjectParser::parseObjectFile(".obj"));
        CHECK_THROWS(ObjectParser::parseObjectFile("ressources/.obj"));
    }

    SUBCASE("testing the parsing of a file with an invalid symbol")
    {
        CHECK_NOTHROW(ObjectParser::parseObjectFile("srcs_bonus/tester/ressources/object.obj"));
        CHECK_THROWS(ObjectParser::parseObjectFile("srcs_bonus/tester/ressources/objectWrongSymbol.obj"));
    }

    ObjectData objectData;
    SUBCASE("testing the definition of a name")
    {
        objectData.reset();
        CHECK_NOTHROW(ObjectParser::defineName(objectData, "o name ", 0));
        CHECK_THROWS(ObjectParser::defineName(objectData, "o ", 0));
        CHECK_THROWS(ObjectParser::defineName(objectData, "o name1 name2", 0));
    }

    SUBCASE("testing the definition of a vertex")
    {
        objectData.reset();
        CHECK_NOTHROW(ObjectParser::defineVertex(objectData, "v -0.5 0 0.5 ", 0));
        CHECK_NOTHROW(ObjectParser::defineVertex(objectData, "v -0.5 0 0.5 1", 0));
        CHECK_THROWS(ObjectParser::defineVertex(objectData, "v -0.5 0 0 0", 0));
        CHECK_THROWS(ObjectParser::defineVertex(objectData, "v -0.5 0", 0));
        CHECK_THROWS(ObjectParser::defineVertex(objectData, "v -0.5 0 0.5 1 1", 0));
        CHECK_THROWS(ObjectParser::defineVertex(objectData, "v -0.5a 0 0.5", 0));
        CHECK_THROWS(ObjectParser::defineVertex(objectData, "v --0.5 0 0.5", 0));
        CHECK_THROWS(ObjectParser::defineVertex(objectData, "v -.5 0 0.5", 0));
        CHECK_THROWS(ObjectParser::defineVertex(objectData, "v .5 0 0.5", 0));
        CHECK_THROWS(ObjectParser::defineVertex(objectData, "v ..5 0 0.5", 0));
    }

    SUBCASE("testing the definition of a texture vertex")
    {
        objectData.reset();
        CHECK_NOTHROW(ObjectParser::defineTextureVertex(objectData, "vt 0 0 ", 0));
        CHECK_NOTHROW(ObjectParser::defineTextureVertex(objectData, "vt 0.5 0.5 ", 0));
        CHECK_NOTHROW(ObjectParser::defineTextureVertex(objectData, "vt 1 1 ", 0));
        CHECK_NOTHROW(ObjectParser::defineTextureVertex(objectData, "vt 1 1 0 ", 0));
        CHECK_THROWS(ObjectParser::defineTextureVertex(objectData, "vt 0.5", 0));
        CHECK_THROWS(ObjectParser::defineTextureVertex(objectData, "vt 0.5 0.5 0.5 0.5", 0));
        CHECK_THROWS(ObjectParser::defineTextureVertex(objectData, "vt -0.5 0.5", 0));
        CHECK_THROWS(ObjectParser::defineTextureVertex(objectData, "vt 0..5 0.5", 0));
        CHECK_THROWS(ObjectParser::defineTextureVertex(objectData, "vt 0.5a 0.5", 0));
    }

    SUBCASE("testing the definition of a face")
    {
        objectData.reset();
        ObjectParser::defineVertex(objectData, "v 0 0 0 ", 0);
        ObjectParser::defineVertex(objectData, "v 0 1 0", 0);
        ObjectParser::defineVertex(objectData, "v 1 1 0 ", 0);
        ObjectParser::defineVertex(objectData, "v 1 0 0 ", 0);
        ObjectParser::defineTextureVertex(objectData, "vt 0 0 ", 0);
        ObjectParser::defineTextureVertex(objectData, "vt 0 1", 0);
        ObjectParser::defineTextureVertex(objectData, "vt 1 1 ", 0);
        ObjectParser::defineTextureVertex(objectData, "vt 1 0 ", 0);
        CHECK_NOTHROW(ObjectParser::defineFace(objectData, "f 1/1 2/2 3/3", 0));
        CHECK_NOTHROW(ObjectParser::defineFace(objectData, "f -1/-1 -2/-2 -3/-3", 0));
        CHECK_NOTHROW(ObjectParser::defineFace(objectData, "f 1/1 2/2 3/3 4/4", 0));
        CHECK_THROWS(ObjectParser::defineFace(objectData, "f 1/1 2/2", 0));
        CHECK_THROWS(ObjectParser::defineFace(objectData, "f 1/1 2/2 3/3 4/4 5/5", 0));
        CHECK_THROWS(ObjectParser::defineFace(objectData, "f 1/1 2/2 3/3 4/4 -5/-5", 0));
        CHECK_THROWS(ObjectParser::defineFace(objectData, "f /1/1 2/2 3/3 4/4", 0));
        CHECK_THROWS(ObjectParser::defineFace(objectData, "f 1//1 2/2 3/3 4/4", 0));
        CHECK_THROWS(ObjectParser::defineFace(objectData, "f 1/1/ 2/2 3/3 4/4", 0));
        CHECK_THROWS(ObjectParser::defineFace(objectData, "f 1/1 / 2/2 3/3 4/4", 0));
        CHECK_THROWS(ObjectParser::defineFace(objectData, "f 1/1 / 2/2 3/3 4/4a", 0));
        CHECK_THROWS(ObjectParser::defineFace(objectData, "f 1/1 / 2/2 3/3 4/3.5", 0));
    }

    SUBCASE("testing the definition of smooth shading")
    {
        objectData.reset();
        CHECK_NOTHROW(ObjectParser::defineSmoothShading(objectData, "s on", 0));
        CHECK_NOTHROW(ObjectParser::defineSmoothShading(objectData, "s off", 0));
        CHECK_NOTHROW(ObjectParser::defineSmoothShading(objectData, "s 0", 0));
        CHECK_NOTHROW(ObjectParser::defineSmoothShading(objectData, "s 1", 0));
        CHECK_THROWS(ObjectParser::defineSmoothShading(objectData, "s", 0));
        CHECK_THROWS(ObjectParser::defineSmoothShading(objectData, "s 1 1", 0));
        CHECK_THROWS(ObjectParser::defineSmoothShading(objectData, "s -1", 0));
        CHECK_THROWS(ObjectParser::defineSmoothShading(objectData, "s 2", 0));
        CHECK_THROWS(ObjectParser::defineSmoothShading(objectData, "s iufhrng", 0));
    }

    SUBCASE("testing the usage of a mtllib")
    {
        objectData.reset();
        CHECK_NOTHROW(ObjectParser::saveNewMTL(objectData, "mtllib srcs_bonus/tester/ressources/material.mtl", 0));
        CHECK(ObjectParser::materials.size() == 2);
        CHECK(ObjectParser::materials[0].getName() == "Material");
        CHECK(ObjectParser::materials[1].getName() == "Material2");
        CHECK_THROWS(ObjectParser::saveNewMTL(objectData, "mtllib ", 0));
        CHECK_THROWS(ObjectParser::saveNewMTL(objectData, "mtllib first.mtl second.mtl", 0));
    }

    SUBCASE("testing the definition of a material")
    {
        objectData.reset();
        ObjectParser::saveNewMTL(objectData, "objectData,mtllib srcs_bonus/tester/ressources/material.mtl", 0);
        CHECK_NOTHROW(ObjectParser::defineMTL(objectData, "usemtl Material", 0));
        CHECK_NOTHROW(ObjectParser::defineMTL(objectData, "usemtl Material2", 0));
        CHECK_THROWS(ObjectParser::defineMTL(objectData, "usemtl ", 0));
        CHECK_THROWS(ObjectParser::defineMTL(objectData, "usemtl first second", 0));
        CHECK_THROWS(ObjectParser::defineMTL(objectData, "usemtl dontExist ", 0));
    }
}

TEST_CASE("test the definition of a material")
{
    SUBCASE("testing the parsing of a file with an invalid extension")
    {
        CHECK_NOTHROW(MaterialParser::parseMaterialFile("srcs_bonus/tester/ressources/material.mtl"));
        CHECK_NOTHROW(MaterialParser::parseMaterialFile("srcs_bonus/tester/ressources/..mtl"));
        CHECK_THROWS(MaterialParser::parseMaterialFile("materialWrongExtension..mtla"));
        CHECK_THROWS(MaterialParser::parseMaterialFile("materialWrongExtension.m"));
        CHECK_THROWS(MaterialParser::parseMaterialFile("materialWrongExtension.mt"));
        CHECK_THROWS(MaterialParser::parseMaterialFile("materialWrongExtensionmtl"));
        CHECK_THROWS(MaterialParser::parseMaterialFile(".mtl"));
        CHECK_THROWS(MaterialParser::parseMaterialFile("ressources/.mtl"));
    }

    SUBCASE("testing the parsing of a file with an invalid symbol")
    {
        CHECK_NOTHROW(MaterialParser::parseMaterialFile("srcs/tester/ressources/material.mtl"));
        CHECK_THROWS(MaterialParser::parseMaterialFile("srcs/tester/ressources/materialWronSymbol.mtl"));
    }

    MaterialData materialData;
    SUBCASE("testing the definition of a name")
    {
        materialData.reset();
        CHECK_NOTHROW(MaterialParser::defineName(materialData, "newmtl name ", 0));
        CHECK_THROWS(MaterialParser::defineName(materialData, "newmtl ", 0));
        CHECK_THROWS(MaterialParser::defineName(materialData, "newmtl name1 name2", 0));
    }
    SUBCASE("testing the definition of ambiant color")
    {
        materialData.reset();
        CHECK_NOTHROW(MaterialParser::defineAmbiantColor(materialData, "Ka 0.5 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineAmbiantColor(materialData, "Ka 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineAmbiantColor(materialData, "Ka 0.5 0.5 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineAmbiantColor(materialData, "Ka 1.1 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineAmbiantColor(materialData, "Ka -0.5 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineAmbiantColor(materialData, "Ka .5 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineAmbiantColor(materialData, "Ka 0.5a 0.5 0.5", 0));
    }
    SUBCASE("testing the definition of specular color")
    {
        materialData.reset();
        CHECK_NOTHROW(MaterialParser::defineSpecularColor(materialData, "Ks 0.5 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineSpecularColor(materialData, "Ks 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineSpecularColor(materialData, "Ks 0.5 0.5 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineSpecularColor(materialData, "Ks 1.1 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineSpecularColor(materialData, "Ks -0.5 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineSpecularColor(materialData, "Ks .5 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineSpecularColor(materialData, "Ks 0.5a 0.5 0.5", 0));
    }
    SUBCASE("testing the definition of diffuse color")
    {
        materialData.reset();
        CHECK_NOTHROW(MaterialParser::defineDiffuseColor(materialData, "Kd 0.5 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineDiffuseColor(materialData, "Kd 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineDiffuseColor(materialData, "Kd 0.5 0.5 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineDiffuseColor(materialData, "Kd 1.1 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineDiffuseColor(materialData, "Kd -0.5 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineDiffuseColor(materialData, "Kd .5 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineDiffuseColor(materialData, "Kd 0.5a 0.5 0.5", 0));
    }
    SUBCASE("testing the definition of specular exponent")
    {
        materialData.reset();
        CHECK_NOTHROW(MaterialParser::defineSpecularExponent(materialData, "Ns 500.0", 0));
        CHECK_THROWS(MaterialParser::defineSpecularExponent(materialData, "Ns", 0));
        CHECK_THROWS(MaterialParser::defineSpecularExponent(materialData, "Ns 500 500", 0));
        CHECK_THROWS(MaterialParser::defineSpecularExponent(materialData, "Ns -10", 0));
        CHECK_THROWS(MaterialParser::defineSpecularExponent(materialData, "Ns 1500", 0));
        CHECK_THROWS(MaterialParser::defineSpecularExponent(materialData, "Ns .1", 0));
        CHECK_THROWS(MaterialParser::defineSpecularExponent(materialData, "Ns 0.1a", 0));
        CHECK_THROWS(MaterialParser::defineSpecularExponent(materialData, "Ns 0..1", 0));
    }
    SUBCASE("testing the definition of refraction index")
    {
        materialData.reset();
        CHECK_NOTHROW(MaterialParser::defineRefractionIndex(materialData, "Ni 5.0", 0));
        CHECK_THROWS(MaterialParser::defineRefractionIndex(materialData, "Ni", 0));
        CHECK_THROWS(MaterialParser::defineRefractionIndex(materialData, "Ni 5 5", 0));
        CHECK_THROWS(MaterialParser::defineRefractionIndex(materialData, "Ni -10", 0));
        CHECK_THROWS(MaterialParser::defineRefractionIndex(materialData, "Ni 15", 0));
        CHECK_THROWS(MaterialParser::defineRefractionIndex(materialData, "Ni .1", 0));
        CHECK_THROWS(MaterialParser::defineRefractionIndex(materialData, "Ni 0.1a", 0));
        CHECK_THROWS(MaterialParser::defineRefractionIndex(materialData, "Ni 0..1", 0));
    }
    SUBCASE("testing the definition of opacity")
    {
        materialData.reset();
        CHECK_NOTHROW(MaterialParser::defineOpacity(materialData, "d 0.5", 0));
        CHECK_THROWS(MaterialParser::defineOpacity(materialData, "d", 0));
        CHECK_THROWS(MaterialParser::defineOpacity(materialData, "d 0.5 0.5", 0));
        CHECK_THROWS(MaterialParser::defineOpacity(materialData, "d -10", 0));
        CHECK_THROWS(MaterialParser::defineOpacity(materialData, "d 1.1", 0));
        CHECK_THROWS(MaterialParser::defineOpacity(materialData, "d .1", 0));
        CHECK_THROWS(MaterialParser::defineOpacity(materialData, "d 0.1a", 0));
        CHECK_THROWS(MaterialParser::defineOpacity(materialData, "d 0..1", 0));
    }
    SUBCASE("testing the definition of illum")
    {
        materialData.reset();
        CHECK_NOTHROW(MaterialParser::defineIllum(materialData, "illum 1", 0));
        CHECK_THROWS(MaterialParser::defineIllum(materialData, "illum", 0));
        CHECK_THROWS(MaterialParser::defineIllum(materialData, "illum 1 1", 0));
        CHECK_THROWS(MaterialParser::defineIllum(materialData, "illum -10", 0));
        CHECK_THROWS(MaterialParser::defineIllum(materialData, "illum 11", 0));
        CHECK_THROWS(MaterialParser::defineIllum(materialData, "illum 0.1", 0));
        CHECK_THROWS(MaterialParser::defineIllum(materialData, "illum 1a", 0));
    }
}