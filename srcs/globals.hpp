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

#define ERROR_MESSAGE "error: " + std::string(__FILE__)  + " : " + std::string(__FUNCTION__)  + " : " + std::to_string(__LINE__)