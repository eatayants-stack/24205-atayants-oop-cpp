#pragma once

#include "Universe.h"

#include <cstddef>

class Engine {
public:
    void Tick(Universe& universe, std::size_t iterations = 1) const;

private:
    int CountNeighbours(const Universe& universe, int x, int y) const;
};

