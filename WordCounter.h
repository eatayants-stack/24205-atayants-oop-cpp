#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H

#include <string>
#include <vector>
#include <map>


class WordCounter {
private:
    std::map<std::string, int> wordCounts;
    int totalWords = 0;

public:
    void addWords(const std::vector<std::string>& words);
    const std::map<std::string, int>& getWordCounts() const;
    int getTotalWords() const;
};

#endif // WORDCOUNTER_H
