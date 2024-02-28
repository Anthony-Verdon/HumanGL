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
}

TEST_CASE("test the definition of an object")
{
    SUBCASE("testing useful functions for parsing")
    {
        SUBCASE("testing if a string can be converted to an int")
        {
            CHECK(ObjectParser::isInt("0123456789"));
            CHECK(ObjectParser::isInt("-1"));
            CHECK_FALSE(ObjectParser::isInt("0.1"));
            CHECK_FALSE(ObjectParser::isInt("1-"));
            CHECK_FALSE(ObjectParser::isInt("-100-1"));
            CHECK_FALSE(ObjectParser::isInt("1a"));
            CHECK_FALSE(ObjectParser::isInt("a1"));
        }

        SUBCASE("testing if a string can be converted to a float")
        {
            CHECK(ObjectParser::isFloat("0123456789"));
            CHECK(ObjectParser::isFloat("0.01"));
            CHECK(ObjectParser::isFloat("-0.01"));
            CHECK_FALSE(ObjectParser::isFloat(".1"));
            CHECK_FALSE(ObjectParser::isFloat("0..1"));
            CHECK_FALSE(ObjectParser::isFloat("-.1"));
            CHECK_FALSE(ObjectParser::isFloat("a.1"));
            CHECK_FALSE(ObjectParser::isFloat("1-"));
            CHECK_FALSE(ObjectParser::isFloat("-100-1"));
            CHECK_FALSE(ObjectParser::isFloat("1a"));
            CHECK_FALSE(ObjectParser::isFloat("a1"));
        }
    }

    SUBCASE("testing definitions of object elements")
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
}

TEST_CASE("test the definition of a material")
{
    SUBCASE("testing the definition of a name")
    {
    }
    SUBCASE("testing the definition of ambiant color")
    {
    }
    SUBCASE("testing the definition of specular color")
    {
    }
    SUBCASE("testing the definition of diffuse color")
    {
    }
    SUBCASE("testing the definition of specular exponent")
    {
    }
    SUBCASE("testing the definition of refraction index")
    {
    }
    SUBCASE("testing the definition of opacity")
    {
    }
    SUBCASE("testing the definition of illum")
    {
    }
}