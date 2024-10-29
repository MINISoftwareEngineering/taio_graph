#pragma once
#include <config.h>

struct GraphData
{
    std::vector<bool>::reference operator ()(size_t row_idx, size_t col_idx)
    {
        if (!initialized) 
            throw new std::runtime_error("GraphData not initialized.");
        if (col_idx >= relation_matrix.size()) 
            throw std::out_of_range("Column index out of range.");
        if (row_idx >= relation_matrix.size())
            throw std::out_of_range("Row index out of range.");

        return relation_matrix[row_idx][col_idx];
    }

private:
    bool initialized = false;
    std::vector<std::vector<bool>> relation_matrix;

    friend class GraphDataManager;
};
