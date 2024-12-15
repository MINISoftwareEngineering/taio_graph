#include <config.h>
#include <app_controller/app_controller.h>
#include <args_parser.h>
#include <program_commanmd.h>
#include <locale>       // For locale settings
#include <codecvt>      // For wide character conversion (if needed)

int main(int argc, char** argv)
{
	ArgsParser args_parser = ArgsParser();
	ProgramCommand command = args_parser.parse(argc, argv);

	AppController app_controller;
	RunData run_data;
	run_data.hamilton.approx.retry_factor = command.param;

	app_controller.run(command, run_data);

	return EXIT_SUCCESS;
}