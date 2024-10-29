#include <config.h>
#include <app_controller/app_controller.h>

#include <filesystem>

int main()
{
	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    

	AppController app_controller;
	app_controller.run();

	return EXIT_SUCCESS;
}