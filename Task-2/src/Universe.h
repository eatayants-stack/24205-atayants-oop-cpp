#pragma once

#include "Rule.h"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

class Universe {
public:
    Universe(std::size_t width, std::size_t height, Rule rule, std::string name = "Unnamed");

    [[nodiscard]] std::size_t Width() const { return width_; }
    [[nodiscard]] std::size_t Height() const { return height_; }
    [[nodiscard]] std::size_t Iteration() const { return iteration_; }
    void SetIteration(std::size_t iter) { iteration_ = iter; }
    void IncrementIteration() { ++iteration_; }

    [[nodiscard]] const std::string& Name() const { return name_; }
    void SetName(std::string name) { name_ = std::move(name); }

    [[nodiscard]] const Rule& GetRule() const { return rule_; }
    void SetRule(const Rule& rule) { rule_ = rule; }

    bool Get(int x, int y) const;
    void Set(int x, int y, bool value);

    [[nodiscard]] std::vector<std::pair<int, int>> LiveCells() const;
    [[nodiscard]] std::string ToDisplayString(char alive = '#', char dead = '.') const;

private:
    std::size_t NormalizeX(int x) const;
    std::size_t NormalizeY(int y) const;
    std::size_t Index(std::size_t x, std::size_t y) const;

    std::size_t width_;
    std::size_t height_;
    std::size_t iteration_{0};
    Rule rule_;
    std::string name_;
    std::vector<bool> cells_;
};

