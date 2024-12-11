#include <program_command/program_commanmd.h>
#include <map>
#include <string>

class ArgsParser {
private:
	std::map<std::string, std::string> arguments;
	CommandName map_to_command_name(std::string);
	AlgorithmType map_to_algorithm_type(std::string value);
public:
	ProgramCommand parse(int argc, char** argv);
};