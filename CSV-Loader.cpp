#include "CSV-Loader.h"
#include<iomanip>
void DataToCSV(const std::map<std::string, int>& wordFrequencies, const int totalWords, const std::string& filename ) {
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        std::cerr << "Error: Couldn't create file " << filename << std::endl;
        return;
    }


    outputFile << "Word,Frequency,Percentage" << std::endl;

    for (const auto& pair : wordFrequencies) {
        double percentage = (static_cast<double>(pair.second) / totalWords) * 100.0;
        outputFile << pair.first << "," << pair.second << "," << std::fixed << std::setprecision(2) << percentage << std::endl;
    }
    outputFile.close();

    outputFile.close();
    std::cout << "Data loaded to CSV file " << filename << std::endl;
}