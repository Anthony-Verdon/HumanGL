#include "classes/ObjectClasses/Object/Object.hpp"
#include "classes/ObjectClasses/ObjectParser/ObjectParser.hpp"
#include "classes/WindowManagement/WindowManagement.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

/**
 * main function.
 * init GLFW,
 * create the shader program,
 * and start the update loop
 */

int main(int argc, char **argv)
{
    try
    {
        if (argc != 2)
            throw(std::runtime_error("MAIN::NO_INPUT_FILE"));

        std::vector<Object> objects = ObjectParser().parseObjectFile(argv[1]);
        WindowManagement instance(objects);

        return (EXIT_SUCCESS);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (EXIT_FAILURE);
    }
}