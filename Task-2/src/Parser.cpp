#include "Parser.h"

#include <filesystem>
#include <fstream>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>

namespace {
bool StartsWith(const std::string& line, const std::string& prefix) {
    return line.rfind(prefix, 0) == 0;
}
} // namespace

Universe Parser::Load(const std::string& path, std::vector<std::string>& warnings) {
    std::ifstream input(path);
    if (!input.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }

    std::string line;
    std::size_t lineNumber = 0;
    std::string name = "Unnamed";
    std::string ruleText;
    std::optional<std::pair<std::size_t, std::size_t>> sizeHint;
    std::vector<std::pair<int, int>> coordinates;
    std::set<std::pair<int, int>> seen;

    auto warn = [&warnings](const std::string& msg) { warnings.push_back(msg); };

    while (std::getline(input, line)) {
        ++lineNumber;
        if (lineNumber == 1) {
            if (line != "Life 1.06") {
                throw std::runtime_error("Unsupported format: expected 'Life 1.06'");
            }
            continue;
        }

        if (line.empty()) {
            continue;
        }

        if (StartsWith(line, "#N")) {
            if (line.size() > 3) {
                name = line.substr(3);
            } else {
                warn("Universe name is empty");
            }
            continue;
        }

        if (StartsWith(line, "#R")) {
            if (line.size() > 3) {
                ruleText = line.substr(3);
            } else {
                warn("Rule is empty, defaulting to B3/S23");
            }
            continue;
        }

        if (StartsWith(line, "#S")) {
            std::istringstream ss(line.substr(3));
            std::size_t w = 0;
            std::size_t h = 0;
            if (ss >> w >> h) {
                sizeHint = {w, h};
            } else {
                warn("Cannot parse size from '#S' line, ignoring");
            }
            continue;
        }

        if (line[0] == '#') {
            warn("Unknown comment: " + line);
            continue;
        }

        std::istringstream ss(line);
        int x = 0;
        int y = 0;
        if (!(ss >> x >> y)) {
            warn("Cannot parse coordinates at line " + std::to_string(lineNumber));
            continue;
        }
        std::pair<int, int> coord{x, y};
        if (seen.count(coord)) {
            warn("Duplicate coordinate ignored: (" + std::to_string(x) + ", " + std::to_string(y) +
                 ")");
            continue;
        }
        seen.insert(coord);
        coordinates.push_back(coord);
    }

    Rule rule = Rule();
    if (!ruleText.empty()) {
        rule = Rule::FromString(ruleText, warnings);
    } else {
        warn("Rule not found, defaulting to B3/S23");
    }

    std::size_t width = 20;
    std::size_t height = 20;
    if (sizeHint) {
        width = sizeHint->first;
        height = sizeHint->second;
    } else if (!coordinates.empty()) {
        int minX = coordinates.front().first;
        int maxX = minX;
        int minY = coordinates.front().second;
        int maxY = minY;
        for (const auto& [x, y] : coordinates) {
            minX = std::min(minX, x);
            maxX = std::max(maxX, x);
            minY = std::min(minY, y);
            maxY = std::max(maxY, y);
        }
        width = static_cast<std::size_t>(maxX - minX + 1);
        height = static_cast<std::size_t>(maxY - minY + 1);
        // Keep universe reasonably sized
        width = std::max<std::size_t>(width, 5);
        height = std::max<std::size_t>(height, 5);
    }

    Universe universe(width, height, rule, name);
    for (const auto& [x, y] : coordinates) {
        universe.Set(x, y, true);
    }

    return universe;
}

void Parser::Save(const Universe& universe, const std::string& path) {
    std::ofstream output(path);
    if (!output.is_open()) {
        throw std::runtime_error("Cannot open output file: " + path);
    }

    output << "Life 1.06\n";
    output << "#N " << universe.Name() << "\n";
    output << "#R " << universe.GetRule().ToString() << "\n";
    output << "#S " << universe.Width() << " " << universe.Height() << "\n";
    for (const auto& [x, y] : universe.LiveCells()) {
        output << x << " " << y << "\n";
    }
}

