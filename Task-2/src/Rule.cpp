#include "Rule.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace {
bool IsDigit(char c) { return c >= '0' && c <= '8'; }
}

Rule::Rule() : birth_{3}, survive_{2, 3} {}

Rule::Rule(std::set<int> birth, std::set<int> survive)
    : birth_(std::move(birth)), survive_(std::move(survive)) {}

Rule Rule::FromString(const std::string& text, std::vector<std::string>& warnings) {
    std::set<int> birth;
    std::set<int> survive;

    enum class Section { None, Birth, Survive };
    Section current = Section::None;

    auto emitWarning = [&warnings](const std::string& msg) { warnings.push_back(msg); };

    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        if (c == 'B' || c == 'b') {
            current = Section::Birth;
            continue;
        }
        if (c == 'S' || c == 's') {
            current = Section::Survive;
            continue;
        }
        if (!IsDigit(c)) {
            if (!std::isspace(static_cast<unsigned char>(c)) && c != '/') {
                emitWarning("Unexpected symbol in rule: " + std::string(1, c));
            }
            continue;
        }
        int value = c - '0';
        if (value < 0 || value > 8) {
            emitWarning("Neighbour count out of range: " + std::to_string(value));
            continue;
        }
        if (current == Section::Birth) {
            birth.insert(value);
        } else if (current == Section::Survive) {
            survive.insert(value);
        } else {
            emitWarning("Digit before specifying Birth/Survive section: " + std::string(1, c));
        }
    }

    if (birth.empty() && survive.empty()) {
        emitWarning("Rule is empty, defaulting to B3/S23");
        return Rule();
    }

    return Rule(birth, survive);
}

bool Rule::WillLive(bool isAlive, int liveNeighbours) const {
    if (liveNeighbours < 0 || liveNeighbours > 8) {
        throw std::out_of_range("liveNeighbours must be between 0 and 8");
    }
    if (isAlive) {
        return survive_.count(liveNeighbours) > 0;
    }
    return birth_.count(liveNeighbours) > 0;
}

std::string Rule::ToString() const {
    std::string result = "B";
    for (int n : birth_) {
        result.push_back(static_cast<char>('0' + n));
    }
    result += "/S";
    for (int n : survive_) {
        result.push_back(static_cast<char>('0' + n));
    }
    return result;
}

