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
            write("|- graph " + std::to_string(i) + " size: "
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
                int graph_size = graph_manager.getGraphSize(graph_data);

                std::string hamilton_cycles_count = std::to_string(graph_data.getHamiltonCycles().size());
                std::string full_execution_time = std::to_string(graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs + graph_data.findAllHamiltonCyclesExecutionTimeMs);
                std::string find_cycles_execution_time = std::to_string(graph_data.findAllHamiltonCyclesExecutionTimeMs);
                std::string find_extention_execution_time = std::to_string(graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs);
                std::string efficiency = "undefined";
                std::string efficiency_ext = "undefined";
                std::string efficiency_cyc = "undefined";
                if (graph_size > 0)
                {
                    efficiency = std::to_string(((float)graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs + (float)graph_data.findAllHamiltonCyclesExecutionTimeMs) / (float)graph_manager.getGraphSize(graph_data));
                    efficiency_ext = std::to_string(((float)graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs) / (float)graph_manager.getGraphSize(graph_data));
                    efficiency_cyc = std::to_string(((float)graph_data.findAllHamiltonCyclesExecutionTimeMs) / (float)graph_manager.getGraphSize(graph_data));
                }

                write("|- graph " + std::to_string(i) + ": \n");
                write("|  |- finding full solution time: " + full_execution_time + " ms  (finding minumum graph: " + find_extention_execution_time + " ms, finding hamilton cycles: " + find_cycles_execution_time + " ms) \n");
                write("|  |- efficiency: " + efficiency + ", (finding minimum graph: " + efficiency_ext + ", finding hamilton cycles: " + efficiency_cyc + ") \n");
                write("|  |- hamilton cycles: " + hamilton_cycles_count + " \n");
                write("|  |- smallest extention: \n");
                write(graph_data.getHamiltonCycleGraphExtention());
            }
            else
                write("|- graph " + std::to_string(i) + ": Finding failed \n");
        }
    }

    void writeDistanceBetweenGraphs(std::vector<GraphData> graphs, int index1, int index2)
    {
        write("Metric distance between graphs " 
            + std::to_string(graphs[index1].getId()) + " and " + std::to_string(graphs[index2].getId()) + ": "
            + std::to_string(graph_manager.getMetricDistance(graphs[index1], graphs[index2])) + " \n");
    }

    void writeApproximateDistanceBetweenGraph(std::vector<GraphData> graphs, int index1, int index2) {
        write("Approximate metric distance between graphs "
            + std::to_string(graphs[index1].getId()) + " and " + std::to_string(graphs[index2].getId()) + ": "
            + std::to_string(graph_manager.tryGetMetricDistance(graphs[index1], graphs[index2])) + " \n");
    }

private:
    GraphManager graph_manager;
};

