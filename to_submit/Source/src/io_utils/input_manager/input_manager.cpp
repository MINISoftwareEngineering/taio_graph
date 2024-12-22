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

bool InputManager::inputFileExists(std::string file_path) {
    return std::filesystem::exists(file_path);
}

bool  InputManager::inputFilesExist(std::string file_path1, std::string file_path2) {
    return inputFileExists(file_path1) && inputFileExists(file_path2);
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

int InputManager::readNumber()
{
    int x = 0;
    while (true)
    {
        std::cin >> x;
        if (!std::cin.fail()) {
            break;
        }
        else {
            std::cout << "Bad entry. Enter a NUMBER: " << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return x;
}

std::string InputManager::readLineFromStdin() {
    std::string line;
    std::getline(std::cin, line); // Read the entire line from stdin
    return line;
}