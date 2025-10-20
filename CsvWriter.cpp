#include "CsvWriter.h"
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

CsvWriter::CsvWriter(const string& filename) : filename(filename) {}

void CsvWriter::write(const StatisticsCollector& stats) const {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Error: Couldn't create file " << filename << endl;
        exit(1);
    }

    outputFile << "Word,Frequence,Percentage (%)\n";
    outputFile << fixed << setprecision(2);

    int totalWords = stats.getTotalWords();
    const auto& results = stats.getResults();

    for (const auto& pair : results) {
        double percentage = (totalWords > 0) ? (static_cast<double>(pair.second) * 100.0 / totalWords) : 0.0;
        outputFile << pair.first << "," << pair.second << "," << percentage << "\n";
    }
    outputFile.close();
}
