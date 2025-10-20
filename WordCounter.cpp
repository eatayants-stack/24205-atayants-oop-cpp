#include "WordCounter.h"

using namespace std;

void WordCounter::addWords(const vector<string>& words) {
    for (const auto& word : words) {
        wordCounts[word]++;
        totalWords++;
    }
}

const map<string, int>& WordCounter::getWordCounts() const {
    return wordCounts;
}

int WordCounter::getTotalWords() const {
    return totalWords;
}
