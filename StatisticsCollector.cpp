#include "StatisticsCollector.h"
#include <algorithm>

using namespace std;

bool StatisticsCollector::compareFrequency(const pair<string, int>& a, const pair<string, int>& b) {
    return a.second > b.second;
}

void StatisticsCollector::process(const WordCounter& counter) {
    totalWords = counter.getTotalWords();
    const auto& counts = counter.getWordCounts();

    sortedWordStats.assign(counts.begin(), counts.end());
    
    sort(sortedWordStats.begin(), sortedWordStats.end(), compareFrequency);
}

const vector<pair<string, int>>& StatisticsCollector::getResults() const {
    return sortedWordStats;
}

int StatisticsCollector::getTotalWords() const {
    return totalWords;
}
