#pragma once

#ifdef _WIN32
#ifdef INPUT_DLL_EXPORT
#define INPUT_API __declspec(dllexport)
#else
#define INPUT_API __declspec(dllimport)
#endif
#else
#define INPUT_API
#endif

#ifndef INPUT_ALLOCATION_HEADER
#include "Alloc.h"
#else
#include INPUT_ALLOCATION_HEADER
#endif
