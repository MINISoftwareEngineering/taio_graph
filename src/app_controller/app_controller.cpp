#include "app_controller.h"


void AppController::run()
{
    console_manager.clear();
    if (input_manager.inputFileExists())
	{
		console_manager.write("Input file detected\n");
	}
	else
	{
		console_manager.write("Create " DATA_PATH " file\n");
	}

    console_manager.write("Loading input...\n");
    graph_data_loader.loadGraphsData(graphs_data);

	console_manager.listGraphsSizes(graphs_data);

	console_manager.write("Removing edges adjecent to leaf nodes...\n");
	for (int i = 0; i < graphs_data.size(); ++i)
		graph_manager.transformToGraphWithoutEdgesAdjecentToLeafNode(graphs_data[i]);

	console_manager.listGraphsSizes(graphs_data);

    console_manager.wait_for_enter();
}