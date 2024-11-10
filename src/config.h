#pragma once
#include <unordered_map>
#include <unordered_set>
#include <set>
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
#include <iomanip>

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
struct edge
{
    int in, out;

    bool operator==(const edge& other) const 
    {
        return in == other.in && out == other.out;
    }

    bool operator<(const edge& other) const 
    {
        return (in < other.in) || (in == other.in && out < other.out);
    }
};
typedef std::set<edge> graph_extention_t;


#define INPUT_FOLDER_PATH RESOURCES_PATH "input"

#define PROJECT_ERROR(message) \
    {																									      \
        std::cerr << "Error at line " << __LINE__ << " in file " << MODELS_PATH << __FILE__ << ": " << message << std::endl; \
		assert(0);																					  \
    }

// G = (V, E)
// |V| = n, |E| = m

template<typename K> // O(n) if excluded_values included
bool getRandomValue(K max_value, K& random_value, const std::unordered_set<K>& excluded_values = {})
{
    if (max_value < 0)
    {
        random_value = -1;
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    if (!excluded_values.empty())
    {
        std::vector<K> available_values;
        for (int value = 0; value <= max_value; ++value)
            if (excluded_values.find(value) == excluded_values.end())
                available_values.push_back(value);

        if (available_values.empty())
        {
            random_value = -1;
            return false;
        }

        std::uniform_int_distribution<size_t> dis(0, available_values.size() - 1);
        random_value = available_values[dis(gen)];
    }
    else
    {
        std::uniform_int_distribution<size_t> dis(0, max_value);
        random_value = dis(gen);
    }

    return true;
}

template<typename K>
bool getRandomValue(const std::unordered_set<K>& set, K& random_value, const std::unordered_set<K>& excluded_values = {})
{
    std::vector<K> available_values;
    for (const auto& value : set)
        if (excluded_values.find(value) == excluded_values.end())
            available_values.push_back(value);

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
bool getRandomKey(const std::unordered_map<K, V>& map, K& random_key, const std::unordered_set<K>& excluded_keys = {})
{
    std::vector<K> available_keys;
    for (const auto& pair : map)
        if (excluded_keys.find(pair.first) == excluded_keys.end())
            available_keys.push_back(pair.first);

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