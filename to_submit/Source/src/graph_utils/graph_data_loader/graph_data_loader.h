#pragma once
#include <config.h>
#include <graph_data.h>
#include <input_manager/input_manager.h>

struct GraphsDataLoader
{
    GraphData loadGraphFromFile(std::string file_path);
    void loadGraphsData(std::vector<GraphData>& graphs_data);
    void loadGraphsFromFileData(std::vector<GraphData>& graphs_data, std::string input_path, int& id_offset);
    std::vector<GraphData> loadGraphsFromFile(std::string file_path);
private:
    InputManager input_manager;

    void loadGraphData(InputManager& input_manager, GraphData& graph_data);
};
