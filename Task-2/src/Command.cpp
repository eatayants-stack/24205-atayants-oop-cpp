#include "Command.h"

#include "Engine.h"
#include "Parser.h"
#include "Universe.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

TickCommand::TickCommand(std::size_t steps) : steps_(steps) {}

ExecutionResult TickCommand::Execute(Universe& universe, Engine& engine, std::ostream& output) {
    if (steps_ == 0) {
        output << "Error: Number of steps must be positive\n";
        return ExecutionResult::Continue;
    }
    engine.Tick(universe, steps_);
    return ExecutionResult::Continue;
}

AnimateCommand::AnimateCommand(std::size_t steps, int delay_ms)
    : steps_(steps), delay_ms_(delay_ms) {}

ExecutionResult AnimateCommand::Execute(Universe& universe, Engine& engine,
                                         std::ostream& output) {
    if (steps_ == 0) {
        output << "Error: Number of steps must be positive\n";
        return ExecutionResult::Continue;
    }

    auto showFrame = [&]() {
        output << "\033[2J\033[H";  // Clear screen and move cursor to top
        output << "Universe: " << universe.Name() << " | Rule: " << universe.GetRule().ToString()
               << " | Iteration: " << universe.Iteration() << "\n";
        output << universe.ToDisplayString() << "\n";
        output << std::flush;
    };

    for (std::size_t i = 0; i < steps_; ++i) {
        engine.Tick(universe, 1);
        showFrame();
        if (i < steps_ - 1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms_));
        }
    }
    return ExecutionResult::Continue;
}

DumpCommand::DumpCommand(std::string filename) : filename_(std::move(filename)) {}

ExecutionResult DumpCommand::Execute(Universe& universe, Engine& engine,
                                      std::ostream& output) {
    if (filename_.empty()) {
        output << "Error: Filename is required\n";
        return ExecutionResult::Continue;
    }
    try {
        Parser::Save(universe, filename_);
        output << "Universe saved to " << filename_ << "\n";
    } catch (const std::exception& ex) {
        output << "Error saving file: " << ex.what() << "\n";
    }
    return ExecutionResult::Continue;
}

ExecutionResult HelpCommand::Execute(Universe& universe, Engine& engine,
                                     std::ostream& output) {
    output << "\nAvailable commands:\n";
    output << "  tick [n] | t [n]     - advance simulation by n iterations (default: 1)\n";
    output << "  animate [n] | a [n]   - animate n iterations with visualization (default: 1)\n";
    output << "  dump <filename>       - save current universe to file\n";
    output << "  help                  - show this help message\n";
    output << "  exit | quit           - exit the program\n";
    output << "\n";
    return ExecutionResult::Continue;
}

ExecutionResult ExitCommand::Execute(Universe& universe, Engine& engine,
                                      std::ostream& output) {
    return ExecutionResult::Exit;
}

std::unique_ptr<Command> CommandFactory::Create(const std::string& commandLine) {
    std::istringstream ss(commandLine);
    std::string cmd;
    ss >> cmd;

    if (cmd.empty()) {
        return nullptr;
    }

    if (cmd == "help" || cmd == "h") {
        return std::make_unique<HelpCommand>();
    }

    if (cmd == "exit" || cmd == "quit" || cmd == "q") {
        return std::make_unique<ExitCommand>();
    }

    if (cmd == "tick" || cmd == "t") {
        std::size_t steps = 1;
        std::string arg;
        if (ss >> arg) {
            try {
                int parsed = std::stoi(arg);
                if (parsed > 0) {
                    steps = static_cast<std::size_t>(parsed);
                }
            } catch (...) {
                // Invalid number, use default
            }
        }
        return std::make_unique<TickCommand>(steps);
    }

    if (cmd == "animate" || cmd == "a") {
        std::size_t steps = 1;
        int delay = 200;
        std::string arg1, arg2;
        if (ss >> arg1) {
            try {
                int parsed = std::stoi(arg1);
                if (parsed > 0) {
                    steps = static_cast<std::size_t>(parsed);
                }
                if (ss >> arg2) {
                    try {
                        int parsedDelay = std::stoi(arg2);
                        if (parsedDelay > 0) {
                            delay = parsedDelay;
                        }
                    } catch (...) {
                        // Invalid delay, use default
                    }
                }
            } catch (...) {
                // Invalid number, use default
            }
        }
        return std::make_unique<AnimateCommand>(steps, delay);
    }

    if (cmd == "dump" || cmd == "d") {
        std::string filename;
        if (ss >> filename) {
            return std::make_unique<DumpCommand>(filename);
        }
        return std::make_unique<DumpCommand>("");
    }

    return nullptr;
}

