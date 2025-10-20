#include "Parser.h"
#include <cctype>


std::vector<std::string> Parser::parseLine(const std::string& line) {
    std::vector<std::string> words;
    std::string currentWord;
    for (char ch : line) {
        if (isalnum(static_cast<unsigned char>(ch))) {
            currentWord += tolower(static_cast<unsigned char>(ch));
        } else {
            if (!currentWord.empty()) {
                words.push_back(currentWord);
                currentWord.clear();
            }
        }
    }
    if (!currentWord.empty()) {
        words.push_back(currentWord);
    }
    return words;
}
