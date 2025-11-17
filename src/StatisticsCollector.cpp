#include "StatisticsCollector.h"

#include "WordCounter.h"

#include <algorithm>

void StatisticsCollector::process(const WordCounter& counter) {
    results.clear();

    const auto& counts = counter.getWordCounts();
    const auto totalWords = counter.getTotalWords();

    if (counts.empty() || totalWords == 0) {
        return;
    }

    results.reserve(counts.size());
    for (const auto& [word, count] : counts) {
        double percentage = static_cast<double>(count) * 100.0 / static_cast<double>(totalWords);
        results.push_back({word, count, percentage});
    }

    std::stable_sort(results.begin(), results.end(), [](const WordStat& lhs, const WordStat& rhs) {
        if (lhs.frequency == rhs.frequency) {
            return lhs.word < rhs.word;
        }
        return lhs.frequency > rhs.frequency;
    });
}

const std::vector<WordStat>& StatisticsCollector::getResults() const {
    return results;
}