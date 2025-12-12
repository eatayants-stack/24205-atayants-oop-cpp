#include "ConsoleUI.h"

#include "Command.h"

#include <sstream>

void ConsoleUI::ShowUniverse(const Universe& universe, std::ostream& output) const {
    output << "Universe: " << universe.Name() << " | Rule: " << universe.GetRule().ToString()
           << " | Iteration: " << universe.Iteration() << "\n";
    output << universe.ToDisplayString() << "\n";
}

ExecutionResult ConsoleUI::ProcessCommand(const std::string& commandLine, Universe& universe,
                                          std::ostream& output) {
    auto command = CommandFactory::Create(commandLine);
    if (!command) {
        output << "Unknown command. Type 'help' for usage.\n";
        HelpCommand help;
        help.Execute(universe, engine_, output);
        return ExecutionResult::Continue;
    }

    // Check if it's an animate command (which handles its own display)
    bool isAnimate = commandLine.find("animate") == 0 || commandLine.find("a ") == 0 ||
                      (commandLine.length() == 1 && commandLine[0] == 'a');

    ExecutionResult result = command->Execute(universe, engine_, output);

    // Show universe after tick commands (but not after animate, which shows it itself)
    if (!isAnimate && (commandLine.find("tick") == 0 || commandLine.find("t ") == 0 ||
                       (commandLine.length() == 1 && commandLine[0] == 't'))) {
        ShowUniverse(universe, output);
    }

    return result;
}

void ConsoleUI::Run(Universe& universe, std::istream& input, std::ostream& output) {
    HelpCommand help;
    help.Execute(universe, engine_, output);
    ShowUniverse(universe, output);
    output << "life> " << std::flush;

    std::string line;
    while (std::getline(input, line)) {
        ExecutionResult result = ProcessCommand(line, universe, output);
        if (result == ExecutionResult::Exit) {
            break;
        }
        output << "life> " << std::flush;
    }
}

