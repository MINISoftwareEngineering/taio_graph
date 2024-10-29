#pragma once
#include <config.h>
#include <input_manager/input_manager.h>
#include <console_manager/console_manager.h>
#include <graph_data_manager/graph_data_manager.h>

struct AppController
{
    void run();

private:
    InputManager input_manager;
    ConsoleManager console_manager;
    GraphDataManager graph_data_manager;
    std::vector<GraphData> graphs_data;
};
