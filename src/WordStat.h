#ifndef WORDSTAT_H
#define WORDSTAT_H

#include <cstddef>
#include <string>

class WordStat {
private:
    std::string word;
    std::size_t frequency;
    double percentage;

public:
    WordStat(const std::string& w, const std::size_t c, const double f = 0.0)
        : word(w), frequency(c), percentage(f) {}

    const std::string& getWord() const;
    std::size_t getFrequency() const;
    double getPercentage() const;
};

#endif // WORDSTAT_H

