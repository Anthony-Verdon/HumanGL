#pragma once

//#define FULL_SCREEN
// window
#ifdef FULL_SCREEN
constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;
#else
constexpr int WINDOW_WIDTH = 1080;
constexpr int WINDOW_HEIGHT = 720;
#endif