#include "Universe.h"

#include <sstream>
#include <stdexcept>

Universe::Universe(std::size_t width, std::size_t height, Rule rule, std::string name)
    : width_(width), height_(height), rule_(std::move(rule)), name_(std::move(name)),
      cells_(width * height, false) {
    if (width_ == 0 || height_ == 0) {
        throw std::invalid_argument("Universe dimensions must be positive");
    }
}

std::size_t Universe::NormalizeX(int x) const {
    int w = static_cast<int>(width_);
    int normalized = ((x % w) + w) % w;
    return static_cast<std::size_t>(normalized);
}

std::size_t Universe::NormalizeY(int y) const {
    int h = static_cast<int>(height_);
    int normalized = ((y % h) + h) % h;
    return static_cast<std::size_t>(normalized);
}

std::size_t Universe::Index(std::size_t x, std::size_t y) const { return y * width_ + x; }

bool Universe::Get(int x, int y) const {
    auto nx = NormalizeX(x);
    auto ny = NormalizeY(y);
    return cells_[Index(nx, ny)];
}

void Universe::Set(int x, int y, bool value) {
    auto nx = NormalizeX(x);
    auto ny = NormalizeY(y);
    cells_[Index(nx, ny)] = value;
}

std::vector<std::pair<int, int>> Universe::LiveCells() const {
    std::vector<std::pair<int, int>> result;
    for (std::size_t y = 0; y < height_; ++y) {
        for (std::size_t x = 0; x < width_; ++x) {
            if (cells_[Index(x, y)]) {
                result.emplace_back(static_cast<int>(x), static_cast<int>(y));
            }
        }
    }
    return result;
}

std::string Universe::ToDisplayString(char alive, char dead) const {
    std::ostringstream ss;
    
    // Top border
    ss << ' ';
    for (std::size_t x = 0; x < width_; ++x) {
        ss << "__";
    }
    ss << '\n';
    
    // Grid with side borders
    for (std::size_t y = 0; y < height_; ++y) {
        ss << '|';
        for (std::size_t x = 0; x < width_; ++x) {
            ss << (cells_[Index(x, y)] ? alive : dead) << ' ';
        }
        ss << "|\n";
    }
    
    // Bottom border
    ss << ' ';
    for (std::size_t x = 0; x < width_; ++x) {
        ss << "--";
    }
    
    return ss.str();
}

