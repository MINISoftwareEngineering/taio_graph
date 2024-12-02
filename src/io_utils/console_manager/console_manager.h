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

                write("|- graph " + std::to_string(i) + ": \n");
                write("|  |- finding full solution time: " + full_execution_time + " ms  (finding minumum graph: " + find_extention_execution_time + " ms, finding hamilton cycles: " + find_cycles_execution_time + " ms) \n");
                write("|  |- hamilton cycles: " + hamilton_cycles_count + " \n");
                write("|  |- smallest extention: \n");
                write(graph_data.getHamiltonCycleGraphExtention());
            }
            else
                write("|- graph " + std::to_string(i) + ": Finding failed \n");
        }
    }

    void listGraphsLongestCycles(std::vector<GraphData>& graphs_data)
    {
        write("Graphs Longest Cycles: \n");

        for (int i = 0; i < graphs_data.size(); ++i) {
            GraphData& graph_data = graphs_data[i];
            int graph_size = graph_manager.getGraphSize(graph_data);

            /*
            int longest_cycles_count = graph_data.getLongestCycles().size();
            std::vector<std::vector<int>> longest_cycles_values = graph_data.getLongestCycles();

            
            if (longest_cycles_count > 0)
            {
                write("|- exact longest cycles (length " + std::to_string(longest_cycles_values[0].size()) + ") : \n");
                for (int j = 0; j < longest_cycles_count; j++)
                {
                    write("|- cycle " + std::to_string(i) + ": \n");
                    printCycle(longest_cycles_values[j]);
                }
            }
            else
            {
                write("|- exact longest cycles: ");
                write("  No cycles found!\n");
            }
            */

            int approximate_longest_cycles_count = graph_data.getApproximateLongestCycles().size();
            std::vector<std::vector<int>> approximate_longest_cycles_values = graph_data.getApproximateLongestCycles();

            write("|-      graph " + std::to_string(i) + "    Size: " + std::to_string(graph_size) + ": \n");
            

            if (approximate_longest_cycles_count > 0)
            {
                write("|- approximate longest cycles (length " + std::to_string(approximate_longest_cycles_values[0].size()) + ") : \n");
                for (int j = 0; j < approximate_longest_cycles_count; j++)
                {
                    write("|- cycle " + std::to_string(j) + ": \n");
                    printCycle(approximate_longest_cycles_values[j]);
                }
            }
            else
            {
                write("|- approximate longest cycles: ");
                write("  No approximate cycles found!\n");
            }

        }
    }

    void printCycle(const std::vector<int>& cycle)
    {
        if (cycle.empty()) {
            write("The cycle is empty.\n");
            return;
        }

        write(std::to_string(cycle[0]));

        for (int i = 1; i < cycle.size(); i++) {
            write(" -> " + std::to_string(cycle[i]));
        }

        write("\n");
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

