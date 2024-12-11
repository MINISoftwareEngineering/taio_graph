#ifndef ProgramCommand_h
#define ProgramCommand_h

#include <program_command/command_name.h>
#include <program_command/algorithm_type.h>
#include <vector>
#include <string>


class ProgramCommand {
public:
	CommandName command_name;
	AlgorithmType algorithm_type;
	std::vector<std::string> files;
};

#endif // ProgramCommand_h 