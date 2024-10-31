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
        write("| Graph sizes: \n");
        for (int i = 0; i < graphs_data.size(); ++i)
            write("|- graph " + std::to_string(i) + " size: "
                + std::to_string(graph_manager.getGraphSize(graphs_data[i])) + "\n");
    }

private:
    GraphManager graph_manager;
};