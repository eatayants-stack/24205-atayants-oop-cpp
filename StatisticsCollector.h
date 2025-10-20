#ifndef STATISTICSCOLLECTOR_H
#define STATISTICSCOLLECTOR_H

#include <string>
#include <vector>
#include <map>
#include "WordCounter.h"


class StatisticsCollector {
private:
    std::vector<std::pair<std::string, int>> sortedWordStats;
    int totalWords = 0;

    static bool compareFrequency(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b);

public:
    void process(const WordCounter& counter);
    const std::vector<std::pair<std::string, int>>& getResults() const;
    int getTotalWords() const;
};

#endif // STATISTICSCOLLECTOR_H