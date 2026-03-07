#include "../../headers/cli/menu.h"

#include <sstream>
#include <utility>
#include <vector>

namespace {

std::pair<std::string, std::vector<std::string>> parseInputLine(const std::string& line) {
    std::istringstream stream(line);

    std::string key;
    stream >> key;

    std::vector<std::string> args;
    for (std::string arg; stream >> arg;) {
        args.push_back(std::move(arg));
    }

    return {key, args};
}

}  // namespace

Menu::Menu(std::istream& input, std::ostream& output)
    : input_(input), output_(output) {
}

void Menu::registerCommand(const CommandPtr& command) {
    if (!command) {
        return;
    }

    commands_[command->key()] = command;
}

void Menu::run() {
    output_ << "Type 'help' to list commands, 'exit' to quit.\n";

    for (std::string line; std::getline(input_, line);) {
        const auto [key, args] = parseInputLine(line);

        if (key.empty()) {
            continue;
        }

        if (key == "help") {
            printHelp();
            continue;
        }

        if (key == "exit") {
            output_ << "Goodbye!\n";
            return;
        }

        const auto it = commands_.find(key);
        if (it == commands_.end()) {
            output_ << "Unknown command: " << key << ". Type 'help'.\n";
            continue;
        }

        it->second->execute(args);
    }
}

void Menu::printHelp() const {
    output_ << "Available commands:\n";
    for (const auto& [_, command] : commands_) {
        output_ << "  - " << command->key() << ": " << command->description() << '\n';
    }
    output_ << "  - exit: terminate the program\n";
}
