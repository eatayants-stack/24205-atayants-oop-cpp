#include "MergeWordCount.h"

void MergeWordCounts(std::map<std::string, int>& totalCounts, const std::map<std::string, int>& newCounts) {
    for (const auto& pair : newCounts) {
        totalCounts[pair.first] += pair.second;
    }
}