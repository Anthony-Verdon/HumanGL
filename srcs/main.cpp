#include "Game/Game.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
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
    if (argc < 2)
        throw(std::runtime_error("MAIN::NO_INPUT_FILE"));

    WindowManager::InitWindow("new game", WINDOW_WIDTH, WINDOW_HEIGHT);
    Game game;
    game.LoadObjects(argc, argv);
    WindowManager::StartUpdateLoop(&game);
    WindowManager::DestructWindowManager();

    return (EXIT_SUCCESS);
}