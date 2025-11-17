#include "Parser.h"
#include <cctype>

using namespace std;

vector<string> Parser::parseLine(const string& line) {
    vector<string> words;
    string currentWord;
    for (char symb : line) {
        if (isalnum(static_cast<unsigned char>(symb))) {
            currentWord += tolower(static_cast<unsigned char>(symb));
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
