#pragma once
#include <config.h>

struct ConsoleManager
{
    void clear()
    {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void write(std::string text)
    {
        std::cout << text;
    }

    void wait_for_enter()
    {
        std::cout << "[Press Enter to continue]\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        clear();
    }
};