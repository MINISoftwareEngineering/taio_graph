#pragma once
#include <config.h>

struct InputManager {
    
    bool inputFileExists();
    void openFile(std::string input_path);
    void readLine(std::string& line);
    void closeFile();
    bool reachedEOF();
    void loadInputPaths(std::vector<std::string>& input_paths);
    int readNumber();
    std::string readLineFromStdin();
private:
    std::ifstream fileStream;
};