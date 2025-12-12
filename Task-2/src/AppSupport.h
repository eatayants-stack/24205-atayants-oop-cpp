#pragma once

#include "Rule.h"
#include "Universe.h"

#include <optional>
#include <string>

struct Options {
    std::optional<std::string> inputFile;
    std::optional<std::string> outputFile;
    std::optional<std::size_t> iterations;
    bool showHelp{false};
};

Options ParseArgs(int argc, char* argv[]);

Universe MakeShowcaseUniverse();

void PrintUsage();

