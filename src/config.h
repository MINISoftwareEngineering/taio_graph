#include <chrono>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cstring>
#include <assert.h>
#include <fstream>
#include <string>
#include <cstdlib> 
#include <stdexcept>

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

#define DATA_PATH RESOURCES_PATH "input/data.txt"

#define PROJECT_ERROR(message) \
    {																									      \
        std::cerr << "Error at line " << __LINE__ << " in file " << MODELS_PATH << __FILE__ << ": " << message << std::endl; \
		assert(0);																					  \
    }