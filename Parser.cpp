#include "Parser.h"
#include "normalize.h"
#include <iostream>
#include <map>
void parse(const std::string& text, std::map<std::string, int>& wordCounts) {
    std::string current_word;

    for (char c : text) {
        if (std::isspace(c)) {
            if (!current_word.empty()) {
                std::string normalized = normalizeWord(current_word);
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
        std::string normalized = normalizeWord(current_word);
        if (!normalized.empty()) {
            wordCounts[normalized]++;
        }
    }
}