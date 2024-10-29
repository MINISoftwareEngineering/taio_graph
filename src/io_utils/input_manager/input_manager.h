#pragma once
#include <config.h>

struct InputManager {
    
    bool inputFileExists();
    void openFile();
    void readLine(std::string& line);
    void closeFile();
    bool reachedEOF();

private:
    std::ifstream fileStream;
};