#pragma once
#include <config.h>

struct GraphData
{
    std::unordered_map<int, std::unordered_set<int>> out_edges_by_node;
    std::unordered_map<int, std::unordered_set<int>> in_edges_by_node;

    int getNodesCount()
    {
        if (!initialized)
            throw new std::runtime_error("GraphData not initialized.");
        return nodes_count;
    }

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
    std::vector<std::vector<bool>> relation_matrix; // TO CONSIDER: remove it and leave only out/in_edges_by_node
    int nodes_count;

    friend class GraphsDataLoader;
};
