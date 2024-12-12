#pragma once
#include <config.h>

struct GraphData
{
#pragma region general
public:
    std::vector<std::vector<int>> adjacency_matrix_representation;
    std::unordered_map<int, std::unordered_set<int>> out_edges_by_node;
    std::unordered_map<int, std::unordered_set<int>> in_edges_by_node;

    int getNodesCount()
    {
        if (!initialized)
            throw new std::runtime_error("GraphData not initialized.");
        return nodes_count;
    }

    void setNodesCount(int new_nodes_count)
    {
        if (new_nodes_count < 0)
            throw new std::runtime_error("Invalid nodes count number.");
        nodes_count = new_nodes_count;
    }

    //std::vector<bool>::reference operator ()(size_t row_idx, size_t col_idx)
    //{
    //    if (!initialized) 
    //        throw new std::runtime_error("GraphData not initialized.");
    //    if (col_idx >= relation_matrix.size()) 
    //        throw std::out_of_range("Column index out of range.");
    //    if (row_idx >= relation_matrix.size())
    //        throw std::out_of_range("Row index out of range.");

    //    return relation_matrix[row_idx][col_idx];
    //}

    int getId()
    {
        if (!initialized)
            throw new std::runtime_error("Graph not initialized");
        return id;
    }

    void setInitialize(bool initializeState) 
    {
        initialized = initializeState;
    }

private:
    bool initialized = false;
    int nodes_count;
    int id = -1;
    //std::vector<std::vector<bool>> relation_matrix; // TO CONSIDER: remove it and leave only out/in_edges_by_node
#pragma endregion

#pragma region longestCycles
public:
    void assignLongestCycles(std::vector<std::vector<int>>& longest_cycles)
    {
        if (longest_cycles_assigned)
            throw new std::runtime_error("Longest cycles already assigned.");

        longest_cycles_assigned = true;
        this->longest_cycles = longest_cycles;
    }
    const std::vector<std::vector<int>>& getLongestCycles()
    {
        if (!longest_cycles_assigned)
            throw new std::runtime_error("Assign longest cycles first.");

        return longest_cycles;
    }
    bool isLongestCyclesAssigned()
    {
        return longest_cycles_assigned;
    }
    void assignApproximateLongestCycles(std::vector<std::vector<int>>& approximate_longest_cycles)
    {
        if (approximate_longest_cycles_assigned)
            throw new std::runtime_error("Longest cycles already assigned.");
        approximate_longest_cycles_assigned = true;
        this->approximate_longest_cycles = approximate_longest_cycles;
    }
    const std::vector<std::vector<int>>& getApproximateLongestCycles()
    {
        if (!approximate_longest_cycles_assigned)
            throw new std::runtime_error("Assign longest cycles first.");
        return approximate_longest_cycles;
    }
    void assignLongestCyclesTime(std::chrono::milliseconds& longest_cycles_time)
    {
        this->longest_cycles_time = longest_cycles_time;
    }
    std::chrono::milliseconds getLongestCyclesTime()
    {
        return longest_cycles_time;
    }
    void assignApproximateLongestCyclesTime(std::chrono::milliseconds& approximate_longest_cycles_time)
    {
        this->approximate_longest_cycles_time = approximate_longest_cycles_time;
    }
    std::chrono::milliseconds getApproximateLongestCyclesTime()
    {
        return approximate_longest_cycles_time;
    }
private:
    bool longest_cycles_assigned = false;
    bool approximate_longest_cycles_assigned = false;

    std::vector<std::vector<int>> longest_cycles;
    std::vector<std::vector<int>> approximate_longest_cycles;
    std::chrono::milliseconds longest_cycles_time;
    std::chrono::milliseconds approximate_longest_cycles_time;

#pragma endregion

#pragma region hamiltonCycle
public:
    void assignHamiltonCyclesAndExtensions(std::vector<std::pair<int, int>>& smallest_extension, int& cycle_count)
    {
        if (precise_hamilton_cycles_extensions_assigned)
            throw new std::runtime_error("Precise Hamilton cycles and extensions already assigned.");

        precise_hamilton_cycles_extensions_assigned = true;
        this->precise_hamilton_cycle_graph_extention = smallest_extension;
        this->precise_hamilton_cycle_count = cycle_count;
    }
    const std::vector<std::pair<int, int>> getPreciseHamiltonCycleGraphExtension()
    {
        if (!precise_hamilton_cycles_extensions_assigned)
            throw new std::runtime_error("Assign Hamilton cycle extension first.");

        return precise_hamilton_cycle_graph_extention;
    }
    const int getPreciseHamiltonCycleCount()
    {
        if (!precise_hamilton_cycles_extensions_assigned)
            throw new std::runtime_error("Assign Hamilton cycles count first.");

        return precise_hamilton_cycle_count;
    }
    bool isPreciseHamiltonCycleGraphExtentionAssigned()
    {
        return precise_hamilton_cycles_extensions_assigned;
    }
    void AssignPreciseHamiltonCycleTime(std::chrono::milliseconds& precise_hamilton_cycle_time)
    {
        this->precise_hamilton_cycle_time = precise_hamilton_cycle_time;
    }
    std::chrono::milliseconds getPreciseHamiltonCycleTime()
    {
        return precise_hamilton_cycle_time;
    }
private:
    bool precise_hamilton_cycles_extensions_assigned = false;
    std::vector<std::pair<int,int>> precise_hamilton_cycle_graph_extention;
    int precise_hamilton_cycle_count;
    std::chrono::milliseconds precise_hamilton_cycle_time;
#pragma endregion

#pragma region hamiltonCycleApproximation
public:
    void setHamiltonCycleGraphExtention(graph_extention_t& graph_extention)
    {
        hamilton_cycle_graph_extention_assigned = true;
        hamilton_cycle_graph_extention = graph_extention;

        hamilton_cycles.clear();
    }
    int getHamiltonCycleGraphExtentionSize()
    {
        if (hamilton_cycle_graph_extention_assigned)
            return hamilton_cycle_graph_extention.size();
        return -1;
    }
    const graph_extention_t& getHamiltonCycleGraphExtention()
    {
        if (!hamilton_cycle_graph_extention_assigned)
            throw new std::runtime_error("Longest cycles already assigned.");

        return hamilton_cycle_graph_extention;
    }
    void addHamiltonCycle(path_t& cycle)
    {
        hamilton_cycles.insert(cycle);
    }
    const std::set<path_t>& getHamiltonCycles()
    {
        return hamilton_cycles;
    }
    bool isHamiltonCycleGraphExtentionAssigned()
    {
        return hamilton_cycle_graph_extention_assigned;
    }
    int findMinimumExtentionForHamiltonCycleExecutionTimeMs = -1;
    int findAllHamiltonCyclesExecutionTimeMs = -1;

private:
    bool hamilton_cycle_graph_extention_assigned = false;
    graph_extention_t hamilton_cycle_graph_extention;
    std::set<path_t> hamilton_cycles;
#pragma endregion


    friend struct GraphsDataLoader;
};
