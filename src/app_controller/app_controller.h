#pragma once
#include <config.h>
#include <input_manager/input_manager.h>
#include <console_manager/console_manager.h>
#include <graph_data_loader/graph_data_loader.h>
#include <graph_manager/graph_manager.h>

struct AppController
{
    void run();

private:
    InputManager input_manager;
    ConsoleManager console_manager;
    GraphManager graph_manager;
    GraphsDataLoader graph_data_loader;
    std::vector<GraphData> graphs_data;
};
