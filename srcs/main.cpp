#include "classes/Game/Game.hpp"
#include "classes/WindowManager/WindowManager.hpp"
#include "globals.hpp"
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
        if (argc < 2)
            throw(std::runtime_error("MAIN::NO_INPUT_FILE"));

        WindowManager::InitWindow("new game", WINDOW_WIDTH, WINDOW_HEIGHT);
        Game game;
        game.LoadObjects(argc, argv);
        WindowManager::StartUpdateLoop(&game);
        WindowManager::DestructWindowManager();

        return (EXIT_SUCCESS);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (EXIT_FAILURE);
    }
}