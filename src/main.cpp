#include <config.h>
#include <input_manager/input_manager.h>
#include <console_manager/console_manager.h>

int main()
{
	InputManager input_manager;
	ConsoleManager console_manager;

	if (input_manager.inputFileExists())
	{
		console_manager.write("Input file detected\n");
	}
	else
	{
		console_manager.write("Create " DATA_PATH " file\n");
	}
	

	return EXIT_SUCCESS;
}