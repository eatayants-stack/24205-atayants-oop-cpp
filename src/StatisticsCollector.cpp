#include "StatisticsCollector.h"

#include <algorithm>

void StatisticsCollector::addWords(const std::vector<std::string>& words) {
    for (const auto& word : words) {
        if (word.empty()) {
            continue;
        }
        ++wordCounts[word];
        ++totalWords;
    }
}

void StatisticsCollector::process() {
    results.clear();

    if (wordCounts.empty()) {
        return;
    }

    results.reserve(wordCounts.size());
    for (const auto& [word, count] : wordCounts) {
        double percentage = static_cast<double>(count) * 100.0 / static_cast<double>(totalWords);
        results.push_back({word, count, percentage});
    }

    std::stable_sort(results.begin(), results.end(), [](const WordStat& lhs, const WordStat& rhs) {
        if (lhs.getFrequency() == rhs.getFrequency()) {
            return lhs.getWord() < rhs.getWord();
        }
        return lhs.getFrequency() > rhs.getFrequency();
    });
}

const std::vector<WordStat>& StatisticsCollector::getResults() {
    process();
    return results;
}