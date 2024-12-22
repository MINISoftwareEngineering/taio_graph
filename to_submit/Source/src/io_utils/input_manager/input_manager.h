#pragma once
#include <config.h>
#include <string>

struct InputManager {
    
    bool inputFileExists();
    bool inputFilesExist(std::string file_path1, std::string file_path2);
    bool inputFileExists(std::string file_path);
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