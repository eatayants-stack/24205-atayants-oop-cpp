#pragma once

#include "Universe.h"

#include <string>
#include <vector>

class Parser {
public:
    static Universe Load(const std::string& path, std::vector<std::string>& warnings);
    static void Save(const Universe& universe, const std::string& path);
};

