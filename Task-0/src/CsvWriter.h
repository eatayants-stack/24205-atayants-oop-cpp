#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <fstream>
#include <string>
#include <vector>

class CsvWriter {
private:
    std::ofstream outputFile;

public:
    explicit CsvWriter(const std::string& filename);
    ~CsvWriter();
    bool isOpen() const;
    void write(const std::vector<std::string>& row);
};

#endif // CSVWRITER_H
