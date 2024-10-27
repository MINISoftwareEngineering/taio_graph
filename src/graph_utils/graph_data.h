#pragma once
#include <config.h>

struct GraphData
{
    // TODO fix (change to (x, y) instead of [x][y])

    std::vector<bool>& operator[](size_t index) {
        if (!initialized) {
            throw new std::runtime_error("GraphData not initialized.");
        }
        if (index >= relation_matrix.size()) {
            throw std::out_of_range("Index out of range");
        }
        return relation_matrix[index];
    }

    std::vector<bool>& operator[](size_t index) {
        if (!initialized) {
            throw new std::runtime_error("GraphData not initialized.");
        }
        if (index >= relation_matrix.size()) {
            throw std::out_of_range("Index out of range");
        }
        return relation_matrix[index];
    }

private:
    bool initialized = false;
    std::vector<std::vector<bool>> relation_matrix;

    friend class GraphDataManager;
};
