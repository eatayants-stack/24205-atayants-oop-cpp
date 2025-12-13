#pragma once

#include <set>
#include <string>
#include <vector>

class Rule {
public:
    Rule();
    Rule(std::set<int> birth, std::set<int> survive);

    static Rule FromString(const std::string& text, std::vector<std::string>& warnings);

    [[nodiscard]] bool WillLive(bool isAlive, int liveNeighbours) const;
    [[nodiscard]] std::string ToString() const;

private:
    std::set<int> birth_;
    std::set<int> survive_;
};

