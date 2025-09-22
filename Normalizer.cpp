#include "Normalizer.h"

std::string NormalizeWord(const std::string& word) {
    if (word.empty()) {
        return "";
    }

    size_t first = 0;
    while (first < word.length() && !std::isalpha(word[first])) {
        first++;
    }

    size_t last = word.length() - 1;
    while (last > first && !std::isalpha(word[last])) {
        last--;
    }

    if (first > last) {
        return "";
    }

    std::string normalized_word = word.substr(first, last - first + 1);

    for (char& c : normalized_word) {
        c = std::tolower(c);
    }

    return normalized_word;
}