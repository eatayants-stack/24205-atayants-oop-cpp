#ifndef WORDSTAT_H
#define WORDSTAT_H

#include <cstddef>
#include <string>

struct WordStat {
    std::string word;
    std::size_t frequency = 0;
    double percentage = 0.0;
};

#endif // WORDSTAT_H

