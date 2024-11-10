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

	console_manager.write("Finding minimum extentions with retry factor=3...\n");
	auto start = boost::chrono::high_resolution_clock::now();
	for (int i = 0; i < graphs_data.size(); ++i)
		if (!graph_manager.tryFindMinimumExtentionForHamiltonCycle(graphs_data[i], 5, true))
		{
			console_manager.write("|- Finding failed for graph " + std::to_string(i) + "! \n");
		}
	auto end = boost::chrono::high_resolution_clock::now();
	boost::chrono::duration<double> duration = end - start;
	std::cout << "Elapsed time: " << duration.count() << " seconds." << std::endl;

	console_manager.listGraphsHamiltonCycleExtentions(graphs_data);
	console_manager.waitForEnter();

	console_manager.listGraphsSizes(graphs_data);
	console_manager.waitForEnter();

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