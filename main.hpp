#pragma once
#define RAPIDJSON_USE_STD_STRING true
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <inttypes.h>
#include <iostream>
#include <string>

#ifdef DEBUG

#define debug(str) std::wcout << str << std::endl

#else

#define debug(str)

#endif

#define exept(str) throw Exeption(str, __FILE__, __LINE__)
