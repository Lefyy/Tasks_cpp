#include "../../../headers/cli/commands/take_project_menu_command.h"

#include "../../../headers/cli/command_support.h"

#include <iostream>

TakeProjectMenuCommand::TakeProjectMenuCommand(AppContext context)
    : context_(context) {}

std::string TakeProjectMenuCommand::key() const {
    return "1";
}

std::string TakeProjectMenuCommand::description() const {
    return "Взять проект";
}

void TakeProjectMenuCommand::execute(const std::vector<std::string>&) {
    std::string statusLine = "Введите id проекта, который хотите взять, или back для выхода.";

    while (true) {
        const auto projects = context_.projectRepository.findAll();
        for (const auto& project : projects) {
            if (project.getState() == ProjectState::Draft) {
                printProjectSummary(project);
            }
        }

        std::cout << statusLine << '\n';
        std::string input;
        if (!std::getline(std::cin, input)) {
            return;
        }

        if (input == "b" || input == "back") {
            return;
        }

        int projectId = 0;
        if (!parseInt(input, projectId)) {
            continue;
        }

        if (!context_.constructionService.takeProject(projectId)) {
            continue;
        }

        if (!context_.financeService.takeProject(projectId)) {
            continue;
        }

        statusLine = "Проект '" + std::to_string(projectId) +
                     "' успешно взят. Введите id проекта, который хотите взять, или back для выхода.";

    }
}
