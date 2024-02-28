#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../srcs/classes/Utils/Utils.hpp"

// allow to use private methods only on this file
#define private public

#include "../srcs/classes/MaterialClasses/MaterialParser/MaterialParser.hpp"
#include "../srcs/classes/ObjectClasses/ObjectParser/ObjectParser.hpp"
#include "doctest.h"

// maybe test texture too
TEST_CASE("test useful functions")
{
    SUBCASE("test the splitLine function")
    {
        std::string line1 = "hi ! How are you ?";
        std::vector<std::string> lineSplit1 = {"hi", "!", "How", "are", "you", "?"};
        CHECK_MESSAGE(Utils::splitLine(line1) == lineSplit1, "line: ", line1);
        std::string line2 = "     multiples     spaces     ";
        std::vector<std::string> lineSplit2 = {"multiples", "spaces"};
        CHECK_MESSAGE(Utils::splitLine(line2) == lineSplit2, "line: ", line2);
        std::string line3 = "";
        std::vector<std::string> lineSplit3 = {};
        CHECK_MESSAGE(Utils::splitLine(line3) == lineSplit3, "line: {nothing}");
        std::string line4 = "            ";
        std::vector<std::string> lineSplit4 = {};
        CHECK_MESSAGE(Utils::splitLine(line4) == lineSplit4, "line: {only space}");
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

    SUBCASE("testing the definition of a name")
    {
        ObjectParser::objectData.reset();
        CHECK_NOTHROW(ObjectParser::defineName("o name ", 0));
        CHECK_THROWS(ObjectParser::defineName("o ", 0));
        CHECK_THROWS(ObjectParser::defineName("o name1 name2", 0));
    }

    SUBCASE("testing the definition of a vertex")
    {
        ObjectParser::objectData.reset();
        CHECK_NOTHROW(ObjectParser::defineVertex("v -0.5 0 0.5 ", 0));
        CHECK_NOTHROW(ObjectParser::defineVertex("v -0.5 0 0.5 1", 0));
        CHECK_THROWS(ObjectParser::defineVertex("v -0.5 0", 0));
        CHECK_THROWS(ObjectParser::defineVertex("v -0.5 0 0.5 1 1", 0));
        CHECK_THROWS(ObjectParser::defineVertex("v -0.5a 0 0.5", 0));
        CHECK_THROWS(ObjectParser::defineVertex("v --0.5 0 0.5", 0));
        CHECK_THROWS(ObjectParser::defineVertex("v -.5 0 0.5", 0));
        CHECK_THROWS(ObjectParser::defineVertex("v .5 0 0.5", 0));
        CHECK_THROWS(ObjectParser::defineVertex("v ..5 0 0.5", 0));
    }

    SUBCASE("testing the definition of a face")
    {
        ObjectParser::objectData.reset();
        ObjectParser::defineVertex("v -0.5 0 0.5 ", 0);
        ObjectParser::defineVertex("v -0.5 0 0.5 ", 0);
        ObjectParser::defineVertex("v -0.5 0 0.5 ", 0);
        ObjectParser::defineVertex("v -0.5 0 0.5 ", 0);
        ObjectParser::defineVertex("v -0.5 0 0.5 ", 0);
        ObjectParser::defineVertex("v -0.5 0 0.5 ", 0);
        ObjectParser::defineVertex("v -0.5 0 0.5 ", 0);
        CHECK_NOTHROW(ObjectParser::defineFace("f 1 2 3", 0));
        CHECK_NOTHROW(ObjectParser::defineFace("f -1 -2 -3", 0));
        CHECK_NOTHROW(ObjectParser::defineFace("f 1 2 3 4 5 6 7", 0));
        CHECK_THROWS(ObjectParser::defineFace("f 1 2", 0));
        CHECK_THROWS(ObjectParser::defineFace("f 1 2 1.5", 0));
        CHECK_THROWS(ObjectParser::defineFace("f 1 2 1a", 0));
        CHECK_THROWS(ObjectParser::defineFace("f 1 2 8", 0));
        CHECK_THROWS(ObjectParser::defineFace("f 1 2 -9", 0));
    }

    SUBCASE("testing the definition of smooth shading")
    {
        ObjectParser::objectData.reset();
        CHECK_NOTHROW(ObjectParser::defineSmoothShading("s on", 0));
        CHECK_NOTHROW(ObjectParser::defineSmoothShading("s off", 0));
        CHECK_NOTHROW(ObjectParser::defineSmoothShading("s 0", 0));
        CHECK_NOTHROW(ObjectParser::defineSmoothShading("s 1", 0));
        CHECK_THROWS(ObjectParser::defineSmoothShading("s", 0));
        CHECK_THROWS(ObjectParser::defineSmoothShading("s 1 1", 0));
        CHECK_THROWS(ObjectParser::defineSmoothShading("s -1", 0));
        CHECK_THROWS(ObjectParser::defineSmoothShading("s 2", 0));
        CHECK_THROWS(ObjectParser::defineSmoothShading("s iufhrng", 0));
    }

    SUBCASE("testing the usage of a mtllib")
    {
        ObjectParser::objectData.reset();

        CHECK_NOTHROW(ObjectParser::saveNewMTL("mtllib srcs_test/ressources/material.mtl", 0));
        CHECK(ObjectParser::materials.size() == 2);
        CHECK(ObjectParser::materials[0].getName() == "Material");
        CHECK(ObjectParser::materials[1].getName() == "Material2");
        CHECK_THROWS(ObjectParser::saveNewMTL("mtllib ", 0));
        CHECK_THROWS(ObjectParser::saveNewMTL("mtllib first.mtl second.mtl", 0));
    }

    SUBCASE("testing the definition of a material")
    {
        ObjectParser::objectData.reset();
        ObjectParser::saveNewMTL("mtllib srcs_test/ressources/material.mtl", 0);
        CHECK_NOTHROW(ObjectParser::defineMTL("usemtl Material", 0));
        CHECK_NOTHROW(ObjectParser::defineMTL("usemtl Material2", 0));
        CHECK_THROWS(ObjectParser::defineMTL("usemtl ", 0));
        CHECK_THROWS(ObjectParser::defineMTL("usemtl first second", 0));
        CHECK_THROWS(ObjectParser::defineMTL("usemtl dontExist ", 0));
    }
}

TEST_CASE("test the definition of a material")
{
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