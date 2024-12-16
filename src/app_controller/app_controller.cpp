#include "app_controller.h"
#include "output_manger/output_manager.h"

void AppController::run(ProgramCommand command, RunData& data)
{
	switch (command.command_name) {
		case CommandName::Help:
		{
			display_help();
			break;
		}
		case CommandName::Distance: {
			distance(command);
			break;
		}
		case CommandName::Size:
		{
			size(command);
			break;
		}
		case CommandName::Hamilton: 
		{
			hamilton(command);
			break;
		}
		case CommandName::MaxCycles:
		{
			max_cycles(command);
			break;
		}
	}

#ifdef METRIC_TESTS
	run_metric_tests();
#endif

// #define HAMILTON_TESTS
#ifdef HAMILTON_TESTS
	run_hamilton_tests();
#endif
	console_manager.waitForEnterToClose();
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


std::pair<int, int> AppController::getSelectedIndices(std::vector<GraphData> graphs_data) {
	int index1;
	int index2;

	bool selectIndex1 = true;
	int indices_limit = graphs_data.size() - 1;
	while (selectIndex1) {
		index1 = input_manager.readNumber();

		if (indices_limit < index1 || index1 < 0) {
			console_manager.write("Select an index within provided range (0,"
				+ std::to_string(indices_limit) + ")\n");
		}
		else {
			selectIndex1 = false;
		}
	}

	bool selectIndex2 = true;
	while (selectIndex2) {
		index2 = input_manager.readNumber();

		if (indices_limit < index2 || index2 < 0) {
			console_manager.write("Select an index within provided range (0,"
				+ std::to_string(indices_limit) + ")\n");
		}
		else {
			selectIndex2 = false;
		}
	}

	return std::make_pair(index1, index2);
}


void AppController::display_help() 
{
	std::string program_name = "taio_graph.exe";
	std::cout << "Uzycie programu: " << program_name << " [Opcje]\n\n"
		<< "Opcje:\n"
		<< std::setw(25) << std::left << "  --help" << " wyswietl to menu\n"
		<< std::setw(25) << std::left << "  -h, --hamilton" << " approx | exact | both\n"
		<< std::setw(25) << std::left << "  -s, --size" << " exact\n"
		<< std::setw(25) << std::left << "  -c, --cycles" << " approx | exact | both\n"
		<< std::setw(25) << std::left << "  -d, --distance" << " approx | exact | both\n"
		<< std::setw(25) << std::left << "  -f, [nazwa_pliku]" << " dla opcji -h, -s, -c\n"
		<< std::setw(25) << std::left << "  -f1 [nazwa_pliku1]" << " dla opcji -d\n"
		<< std::setw(25) << std::left << "  -f2 [nazwa_pliku2]" << " dla opcji -d\n"
		<< std::setw(25) << std::left << "  -r  [1-15]" << " dla opcji -h approx | both " << " postawowo 10\n"
		<< std::setw(25) << std::left << "  -o, ---output  " << " long | short " << " podstawowo long\n\n"
		<< "Przyklady:\n"
		<< "  " << program_name << " --help\n"
		<< "  " << program_name << " -h approx -f graph1.txt\n"
		<< "  " << program_name << " -s exact -f graph1.txt\n"
		<< "  " << program_name << " -c both -f graph1.txt\n"
		<< "  " << program_name << " -d both -f1 graph1.txt -f2 graph2.txt\n";

}

void AppController::distance(ProgramCommand command) 
{
	if (!input_manager.inputFilesExist(command.files.at(0), command.files.at(1))) {
		std::cerr << "Podane pliki z danymi grafow nie zostaly znalezione" << std::endl;
		exit(EXIT_FAILURE);
	}

	GraphData graph1 = graph_data_loader.loadGraphFromFile(command.files.at(0));
	GraphData graph2 = graph_data_loader.loadGraphFromFile(command.files.at(1));

	console_manager.write("\n==========================================================\n");
	console_manager.write("\tWyznaczanie odległosci miedzy grafami\n");
	console_manager.write("==========================================================\n\n");

	if (command.long_output) {
		console_manager.write("Macierz sasiedztwa 1:\n");
		console_manager.writeGraphInConsole(graph1);
		console_manager.write("\n");

		console_manager.write("Macierz sasiedztwa 2:\n");
		console_manager.writeGraphInConsole(graph2); \
			console_manager.write("\n");
	}

	if (command.algorithm_type == AlgorithmType::Exact || command.algorithm_type == AlgorithmType::Both) {
		if (graph1.getNodesCount() <= 8 && graph2.getNodesCount() <= 8) {
			console_manager.write("Obliczanie dokladnej metryki miedzy grafami...\n");
			int distance = graph_manager.getMetricDistance(graph1, graph2);
			console_manager.write("Dokladna metryka = " + std::to_string(distance) + " \n");
		}
		else {
			console_manager.write("Obliczanie metryki dokladnej pominiete przez zbyt duzy rozmiar jednego z grafow.\n");
			console_manager.write("Rozmiary to: \n\t" +
				std::to_string(graph1.getNodesCount()) + " (wczytany z pliku " + command.files.at(0) + " )\n" +
				"\t" + std::to_string(graph2.getNodesCount()) + " (wczytany z pliku " + command.files.at(1) + " )\n");
		}
	}

	if (command.algorithm_type == AlgorithmType::Approximate || command.algorithm_type == AlgorithmType::Both)
	{
		console_manager.write("Obliczanie aproksymacji metryki miedzy grafami...\n");
		int distance = graph_manager.tryGetMetricDistance(graph1, graph2);
		console_manager.write("Aproksymacja metryki = " + std::to_string(distance) + " \n");
	}
}

void AppController::size(ProgramCommand command)
{
	if (!input_manager.inputFileExists(command.files.at(0))) {
		std::cerr << "Podany plik z danymi grafu, nie istnieje" << std::endl;
	}

	std::vector<GraphData> graphs = graph_data_loader.loadGraphsFromFile(command.files.at(0));

	for (int i = 0; i < graphs.size(); i++) {

		console_manager.write("\n==========================================================\n");
		console_manager.write("Wyznaczenie rozmiaru dla grafu nr " + std::to_string(i + 1) + "\n");
		console_manager.write("==========================================================\n\n");

		if (command.long_output) {
			console_manager.write("Macierz sasiedztwa:\n");
			console_manager.writeGraphInConsole(graphs.at(i));
			console_manager.write("\n");
		}

		console_manager.write("Obliczanie rozmiaru grafu...\n");
		int size = graph_manager.getGraphSize(graphs.at(i));
		console_manager.write("Rozmiar grafu to = " + std::to_string(size) + "\n");
	}
}

void AppController::hamilton(ProgramCommand command)
{
	if (!input_manager.inputFileExists(command.files.at(0))) {
		std::cerr << "Podany plik z danymi grafu nie zostal znaleziony" << std::endl;
		std::cerr << "Podana sciezka to: " << command.files.at(0);
		exit(EXIT_FAILURE);
	}

	std::vector<GraphData> graphs = graph_data_loader.loadGraphsFromFile(command.files.at(0));

	for (int i = 0; i < graphs.size(); i++) {

		console_manager.write("\n==========================================================\n");
		console_manager.write("Znajdowanie rozszerzenia hamiltonowskiego dla grafu nr " + std::to_string(i + 1) + "\n");
		console_manager.write("==========================================================\n\n");

		if (command.long_output) {
			if (command.algorithm_type == AlgorithmType::Approximate || command.algorithm_type == AlgorithmType::Both)	
				
			console_manager.write("Macierz sasiedztwa:\n");
			console_manager.writeGraphInConsole(graphs.at(i));
			console_manager.write("\n");
		}

		if (command.algorithm_type == AlgorithmType::Exact || command.algorithm_type == AlgorithmType::Both) {
			int nodeCount = graphs.at(i).getNodesCount();
			if (nodeCount <= 8)
			{
				console_manager.write("Dokladne wyznaczanie rozszerzenia hamiltonowskiego rozpoczete...\n");
				graph_manager.findHamiltonCycle(graphs.at(i));
				console_manager.write("Wyznaczanie dokladne skonczone\n");
				std::vector<GraphData> stub_vector = { graphs.at(i) };
				console_manager.listPreciseGraphsHamiltonCycleExtentions(stub_vector);
			}
			else
				console_manager.write("Dokladny algorytm pominiety, graf ma wiecej niz 8 wierzcholkow\n");
		}

		if (command.algorithm_type == AlgorithmType::Approximate || command.algorithm_type == AlgorithmType::Both) {
			std::string param_note = "";
			if (command.param == 10)
				param_note = " (wartosc domyslna)";

			console_manager.write("\nParametr retryFactor dla algorytmu aproksymacyjnego: " + std::to_string(command.param) + param_note + "\n\n");
			console_manager.write("Aproksymacyjne wyznaczanie rozszerzenia hamiltonowskiego rozpoczete...\n");
			if (!graph_manager.tryFindMinimumExtentionForHamiltonCycleAndAllHamiltonCycles(graphs.at(i), command.param))
				console_manager.write("Nie udalo sie znalezc aproksymacji rozszerzenia hamiltonowskiego\n");
			else
				console_manager.write("Wyznaczenie aproksymacyjne udane\n");
			console_manager.writeGraphHamiltonCycleExtention(graphs.at(i), command.long_output);
		}
	}
}

void AppController::max_cycles(ProgramCommand command) 
{
	if (!input_manager.inputFileExists(command.files.at(0))) {
		std::cerr << "Podany plik z danymi grafu nie zostal znaleziony" << std::endl;
		std::cerr << "Podana sciezka to: " << command.files.at(0);
		exit(EXIT_FAILURE);
	}

	std::vector<GraphData> graphs = graph_data_loader.loadGraphsFromFile(command.files.at(0));

	for (int i = 0; i < graphs.size(); i++) {

		console_manager.write("\n================================================\n");
		console_manager.write("Znajdowanie najwiekszych cykli dla grafu nr " + std::to_string(i + 1) + "\n");
		console_manager.write("================================================\n\n");


		if (command.long_output) {
			console_manager.write("Macierz sasiedztwa:\n");
			console_manager.writeGraphInConsole(graphs.at(i));
			console_manager.write("\n");
		}

		if (command.algorithm_type == AlgorithmType::Exact || command.algorithm_type == AlgorithmType::Both) {
			console_manager.write("Rozpoczeto dokladne znajodwanie najwiekszego cyklu...\n");
			if (graphs.at(i).getNodesCount() <= 8) {
				graph_manager.findLongestCycles(graphs.at(i));
				console_manager.write("Najdluszy cykl znaleziony.\n");
				std::vector<GraphData> stub_vector = { graphs.at(i) };
				console_manager.listLongestCycles(stub_vector);

				if (command.long_output && !graphs.at(i).getLongestCycles().empty()) {
					auto cycle = graphs.at(i).getLongestCycles().front();
					console_manager.writeCycleOnGraph(graphs.at(i), cycle);
				}
			}
			else
				console_manager.write("Dokladny algorytm pominiety, graf ma wiecej niz 8 wierzcholkow\n");
		}


		if (command.algorithm_type == AlgorithmType::Approximate || command.algorithm_type == AlgorithmType::Both)
		{
			console_manager.write("Rozpoczeto aproksymacyjne znajodwanie najwiekszego cyklu...\n");
			graph_manager.tryFindLongestCycles(graphs.at(i));
			console_manager.write("Aproksymacja najdluzszego cyklu znaleziona.\n");
			std::vector<GraphData> stub_vector = { graphs.at(i) };
			console_manager.listGraphsLongestCycles(stub_vector);

			if (command.long_output && !graphs.at(i).getApproximateLongestCycles().empty()) {
				auto cycle = graphs.at(i).getApproximateLongestCycles().front();
				console_manager.writeCycleOnGraph(graphs.at(i), cycle);
			}
		}
	}
}