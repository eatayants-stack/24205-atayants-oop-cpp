#include "TotalWords.h"

int TotalWords(const std::map<std::string, int>& wordCounts) {
    int total = 0;
    for (const auto& pair : wordCounts) {
        total += pair.second;
    }
    return total;
}