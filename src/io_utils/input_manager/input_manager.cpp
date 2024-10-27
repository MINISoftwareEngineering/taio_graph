#include "input_manager.h"

bool InputManager::inputFileExists()
{
    std::ifstream file(DATA_PATH);
    return file.good();
}

void InputManager::openFile()
{
    if (fileStream.is_open()) 
    {
        closeFile();
    }

    fileStream.open(DATA_PATH);

    if (!fileStream.is_open()) 
    {
        throw std::runtime_error("Failed to open file: " DATA_PATH);
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