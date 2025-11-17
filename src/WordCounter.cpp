#include "WordCounter.h"

void WordCounter::addWords(const std::vector<std::string>& words) {
    for (const auto& word : words) {
        if (word.empty()) {
            continue;
        }
        ++wordCounts[word];
        ++totalWords;
    }
}

const std::map<std::string, std::size_t>& WordCounter::getWordCounts() const {
    return wordCounts;
}

std::size_t WordCounter::getTotalWords() const {
    return totalWords;
}
