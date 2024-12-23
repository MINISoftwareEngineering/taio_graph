#include "graph_data_loader.h"

void GraphsDataLoader::loadGraphData(InputManager& input_manager, GraphData& graph_data)
{
    if (graph_data.initialized)
        throw new std::runtime_error("GraphData already initialized.");

    std::string line;
    input_manager.readLine(line);
    int nodes_count = std::stoi(line);

    graph_data.initialized = true;
    graph_data.nodes_count = nodes_count;
    //graph_data.relation_matrix = std::vector(nodes_count, std::vector<bool>(nodes_count, false));
    for (int row = 0; row < nodes_count; ++row) // out_node idx
    {
        input_manager.readLine(line);
            
        std::istringstream stream(line);
        bool value;

        graph_data.adjacency_matrix_representation.push_back(std::vector<int>(nodes_count, 0));

        for (int col = 0; col < nodes_count; ++col) // in_node idx
        {
            stream >> value;
            //graph_data(row, col) = value;

            if (!value)
                continue;
            
            graph_data.out_edges_by_node[row].insert(col);
            graph_data.in_edges_by_node[col].insert(row);

            graph_data.adjacency_matrix_representation.at(row).at(col) = 1;

            // TO CONSIDER: if you know that node x has 0 out_edges it means that x is a leaf node for any in_edges. 
            // this fact later means that those in_edges are useless and you can get stop adding them in result, because they can't belong to any cycle.
            // (those edges are only useless in terms of looking for longest cycle task)
        }
    }
}

void GraphsDataLoader::loadGraphsFromFileData(std::vector<GraphData>& graphs_data, std::string input_path, int& id_offset)
{
    input_manager.openFile(input_path);

    std::string line;
    input_manager.readLine(line);

    int graphs_data_count = std::stoi(line);
    for (int i = 0; i < graphs_data_count; ++i)
    {
        GraphData graph_data;
        graph_data.id = i + id_offset;
        loadGraphData(input_manager, graph_data);
        graphs_data.push_back(graph_data);

        if (i == graphs_data_count - 1)
            continue;

        do 
        {
            input_manager.readLine(line); // get rid of empty line between graphs
        } while (!line.empty() && !std::all_of(line.begin(), line.end(), [](unsigned char c) { return std::isspace(c); }));
    }
    id_offset += graphs_data_count;

    input_manager.closeFile();
}

void GraphsDataLoader::loadGraphsData(std::vector<GraphData>& graphs_data)
{
    std::vector<std::string> input_paths;
    input_manager.loadInputPaths(input_paths);

    int id_offset = 0;
    for (const std::string& input_path : input_paths)
        loadGraphsFromFileData(graphs_data, input_path, id_offset);
}
GraphData GraphsDataLoader::loadGraphFromFile(std::string file_path)
{
    std::vector<GraphData> graphs;
    int id_offset = 0;
    loadGraphsFromFileData(graphs, file_path, id_offset);

    if (graphs.size() == 0) {
        std::cerr << "No graphs were found in provided file" << std::endl;
        exit(EXIT_FAILURE);
    }

    return graphs.at(0);
}

std::vector<GraphData> GraphsDataLoader::loadGraphsFromFile(std::string file_path)
{
    std::vector<GraphData> graphs;
    int id_offset = 0;
    loadGraphsFromFileData(graphs, file_path, id_offset);

    if (graphs.size() == 0) {
        std::cerr << "No graphs were found in provided file" << std::endl;
        exit(EXIT_FAILURE);
    }

    return graphs;
}