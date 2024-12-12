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

    void listPreciseGraphsHamiltonCycleExtentions(std::vector<GraphData>& graphs_data)
    {
        write("Precise Graphs Hamilton Cycle extentions: \n");
        for (int i = 0; i < graphs_data.size(); ++i)
        {
            GraphData& graph_data = graphs_data[i];

            if (graphs_data[i].getNodesCount() > 8)
                write("|- graph " + std::to_string(i) + ": node count > 8 - omitted \n");
            else
            {
                if (graph_data.isPreciseHamiltonCycleGraphExtentionAssigned())
                {
                    int graph_size = graph_manager.getGraphSize(graph_data);

                    std::string precise_hamilton_cycles_count = std::to_string(graph_data.getPreciseHamiltonCycleCount());
                    //std::string full_execution_time = std::to_string(graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs + graph_data.findAllHamiltonCyclesExecutionTimeMs);
                    //std::string find_cycles_execution_time = std::to_string(graph_data.findAllHamiltonCyclesExecutionTimeMs);
                    //std::string find_extention_execution_time = std::to_string(graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs);
                    std::chrono::milliseconds duration = graph_data.getPreciseHamiltonCycleTime();

                    write("|- graph " + std::to_string(i) + ": \n");
                    //write("|  |- finding full solution time: " + full_execution_time + " ms  (finding minumum graph: " + find_extention_execution_time + " ms, finding hamilton cycles: " + find_cycles_execution_time + " ms) \n");
                    write("|  |- execution time: " + std::to_string(duration.count()) + "ms\n");
                    write("|  |- hamilton cycles: " + precise_hamilton_cycles_count + " \n");
                    write("|  |- smallest extention (edges count: " + std::to_string(graph_data.getPreciseHamiltonCycleGraphExtension().size()) + "): \n|  |- [");
                    
                    std::vector<std::pair<int, int>> precise_hamilton_cycle_graph_extention = graph_data.getPreciseHamiltonCycleGraphExtension();
                    for (int j = 0; j < precise_hamilton_cycle_graph_extention.size(); j++)
                    {
                        std::pair<int, int> current_edge = precise_hamilton_cycle_graph_extention[j];
                        write(std::to_string(current_edge.first) + " -> " + std::to_string(current_edge.second));
                        if (j < precise_hamilton_cycle_graph_extention.size() - 1)
                            write(", ");
                    }
                    write("]\n");
                }
                else
                    write("|- graph " + std::to_string(i) + ": Finding failed \n");
            }
            
        }
    }

    void listGraphsHamiltonCycleExtentions(std::vector<GraphData>& graphs_data)
    {
        write("Approximate Graphs Hamilton Cycle extentions: \n");
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
                std::string note = "";
                if (graph_data.getHamiltonCycleGraphExtentionSize() > 0)
                    note = " (for graph combined with smallest extention)";

                write("|- graph " + std::to_string(i) + ": \n");
                write("|  |- finding full solution time: " + full_execution_time + " ms  (finding minumum graph: " + find_extention_execution_time + " ms, finding hamilton cycles: " + find_cycles_execution_time + " ms) \n");
                write("|  |- hamilton cycles" + note + ": " + hamilton_cycles_count + " \n");
                write("|  |- smallest extention (edges count: " + std::to_string(graph_data.getHamiltonCycleGraphExtentionSize()) + "): \n");
                write(graph_data.getHamiltonCycleGraphExtention());
            }
            else
                write("|- graph " + std::to_string(i) + ": Finding failed \n");
        }
    }


    void listLongestCycles(std::vector<GraphData>& graphs_data)
    {
        write("Longest cycles: \n");
        for (int i = 0; i < graphs_data.size(); ++i)
        {
            GraphData& graph_data = graphs_data[i];

            if (graphs_data[i].getNodesCount() > 8)
                write("|- graph " + std::to_string(i) + ": node count > 8 - omitted \n");
            else
            {
                if (graph_data.isLongestCyclesAssigned())
                {
                    std::chrono::milliseconds duration = graph_data.getLongestCyclesTime();
                    write("|- graph " + std::to_string(i) + ": \n");
                    write("|  |- execution time: " + std::to_string(duration.count()) + "ms\n");
                    std::vector<std::vector<int>> longest_cycles = graph_data.getLongestCycles();
                    if (longest_cycles.size() == 0)
                    {
                        write("|  |- No cycles found!\n");
                    }
                    else
                    {
                        std::vector<int> first_longest_cycle = longest_cycles[0];
                        write("|  |- longest cycle length: " + std::to_string(first_longest_cycle.size() - 1) + " (" + std::to_string(longest_cycles.size()) + " cycles)\n");
                        write("|  |- first result:");
                        printCycle(first_longest_cycle);
                        write("\n");
                    }

                    //std::string full_execution_time = std::to_string(graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs + graph_data.findAllHamiltonCyclesExecutionTimeMs);
                    //std::string find_cycles_execution_time = std::to_string(graph_data.findAllHamiltonCyclesExecutionTimeMs);
                    //std::string find_extention_execution_time = std::to_string(graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs);
                    //write("|  |- finding full solution time: " + full_execution_time + " ms  (finding minumum graph: " + find_extention_execution_time + " ms, finding hamilton cycles: " + find_cycles_execution_time + " ms) \n");
                    
                }
                else
                    write("|- graph " + std::to_string(i) + ": Finding failed \n");
            }
        }
    }

    void listGraphsLongestCycles(std::vector<GraphData>& graphs_data)
    {
        write("Approximate Graphs Longest Cycles: \n");
        for (int i = 0; i < graphs_data.size(); ++i) {
            GraphData& graph_data = graphs_data[i];
            int graph_size = graph_manager.getGraphSize(graph_data);
            std::chrono::milliseconds duration = graph_data.getApproximateLongestCyclesTime();
            int approximate_longest_cycles_count = graph_data.getApproximateLongestCycles().size();
            std::vector<std::vector<int>> approximate_longest_cycles_values = graph_data.getApproximateLongestCycles();
            write("|-      graph " + std::to_string(i) + "    Size: " + std::to_string(graph_size) + ": \n");
            write("|- execution time: " + std::to_string(duration.count()) + "ms\n");
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
        write(" -> " + std::to_string(cycle[0]));

        write("\n");
    }


    void writeDistanceBetweenGraphs(std::vector<GraphData> graphs, int index1, int index2)
    {
        write("Calculating accurate metric between graph "
            + std::to_string(index1) + "(" + std::to_string(graphs.at(index1).getNodesCount()) + ")" + " and graph "
            + std::to_string(index2) + "(" + std::to_string(graphs.at(index2).getNodesCount()) + ")\n");

        write("Finished calcualting metric distance between graphs "
            + std::to_string(graphs[index1].getId()) + " and " + std::to_string(graphs[index2].getId())
            + ".\nThe result is "
            + std::to_string(graph_manager.getMetricDistance(graphs[index1], graphs[index2])) + " \n");

    }


    void writeApproximateDistanceBetweenGraph(std::vector<GraphData> graphs, int index1, int index2) {
        write("Calculating approximate metric between graph "
            + std::to_string(index1) + "(" + std::to_string(graphs.at(index1).getNodesCount()) + ")" + " and graph "
            + std::to_string(index2) + "(" + std::to_string(graphs.at(index2).getNodesCount()) + ")\n");

        write("Approximate metric distance between graphs "
            + std::to_string(graphs[index1].getId()) + " and " + std::to_string(graphs[index2].getId()) 
            +".\nThe result is "
            + std::to_string(graph_manager.tryGetMetricDistance(graphs[index1], graphs[index2])) + " \n");
    }

    void writeGraphsIndiciesWithSizes(std::vector<GraphData> graphs) {

        write("List of available graphs loaded from data files, the output format is: index(size)\n");
        int index = 0;
        for (auto& graph : graphs) {
            write(std::to_string(index++) + "(" + std::to_string(graph.getNodesCount()) + ") ");
        }
        write("\n");
    }


private:
    GraphManager graph_manager;
};

