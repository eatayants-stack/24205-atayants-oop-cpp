#ifndef STATISTICSCOLLECTOR_H
#define STATISTICSCOLLECTOR_H

#include "WordStat.h"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

class StatisticsCollector {
private:
    std::map<std::string, std::size_t> wordCounts;
    std::size_t totalWords = 0;
    std::vector<WordStat> results;
    void process();

public:
    void addWords(const std::vector<std::string>& words);
    const std::vector<WordStat>& getResults();
};

#endif // STATISTICSCOLLECTOR_H