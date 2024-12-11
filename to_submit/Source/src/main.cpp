#include <config.h>
#include <app_controller/app_controller.h>

int main()
{
	AppController app_controller;
	RunData run_data;
	run_data.hamilton.approx.retry_factor = 10;

	app_controller.run(run_data);

	return EXIT_SUCCESS;
}