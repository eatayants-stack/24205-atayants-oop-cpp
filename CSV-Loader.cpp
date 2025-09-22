#include "CSV-Loader.h"
#include<fstream>
#include<map>
#include<iostream>
void DataToCSV(const std::map<std::string, int>& wordFrequencies, const std::string& filename) {
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        std::cerr << "Error: Couldn't create file " << filename << std::endl;
        return;
    }


    outputFile << "Word,Frequency" << std::endl;

    for (const auto& pair : wordFrequencies) {
        outputFile << pair.first << "," << pair.second << std::endl;
    }

    outputFile.close();
    std::cout << "Data loaded to CSV file " << filename << std::endl;
}