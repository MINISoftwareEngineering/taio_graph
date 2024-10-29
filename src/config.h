#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib> 
#include <cstdio>
#include <stdexcept>
#include <assert.h>
#include <chrono>

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

#define DATA_PATH RESOURCES_PATH "input/data.txt"

#define PROJECT_ERROR(message) \
    {																									      \
        std::cerr << "Error at line " << __LINE__ << " in file " << MODELS_PATH << __FILE__ << ": " << message << std::endl; \
		assert(0);																					  \
    }