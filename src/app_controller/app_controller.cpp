#include "app_controller.h"

void AppController::run(RunData& data)
{
    console_manager.clear();
    if (input_manager.inputFileExists())
		console_manager.write("Input files detected\n");
	else
	{
		console_manager.write("Create " INPUT_FOLDER_PATH " files\n");
		console_manager.waitForEnter();
	}

    console_manager.write("Loading input...\n");
    graph_data_loader.loadGraphsData(graphs_data);

	console_manager.writeDistanceBetweenGraphs(graphs_data[0], graphs_data[1]);
	console_manager.waitForEnter();

	console_manager.write("Finding minimum extentions with retry factor=" + std::to_string(data.hamilton.approx.retry_factor) + "...\n");
	for (int i = 0; i < graphs_data.size(); ++i)
		if (!graph_manager.tryFindMinimumExtentionForHamiltonCycleAndAllHamiltonCycles(graphs_data[i], data.hamilton.approx.retry_factor))
			console_manager.write("|- graph " + std::to_string(i) + ": Finding failed! \n");
		else
			console_manager.write("|- graph " + std::to_string(i) + ": Finding finished! \n");
	console_manager.clear();

	console_manager.listGraphsHamiltonCycleExtentions(graphs_data);
	console_manager.waitForEnter();

	console_manager.listGraphsSizes(graphs_data);
	console_manager.waitForEnter();

	console_manager.write("Finding all longest cycles...\n");
	for (int i = 0; i < graphs_data.size(); ++i)
		graph_manager.findLongestCycles(graphs_data[i]);
    console_manager.waitForEnter();
}