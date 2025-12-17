#include "Engine.h"

#include <vector>

int Engine::CountNeighbours(const Universe& universe, int x, int y) const {
    static const int offsets[8][2] = {{-1, -1}, {0, -1},  {1, -1}, {1, 0},
                                      {1, 1},   {0, 1},   {-1, 1}, {-1, 0}};
    int count = 0;
    for (auto [dx, dy] : offsets) {
        if (universe.Get(x + dx, y + dy)) {
            ++count;
        }
    }
    return count;
}

void Engine::Tick(Universe& universe, std::size_t iterations) const {
    std::vector<bool> buffer(universe.Width() * universe.Height());

    for (std::size_t iter = 0; iter < iterations; ++iter) {
        for (std::size_t y = 0; y < universe.Height(); ++y) {
            for (std::size_t x = 0; x < universe.Width(); ++x) {
                bool alive = universe.Get(static_cast<int>(x), static_cast<int>(y));
                int neighbours = CountNeighbours(universe, static_cast<int>(x), static_cast<int>(y));
                bool next = universe.GetRule().WillLive(alive, neighbours);
                buffer[y * universe.Width() + x] = next;
            }
        }

        for (std::size_t y = 0; y < universe.Height(); ++y) {
            for (std::size_t x = 0; x < universe.Width(); ++x) {
                universe.Set(static_cast<int>(x), static_cast<int>(y),
                             buffer[y * universe.Width() + x]);
            }
        }
        universe.IncrementIteration();
    }
}

