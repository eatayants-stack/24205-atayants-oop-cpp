#include "CsvWriter.h"
CsvWriter::CsvWriter(const std::string& filename)
    : outputFile(filename) {
}

CsvWriter::~CsvWriter() {
    if (outputFile.is_open()) {
        outputFile.close();
    }
}

bool CsvWriter::isOpen() const {
    return outputFile.is_open();
}

void CsvWriter::write(const std::vector<std::string>& row) {
    if (!isOpen()) {
        return;
    }

    for (std::size_t i = 0; i < row.size(); ++i) {
        outputFile << row[i];
        if (i + 1 < row.size()) {
            outputFile << ',';
        }
    }
    outputFile << '\n';
    outputFile.flush();
}