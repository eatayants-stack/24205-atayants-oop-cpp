#include "AppSupport.h"

#include <iostream>
#include <random>
#include <stdexcept>

Options ParseArgs(int argc, char* argv[]) {
    Options opts;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            opts.showHelp = true;
            continue;
        }
        if (arg.rfind("--iterations=", 0) == 0) {
            opts.iterations = static_cast<std::size_t>(std::stoul(arg.substr(13)));
            continue;
        }
        if (arg == "-i" && i + 1 < argc) {
            opts.iterations = static_cast<std::size_t>(std::stoul(argv[++i]));
            continue;
        }
        if (arg.rfind("--output=", 0) == 0) {
            opts.outputFile = arg.substr(9);
            continue;
        }
        if (arg == "-o" && i + 1 < argc) {
            opts.outputFile = argv[++i];
            continue;
        }
        if (arg[0] == '-') {
            throw std::runtime_error("Unknown option: " + arg);
        }
        opts.inputFile = arg;
    }
    return opts;
}

void SetupGlider(Universe& u, int offsetX, int offsetY) {
    u.Set(offsetX + 1, offsetY + 0, true);
    u.Set(offsetX + 2, offsetY + 1, true);
    u.Set(offsetX + 0, offsetY + 2, true);
    u.Set(offsetX + 1, offsetY + 2, true);
    u.Set(offsetX + 2, offsetY + 2, true);
}

void SetupBlinker(Universe& u, int offsetX, int offsetY) {
    u.Set(offsetX + 1, offsetY + 0, true);
    u.Set(offsetX + 1, offsetY + 1, true);
    u.Set(offsetX + 1, offsetY + 2, true);
}

void SetupBlock(Universe& u, int offsetX, int offsetY) {
    u.Set(offsetX + 0, offsetY + 0, true);
    u.Set(offsetX + 1, offsetY + 0, true);
    u.Set(offsetX + 0, offsetY + 1, true);
    u.Set(offsetX + 1, offsetY + 1, true);
}

void SetupRPentomino(Universe& u, int offsetX, int offsetY) {
    u.Set(offsetX + 1, offsetY + 0, true);
    u.Set(offsetX + 2, offsetY + 0, true);
    u.Set(offsetX + 0, offsetY + 1, true);
    u.Set(offsetX + 1, offsetY + 1, true);
    u.Set(offsetX + 1, offsetY + 2, true);
}

void SetupToad(Universe& u, int offsetX, int offsetY) {
    u.Set(offsetX + 1, offsetY + 0, true);
    u.Set(offsetX + 2, offsetY + 0, true);
    u.Set(offsetX + 3, offsetY + 0, true);
    u.Set(offsetX + 0, offsetY + 1, true);
    u.Set(offsetX + 1, offsetY + 1, true);
    u.Set(offsetX + 2, offsetY + 1, true);
}

Universe MakeShowcaseUniverse() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 4);

    Rule rule;
    Universe universe(35, 35, rule, "Showcase Universe");

    int pattern = dis(gen);

    switch (pattern) {
        case 0:  // Glider
            universe.SetName("Glider");
            SetupGlider(universe, 15, 15);
            break;
        case 1:  // Blinker
            universe.SetName("Blinker Oscillator");
            SetupBlinker(universe, 16, 16);
            break;
        case 2:  // R-Pentomino
            universe.SetName("R-Pentomino");
            SetupRPentomino(universe, 15, 15);
            break;
        case 3:  // Toad
            universe.SetName("Toad Oscillator");
            SetupToad(universe, 15, 15);
            break;
        case 4:  // Mixed showcase
            universe.SetName("Mixed Patterns");
            SetupGlider(universe, 5, 5);
            SetupBlinker(universe, 15, 10);
            SetupBlock(universe, 25, 5);
            SetupRPentomino(universe, 10, 20);
            SetupToad(universe, 20, 20);
            break;
    }

    return universe;
}

void PrintUsage() {
    std::cout << "Conway's Game of Life\n";
    std::cout << "Usage:\n";
    std::cout << "  life_cli [input_file]\n";
    std::cout << "  life_cli [input_file] -i N -o output_file\n";
    std::cout << "Options:\n";
    std::cout << "  -i, --iterations=N   Number of iterations to perform (offline mode)\n";
    std::cout << "  -o, --output=FILE    Output file for offline mode\n";
    std::cout << "  -h, --help           Show this help\n";
}

