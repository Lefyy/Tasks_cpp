#include "../../../headers/cli/commands/my_tools_menu_command.h"

#include "../../../headers/cli/command_support.h"

#include <iostream>
#include <sstream>

MyToolsMenuCommand::MyToolsMenuCommand(AppContext context)
    : context_(context) {}

std::string MyToolsMenuCommand::key() const {
    return "6";
}

std::string MyToolsMenuCommand::description() const {
    return "Мои инструменты";
}

void MyToolsMenuCommand::execute(const std::vector<std::string>&) {
    while (true) {
        bool hasTools = false;
        for (const Equipment* equipment : context_.equipmentRepository.findAll()) {
            if (equipment->getCategory() != EquipmentCategory::Tool) {
                continue;
            }

            hasTools = true;
            printEquipmentSummary(*equipment);
        }

        if (!hasTools) {
            std::cout << "У вас пока нет инструментов\n";
        }

        printSeparator();
        printSectionHeader("Команды:");
        std::cout << "sell <id>   — Продать инструмент\n"
                  << "assign <id> — Назначить на проект\n"
                  << "back        — Выход\n";
        printSeparator();
        std::cout << "\n";

        std::string line;
        if (!std::getline(std::cin, line)) {
            return;
        }

        if (line == "b" || line == "back") {
            return;
        }

        std::istringstream stream(line);
        std::string command;
        int equipmentId = 0;
        stream >> command >> equipmentId;

        Equipment* equipment = context_.equipmentRepository.findById(equipmentId);
        if (equipment == nullptr || equipment->getCategory() != EquipmentCategory::Tool) {
            continue;
        }

        if (command == "sell") {
            if (equipment->getState() == EquipmentState::Assigned) {
                context_.constructionService.releaseEquipment(equipmentId);
            }

            if (context_.financeService.sellEquipment(equipmentId, salePriceForEquipment(*equipment))) {
                std::cout << "Вы успешно продали инструмент\n";
            }
            continue;
        }

        if (command != "assign") {
            continue;
        }

        if (equipment->getState() != EquipmentState::Available) {
            continue;
        }

        std::vector<Project> suitableProjects;
        const auto projects = context_.projectRepository.findAll();
        const auto assignments = context_.constructionService.getAssignments();
        const auto toolType = std::get<ToolType>(equipment->getType());

        for (const auto& project : projects) {
            if (!isProjectOwned(project) || project.isFinished() || project.getState() == ProjectState::Cancelled) {
                continue;
            }

            const auto& reqTools = project.getCurrentPhase().getRequirements().tools;
            const auto reqIt = reqTools.find(toolType);
            if (reqIt == reqTools.end()) {
                continue;
            }

            const int assignedCount = countAssignedByType(context_.equipmentRepository,
                                                          assignments,
                                                          project.getId(),
                                                          toolType);
            if (assignedCount >= reqIt->second) {
                continue;
            }

            suitableProjects.push_back(project);
        }

        if (suitableProjects.empty()) {
            std::cout << "Такой инструмент пока не требуется ни на одном проекте. Введите back для выхода.\n";
            std::string back;
            while (std::getline(std::cin, back)) {
                if (back == "b" || back == "back") {
                    break;
                }
            }
            continue;
        }

        for (const auto& project : suitableProjects) {
            printProjectSummary(project);
        }

        std::cout << "Введите id проекта для назначения инструмента или back для выхода.\n";
        std::string input;
        if (!std::getline(std::cin, input)) {
            return;
        }

        if (input == "b" || input == "back") {
            continue;
        }

        int projectId = 0;
        if (!parseInt(input, projectId)) {
            continue;
        }

        context_.constructionService.assignEquipment(equipmentId, projectId);
    }
}
