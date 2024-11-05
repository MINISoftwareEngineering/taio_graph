#include "input_manager.h"

bool InputManager::inputFileExists()
{
    for (const auto& entry : std::filesystem::directory_iterator(INPUT_FOLDER_PATH))
    {
        if (entry.is_regular_file())
            return true;
    }
    return false;
}

void InputManager::openFile(std::string input_path)
{
    if (fileStream.is_open()) 
    {
        closeFile();
    }

    fileStream.open(input_path);

    if (!fileStream.is_open()) 
    {
        throw std::runtime_error("Failed to open file: " INPUT_FOLDER_PATH);
    }
}

void InputManager::readLine(std::string& line)
{
    if (!fileStream.is_open()) 
    {
        throw std::runtime_error("No file is open.");
    }

    if (reachedEOF())
    {
        throw new std::runtime_error("No line to read.");
    }
        
    std::string _line;
    if (std::getline(fileStream, _line)) {
        line = _line;
    }
}

void InputManager::closeFile()
{
    if (fileStream.is_open()) 
    {
        fileStream.close();
    }
}

bool InputManager::reachedEOF()
{
    return fileStream.eof();
}

void InputManager::loadInputPaths(std::vector<std::string>& input_paths)
{
    for (const auto& entry : std::filesystem::directory_iterator(INPUT_FOLDER_PATH))
    {
        if (entry.is_regular_file())
            input_paths.push_back(entry.path().string());
    }
}