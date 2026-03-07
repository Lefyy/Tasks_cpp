#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <unordered_map>

#include "commands/command.h"

class Menu {
public:
    Menu(std::istream& input, std::ostream& output);

    void registerCommand(const CommandPtr& command);
    void run();

private:
    void printHelp() const;

    std::istream& input_;
    std::ostream& output_;
    std::unordered_map<std::string, CommandPtr> commands_;
};
