#pragma once

#include "Command.h"
#include "Engine.h"
#include "Universe.h"

#include <istream>
#include <ostream>
#include <string>

class ConsoleUI {
public:
    void Run(Universe& universe, std::istream& input, std::ostream& output);

private:
    void ShowUniverse(const Universe& universe, std::ostream& output) const;
    ExecutionResult ProcessCommand(const std::string& commandLine, Universe& universe,
                                   std::ostream& output);

    Engine engine_;
};

