#include "AppSupport.h"
#include "ConsoleUI.h"
#include "Engine.h"
#include "Parser.h"
#include "Universe.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    Options options;
    try {
        options = ParseArgs(argc, argv);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        PrintUsage();
        return EXIT_FAILURE;
    }

    if (options.showHelp) {
        PrintUsage();
        return EXIT_SUCCESS;
    }

    std::vector<std::string> warnings;
    Universe universe = MakeShowcaseUniverse();

    if (options.inputFile) {
        try {
            universe = Parser::Load(*options.inputFile, warnings);
        } catch (const std::exception& ex) {
            std::cerr << "Failed to load '" << *options.inputFile << "': " << ex.what() << "\n";
            return EXIT_FAILURE;
        }
    }

    for (const auto& w : warnings) {
        std::cerr << "Warning: " << w << "\n";
    }

    // Offline mode
    if (options.iterations && options.outputFile) {
        Engine engine;
        engine.Tick(universe, *options.iterations);
        try {
            Parser::Save(universe, *options.outputFile);
            std::cout << "Saved universe after " << *options.iterations
                      << " iterations to " << *options.outputFile << "\n";
            return EXIT_SUCCESS;
        } catch (const std::exception& ex) {
            std::cerr << "Failed to save: " << ex.what() << "\n";
            return EXIT_FAILURE;
        }
    }

    ConsoleUI ui;
    ui.Run(universe, std::cin, std::cout);
    return EXIT_SUCCESS;
}
