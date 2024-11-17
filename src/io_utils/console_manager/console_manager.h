#pragma once
#include <config.h>
#include <graph_manager/graph_manager.h>
#include <limits>
#include <future>
#include <thread>
#include <chrono>

inline std::ostream& operator<<(std::ostream& os, const graph_extention_t& extention)
{
    if (extention.empty())
    {
        os << "|  |   [] \n";
        return os;
    }

    int i = 0;
    os << "|  |   [ ";
    for each (edge e in extention)
    {
        if (i % 6 == 0)
            os << "\n|  |     ";
        os << e.start << " -> " << e.end;
        if (i++ != extention.size() - 1)
            os << ", ";
    }
    os << "\n|  |   ] \n";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const path_t& path)
{
    os << "[ ";
    for (size_t i = 0; i < path.size(); ++i)
    {
        os << path[i];
        if (i != path.size() - 1)
            os << " -> ";
    }
    os << " ]";
    return os;
}

struct ConsoleManager
{
    void clear()
    {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void write(std::string text)
    {
        std::cout << text;
    }

    void write(const graph_extention_t& graph_extention)
    {
        std::cout << graph_extention;
    }

    void waitForEnter()
    {
            std::cout << "[Press Enter to continue] \n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clear();
    }

    void listGraphsSizes(std::vector<GraphData>& graphs_data)
    {
        write("Graphs sizes: \n");
        for (int i = 0; i < graphs_data.size(); ++i)
            write("|- graph " + std::to_string(i + 1) + " size: "
                + std::to_string(graph_manager.getGraphSize(graphs_data[i])) + "\n");
    }

    void listGraphsHamiltonCycleExtentions(std::vector<GraphData>& graphs_data)
    {
        write("Graphs Hamilton Cycle extentions: \n");
        for (int i = 0; i < graphs_data.size(); ++i)
        {
            GraphData& graph_data = graphs_data[i];

            if (graph_data.isHamiltonCycleGraphExtentionAssigned())
            {
                std::string hamilton_cycles_count = std::to_string(graph_data.getHamiltonCycles().size());

                write("|- graph " + std::to_string(i + 1) + ": \n");
                write("|  |- hamilton cycles: " + hamilton_cycles_count + " \n");
                write("|  |- smallest extention: \n");
                write(graph_data.getHamiltonCycleGraphExtention());
            }
            else
                write("|- graph " + std::to_string(i + 1) + ": Finding failed \n");
        }
    }

    void writeDistanceBetweenGraphs(GraphData& graph_data_1, GraphData& graph_data_2)
    {
        write("Metric distance between graphs " 
            + std::to_string(graph_data_1.getId()) + " and " + std::to_string(graph_data_2.getId()) + ": " 
            + std::to_string(graph_manager.getExactMetricDistance(graph_data_1, graph_data_2)) + " \n");
    }

private:
    GraphManager graph_manager;
};

