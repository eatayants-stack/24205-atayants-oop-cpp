#include "WordStat.h"

std::size_t WordStat::getFrequency() const {
    return frequency;
}
double WordStat::getPercentage() const {
    return percentage;
}
const std::string &WordStat::getWord() const {
    return word;
}