#pragma once
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
#include <filesystem>
#include <random>
#include <iterator>

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

typedef struct { int in, out; } edge;

#define INPUT_FOLDER_PATH RESOURCES_PATH "input"

#define PROJECT_ERROR(message) \
    {																									      \
        std::cerr << "Error at line " << __LINE__ << " in file " << MODELS_PATH << __FILE__ << ": " << message << std::endl; \
		assert(0);																					  \
    }

template<typename K>
inline bool getRandomValue(const std::unordered_set<K>& set, K& random_value, const std::unordered_set<K>& excluded_values = {})
{
    std::vector<K> available_values;
    for (const auto& value : set) {
        if (excluded_values.find(value) == excluded_values.end()) {
            available_values.push_back(value);
        }
    }

    if (available_values.empty())
    {
        random_value = -1;
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, available_values.size() - 1);

    random_value = available_values[dis(gen)];
    return true;
}

template<typename K, typename V>
inline bool getRandomKey(const std::unordered_map<K, V>& map, K& random_key, const std::unordered_set<K>& excluded_keys = {})
{
    std::vector<K> available_keys;
    for (const auto& pair : map) {
        if (excluded_keys.find(pair.first) == excluded_keys.end()) {
            available_keys.push_back(pair.first);
        }
    }

    if (available_keys.empty())
    {
        random_key = -1;
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, available_keys.size() - 1);

    random_key = available_keys[dis(gen)];
    return true;
}