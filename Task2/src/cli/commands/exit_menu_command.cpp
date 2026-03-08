#include "../../../headers/cli/commands/exit_menu_command.h"

std::string ExitMenuCommand::key() const {
    return "0";
}

std::string ExitMenuCommand::description() const {
    return "Выход";
}

void ExitMenuCommand::execute(const std::vector<std::string>&) {
}
