#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <string>
#include "StatisticsCollector.h"


class CsvWriter {
private:
    std::string filename;

public:
    CsvWriter(const std::string& filename);
    void write(const StatisticsCollector& stats) const;
};

#endif // CSVWRITER_H
