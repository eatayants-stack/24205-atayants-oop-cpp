#include "Parser.h"
#include "Normalizer.h"

std::map<std::string, int> Parse(const std::string& text) {
    std::map<std::string, int> wordCounts;
    std::string current_word;

    for (char c : text) {
        if (std::isspace(c)) {
            if (!current_word.empty()) {
                std::string normalized = NormalizeWord(current_word);
                if (!normalized.empty()) {
                    wordCounts[normalized]++;
                }
                current_word.clear();
            }
        } else {
            current_word += c;
        }
    }
    if (!current_word.empty()) {
        std::string normalized = NormalizeWord(current_word);
        if (!normalized.empty()) {
            wordCounts[normalized]++;
        }
    }
    return wordCounts;
}