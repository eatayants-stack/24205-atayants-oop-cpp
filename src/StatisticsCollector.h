#ifndef STATISTICSCOLLECTOR_H
#define STATISTICSCOLLECTOR_H

#include <cstddef>
#include <string>
#include <vector>

class WordCounter;

struct WordStat {
    std::string word;
    std::size_t frequency = 0;
    double percentage = 0.0;
};

class StatisticsCollector {
private:
    std::vector<WordStat> results;

public:
    void process(const WordCounter& counter);
    const std::vector<WordStat>& getResults() const;
};


#endif // STATISTICSCOLLECTOR_H