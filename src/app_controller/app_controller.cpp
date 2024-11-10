#include "app_controller.h"

void AppController::run()
{
    console_manager.clear();
    if (input_manager.inputFileExists())
	{
		console_manager.write("Input files detected\n");
	}
	else
	{
		console_manager.write("Create " INPUT_FOLDER_PATH " files\n");
	}

    console_manager.write("Loading input...\n");
    graph_data_loader.loadGraphsData(graphs_data);

	console_manager.write("Finding minimum extention 3 times...\n");
	for (int i = 0; i < 3; ++i)
	{
		GraphData graph_data = graphs_data[2];
		console_manager.write(std::to_string(i) + " try: \n");
		graph_manager.findMinimumExtentionForHamiltonCycle(graph_data);
	}

	console_manager.listGraphsSizes(graphs_data);

	console_manager.write("Removing edges adjecent to leaf nodes...\n");
	for (int i = 0; i < graphs_data.size(); ++i)
		graph_manager.transformToGraphWithoutEdgesAdjecentToLeafNode(graphs_data[i]);

	console_manager.listGraphsSizes(graphs_data);
	console_manager.waitForEnter();

	console_manager.write("Finding all longest cycles...\n");
	for (int i = 0; i < graphs_data.size(); ++i)
		graph_manager.findLongestCycles(graphs_data[i]);

    console_manager.waitForEnter();
}