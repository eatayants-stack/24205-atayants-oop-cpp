#pragma once

#include <memory>
#include <string>

class Universe;
class Engine;

enum class ExecutionResult { Continue, Exit };

class Command {
public:
    virtual ~Command() = default;
    virtual ExecutionResult Execute(Universe& universe, Engine& engine, std::ostream& output) = 0;
};

class TickCommand : public Command {
private:
    std::size_t steps_;

public:
    explicit TickCommand(std::size_t steps = 1);
    ExecutionResult Execute(Universe& universe, Engine& engine, std::ostream& output) override;
};

class AnimateCommand : public Command {
private:
    std::size_t steps_;
    int delay_ms_;

public:
    explicit AnimateCommand(std::size_t steps = 1, int delay_ms = 200);
    ExecutionResult Execute(Universe& universe, Engine& engine, std::ostream& output) override;
};

class DumpCommand : public Command {
private:
    std::string filename_;

public:
    explicit DumpCommand(std::string filename);
    ExecutionResult Execute(Universe& universe, Engine& engine, std::ostream& output) override;
};

class HelpCommand : public Command {
public:
    ExecutionResult Execute(Universe& universe, Engine& engine, std::ostream& output) override;
};

class ExitCommand : public Command {
public:
    ExecutionResult Execute(Universe& universe, Engine& engine, std::ostream& output) override;
};

class CommandFactory {
public:
    static std::unique_ptr<Command> Create(const std::string& commandLine);
};

