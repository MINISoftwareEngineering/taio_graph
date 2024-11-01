#include "graph_data_loader.h"

void GraphsDataLoader::loadGraphData(InputManager& input_manager, GraphData& graph_data)
{
    if (graph_data.initialized)
    {
        throw new std::runtime_error("GraphData already initialized.");
    }

    std::string line;
    input_manager.readLine(line);
    int nodes_count = std::stoi(line);

    graph_data.initialized = true;
    graph_data.nodes_count = nodes_count;
    graph_data.relation_matrix = std::vector(nodes_count, std::vector<bool>(nodes_count, false));

    for (int row = 0; row < nodes_count; ++row) // out_node idx
    {
        input_manager.readLine(line);
            
        std::istringstream stream(line);
        bool value;

        for (int col = 0; col < nodes_count; ++col) // in_node idx
        {
            stream >> value;
            graph_data(row, col) = value;

            if (value)
            {
                graph_data.out_edges_by_node[row].insert(col);
                graph_data.in_edges_by_node[col].insert(row);

                // TO CONSIDER: if you know that node x has 0 out_edges it means that x is a leaf node for any in_edges. 
                // this fact later means that those in_edges are useless and you can get stop adding them in result, because they can't belong to any cycle.
                // (those edges are only useless in terms of looking for longest cycle task)
            }
        }
    }
}

void GraphsDataLoader::loadGraphsData(std::vector<GraphData>& graphs_data)
{
    input_manager.openFile();

    std::string line;
    input_manager.readLine(line);

    int graphs_data_count = std::stoi(line);
    for (int i = 0; i < graphs_data_count; ++i)
    {
        GraphData graph_data;
        loadGraphData(input_manager, graph_data);
        graphs_data.push_back(graph_data);

        if (i != graphs_data_count - 1)
        {
            input_manager.readLine(line); // get rid of empty line between graphs
        }
    }

    input_manager.closeFile();
}