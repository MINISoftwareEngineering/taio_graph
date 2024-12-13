#include <config.h>
#include <app_controller/app_controller.h>
#include <args_parser.h>
#include <program_commanmd.h>

int main(int argc, char** argv)
{
	ArgsParser args_parser = ArgsParser();
	ProgramCommand command = args_parser.parse(argc, argv);

	AppController app_controller;
	RunData run_data;
	run_data.hamilton.approx.retry_factor = 10;

	app_controller.run(command, run_data);

	return EXIT_SUCCESS;
}