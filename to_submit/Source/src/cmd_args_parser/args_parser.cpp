#include <args_parser.h>
#include <iostream>

ProgramCommand ArgsParser::parse(int argc, char** argv) {
	ProgramCommand command = ProgramCommand();

	bool mapped_command = false;
	for (int i = 1; i < argc; i += 2) {
		std::string flag = argv[i];
		CommandName command_name = map_to_command_name(flag);

		if (command_name == CommandName::Help) {
			command.command_name = command_name;
			return command;
		}

		if (i + 1 >= argc) {
			std::cerr << "Niepoprawny format parametru wywolania dla: " << flag << std::endl;
			exit(EXIT_FAILURE);
		}

		std::string value = argv[i + 1];

		if (!mapped_command) {
			if (command_name != CommandName::None) {

				if (i + 1 >= argc) {
					std::cerr << "Niepoprawny format parametru wywolania dla: " << flag << std::endl;
					exit(EXIT_FAILURE);
				}

				std::string value = argv[i + 1];
				mapped_command = true;
				AlgorithmType alg_type = map_to_algorithm_type(value);

				if (alg_type == AlgorithmType::AlgorithmNone) {
					std::cerr << "Zly wybor algorytmu, dostepne: approx|exact|both" << std::endl;
					exit(EXIT_FAILURE);
				}

				command.command_name = command_name;
				command.algorithm_type = alg_type;
			}
			
		}

		arguments.insert({ flag, value });
	}

	auto short_output_flag = arguments.find("-o");
	auto long_output_flag = arguments.find("--output");

	if (short_output_flag == arguments.end() && long_output_flag == arguments.end()) {
		command.long_output = true; // by default long output
	}
	else if (short_output_flag != arguments.end()) {
		if (short_output_flag->second == "short") {
			command.long_output = false;
		}
		else
		{
			command.long_output = true;
		}
	}
	else {
		if (long_output_flag->second == "short") {
			command.long_output = false;
		}
		else
		{
			command.long_output = true;
		}
	}


	if (command.command_name == CommandName::Distance) {
		auto file1 = arguments.find("-f1");
		auto file2 = arguments.find("-f2");

		if (file1 == arguments.end() || file2 == arguments.end()) {
			std::cerr << "Brak nazw plikow do obliczenia metryki miedzy grafami, spodziewany format: -f1 [nazwa_pliku1] -f2 [nazwa_pliku2]" << std::endl;
			exit(EXIT_FAILURE);
		}

		command.files.push_back(file1->second);
		command.files.push_back(file2->second);
	}
	else {
		auto file = arguments.find("-f");

		if (file == arguments.end()) {
			std::cerr << "Brak nazwy pliku do danych do obliczen, spodziewany format: -f [nazwa_pliku]" << std::endl;
			exit(EXIT_FAILURE);
		}

		command.files.push_back(file->second);
	}

	if (command.command_name == CommandName::Hamilton && command.algorithm_type != AlgorithmType::Exact)
	{
		int retryFactor = 10;
		auto retryFactorArg = arguments.find("-r");

		if (retryFactorArg != arguments.end())
		{
			retryFactor = std::stoi(retryFactorArg->second);

			if (retryFactor < 1 || retryFactor > 15)
			{
				std::cerr << "Parametr retryFactor powinien posiadac wartosc wieksza od 0 i mniejsza od 16 (aby uzyskac rozsadny czas wykonania algorytmu), spodziewany format: -r [wartosc retryFactor]" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		command.param = retryFactor;
	}

	return command;
}

CommandName ArgsParser::map_to_command_name(std::string flag) {

	if (flag == "-h" || flag == "--hamilton") {
		return CommandName::Hamilton;
	}
	else if (flag == "-d" || flag == "--distance") {
		return CommandName::Distance;
	}
	else if (flag == "-s" || flag == "--size") {
		return CommandName::Size;
	}
	else if (flag == "-c" || flag == "--cycles") {
		return CommandName::MaxCycles;
	}
	else if (flag == "--help" || flag == "help") {
		return CommandName::Help;
	}
	return CommandName::None;
}

AlgorithmType ArgsParser::map_to_algorithm_type(std::string value) {
	if (value == "exact") {
		return AlgorithmType::Exact;
	}
	else if (value == "approx") {
		return AlgorithmType::Approximate;
	}
	else if (value == "both") {
		return AlgorithmType::Both;
	}

	return AlgorithmType::AlgorithmNone;
}

