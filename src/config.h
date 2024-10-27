#include <chrono>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cstring>
#include <assert.h>
#ifdef _WIN32
#include <Windows.h>
#endif
#undef min
#undef max

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

#define MODELS_PATH INPUT_PATH "input/"

#define PROJECT_ERROR(message)																						  \
    {																									      \
        std::cerr << "Error at line " << __LINE__ << " in file " << MODELS_PATH << __FILE__ << ": " << message << std::endl; \
		assert(0);																					  \
    }