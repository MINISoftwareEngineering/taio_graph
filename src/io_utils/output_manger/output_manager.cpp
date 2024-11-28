#include "output_manager.h"

// Function to create a CSV file
void createCSV(const std::string& fileName) {
    std::ofstream file(fileName);

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }
    // Close the file
    file.close();
    std::cout << "CSV file created successfully: " << fileName << std::endl;
}

void writeDataToCSV(const std::string& fileName, const std::vector<std::vector<std::string>>& data, bool append) {
    std::ofstream file;

    // Open the file in append or overwrite mode
    if (append) {
        file.open(fileName, std::ios::app);
    }
    else {
        file.open(fileName);
    }

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    // Write data rows
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) {
                file << ";"; // Separate columns with commas
            }
        }
        file << "\n"; // Newline for the next row
    }

    // Close the file
    file.close();
    std::cout << "Data written to CSV file: " << fileName << std::endl;
}

