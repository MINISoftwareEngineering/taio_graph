#include <config.h>
#include <app_controller/app_controller.h>

#include <filesystem>

int main()
{
	AppController app_controller;
	app_controller.run();

	return EXIT_SUCCESS;
}