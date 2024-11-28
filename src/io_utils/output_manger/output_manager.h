#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Function to create a CSV file
void createCSV(const std::string& fileName);
void writeDataToCSV(const std::string& fileName, const std::vector<std::vector<std::string>>& data, bool append = false);