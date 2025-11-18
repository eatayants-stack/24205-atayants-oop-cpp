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

public:
    void addWords(const std::vector<std::string>& words);
    void process();
    const std::vector<WordStat>& getResults() const;
    const std::map<std::string, std::size_t>& getWordCounts() const;
    std::size_t getTotalWords() const;
};

#endif // STATISTICSCOLLECTOR_H