#pragma once
#include <config.h>
#include <input_manager/input_manager.h>
#include <console_manager/console_manager.h>
#include <graph_data_loader/graph_data_loader.h>
#include <graph_manager/graph_manager.h>
#include <program_commanmd.h>

struct RunData
{
    struct
    {
        struct
        {
            int retry_factor = 1;
        } approx;
    } hamilton;
};

struct AppController
{
    void run(ProgramCommand command, RunData& data);
    void run_metric_tests();
    void run_hamilton_tests();
    void display_help();
    void run_comparison_hamilton_tests();

private:
    InputManager input_manager;
    ConsoleManager console_manager;
    GraphManager graph_manager;
    GraphsDataLoader graph_data_loader;
    std::vector<GraphData> graphs_data;
    std::vector<GraphData> metric_tests_graphs;
    std::vector<GraphData> hamilton_tests_graphs;
    std::pair<int, int> getSelectedIndices(std::vector<GraphData> graphs_data);
};
