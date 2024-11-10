#pragma once
#include <config.h>
#include <graph_data.h>
#include <graph_manager/graph_manager.h>

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

    void waitForEnter()
    {
        std::cout << "[Press Enter to continue]\n";
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
            std::string hamilton_cycles_count = std::to_string(graph_data.getHamiltonCycles().size());

            write("|- graph " + std::to_string(i + 1) + ": { hamilton cycles: " + hamilton_cycles_count + ", extention: ");
            std::cout << graph_data.getHamiltonCycleGraphExtention();
            write(" } \n");
        }
    }

private:
    GraphManager graph_manager;
};