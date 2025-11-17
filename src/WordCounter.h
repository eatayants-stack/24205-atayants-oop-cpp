#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H

#include <cstddef>
#include <map>
#include <string>
#include <vector>

class WordCounter {
private:
    std::map<std::string, std::size_t> wordCounts;
    std::size_t totalWords = 0;

public:
    void addWords(const std::vector<std::string>& words);

    const std::map<std::string, std::size_t>& getWordCounts() const;
    std::size_t getTotalWords() const;
};

#endif // WORDCOUNTER_H
