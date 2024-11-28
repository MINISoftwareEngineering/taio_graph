#pragma once
#include <config.h>
#include <graph_data.h>
#include <input_manager/input_manager.h>

struct GraphsDataLoader
{
    void loadGraphsData(std::vector<GraphData>& graphs_data);
    void loadGraphsFromFileData(std::vector<GraphData>& graphs_data, std::string input_path, int& id_offset);
private:
    InputManager input_manager;

    void loadGraphData(InputManager& input_manager, GraphData& graph_data);
};
