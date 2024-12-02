#include "app_controller.h"
#include "output_manger/output_manager.h"

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
	console_manager.clear();

#ifdef METRIC_TESTS
	run_metric_tests();
#endif

	// #define HAMILTON_TESTS
#ifdef HAMILTON_TESTS
	run_hamilton_tests();
#endif

	std::string line = "R";
	int index1 = 0;
	int index2 = 1;
	while (line == "R") {
		console_manager.write("Select graph indices for metric calculations\n");
		index1 = input_manager.readNumber();
		index2 = input_manager.readNumber();
		console_manager.waitForEnter();
		console_manager.writeDistanceBetweenGraphs(graphs_data, index1, index2);
		console_manager.write("enter R to calculate metric again for new indices\n");
		line = input_manager.readLineFromStdin();
	}
	console_manager.waitForEnter();

	line = "R";
	while (line == "R") {
		console_manager.write("Select graph indices for approximate metric calculations\n");
		index1 = input_manager.readNumber();
		index2 = input_manager.readNumber();
		console_manager.waitForEnter();
		console_manager.writeApproximateDistanceBetweenGraph(graphs_data, index1, index2);
		console_manager.write("enter R to calculate approximate metric again for new indices\n");
		line = input_manager.readLineFromStdin();
	}
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

	/*
	console_manager.write("Finding all longest cycles...\n");
	for (int i = 0; i < graphs_data.size(); ++i)
	{
		graph_manager.findLongestCycles(graphs_data[i]);
	}
    console_manager.waitForEnter();
	*/

	console_manager.write("Finding approximate longest cycles...\n");
	for (int i = 0; i < graphs_data.size(); ++i) {
		graph_manager.tryFindLongestCycles(graphs_data[i]);
		console_manager.write("|- graph " + std::to_string(i) + ": Approximate longest cycles found.\n");
	}
	console_manager.listGraphsLongestCycles(graphs_data);

	console_manager.waitForEnter();
}

void AppController::run_hamilton_tests()
{
	int id_offset = 0;
	graph_data_loader.loadGraphsFromFileData(hamilton_tests_graphs, HAMILTON_TESTS_INPUT_PATH, id_offset);

	std::string hamilton_tests_output = "hamilton_tests.csv";
	createCSV(hamilton_tests_output);
	std::vector<std::vector<std::string>> rows = { { "Approximate Extention", "Approximate Extention Size" , "Approximate Cycles Count", "Approximate Extention Time", "Approximate Cycles Time", "Approximate Full Time", "Graph Size", "Vertices Count", "Retry Factor"}};
	for (int retry_factor = 1; retry_factor <= 17; retry_factor += 4)
	{
		for (int i = 0; i < hamilton_tests_graphs.size(); ++i)
		{
			int time_of_approximate = 0;
			int approximate = measure_execution_time(
				time_of_approximate,
				[&]() {
					return graph_manager.tryFindMinimumExtentionForHamiltonCycleAndAllHamiltonCycles(hamilton_tests_graphs[i], retry_factor);
				}
			);

			std::stringstream extention_stream;
			for each (auto v in hamilton_tests_graphs[i].getHamiltonCycleGraphExtention())
			{
				extention_stream << v.start << "->" << v.end << ", ";
			}

			std::vector<std::string> row = {
				extention_stream.str(),
				std::to_string(hamilton_tests_graphs[i].getHamiltonCycleGraphExtentionSize()),
				std::to_string(hamilton_tests_graphs[i].getHamiltonCycles().size()),
				std::to_string(hamilton_tests_graphs[i].findMinimumExtentionForHamiltonCycleExecutionTimeMs),
				std::to_string(hamilton_tests_graphs[i].findAllHamiltonCyclesExecutionTimeMs),
				std::to_string(time_of_approximate),
				std::to_string(graph_manager.getGraphSize(hamilton_tests_graphs[i])),
				std::to_string(hamilton_tests_graphs[i].getNodesCount()),
				std::to_string(retry_factor)
			};

			rows.push_back(row);
		}
	}


	writeDataToCSV(hamilton_tests_output, rows);
}

void AppController::run_metric_tests() {
	int id_offset = 0;
	graph_data_loader.loadGraphsFromFileData(metric_tests_graphs, METRIC_TESTS_INPUT_PATH, id_offset);

	std::string metric_test_output = "metric_tests.csv";
	createCSV("metric_tests.csv");
	std::vector<std::vector<std::string>> rows = { { "Accurate Result", "Accurate Time", "Approximate Result", "Approximate Time", "Graph1 Size", "Graph 2 Size"} };
	for (int i = 0; i < metric_tests_graphs.size(); i += 2) {

		int time_of_accurate = 0;
		int accurate = measure_execution_time(
			time_of_accurate,
			[&]() {
				return graph_manager.getMetricDistance(metric_tests_graphs[i], metric_tests_graphs[i + 1]);
			}
		);


		int time_of_approximate = 0;
		int approximate = measure_execution_time(
			time_of_approximate,
			[&]() {
				return graph_manager.tryGetMetricDistance(metric_tests_graphs[i], metric_tests_graphs[i + 1]);
			}
		);

		std::vector<std::string> row = {
			std::to_string(accurate),
			std::to_string(time_of_accurate),
			std::to_string(approximate),
			std::to_string(time_of_approximate),
			std::to_string(metric_tests_graphs[i].getNodesCount()),
			std::to_string(metric_tests_graphs[i + 1].getNodesCount())
		};

		rows.push_back(row);

	}

	writeDataToCSV(metric_test_output, rows);
}