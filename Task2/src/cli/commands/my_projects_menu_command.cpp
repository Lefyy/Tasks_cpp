#include "../../../headers/cli/commands/my_projects_menu_command.h"

#include "../../../headers/cli/command_support.h"

#include <iostream>
#include <sstream>

MyProjectsMenuCommand::MyProjectsMenuCommand(AppContext context)
    : context_(context) {}

std::string MyProjectsMenuCommand::key() const {
    return "7";
}

std::string MyProjectsMenuCommand::description() const {
    return "Мои проекты";
}

void MyProjectsMenuCommand::execute(const std::vector<std::string>&) {
    while (true) {
        const auto projects = context_.projectRepository.findAll();
        for (const auto& project : projects) {
            if (isProjectOwned(project) && project.getState() != ProjectState::Cancelled) {
                printProjectSummary(project);
            }
        }

        printSeparator();
        printSectionHeader("Команды:");
        std::cout << "pause <id>    — Поставить проект на паузу\n"
                  << "continue <id> — Возобновить проект\n"
                  << "drop <id>     — Бросить проект\n"
                  << "resource <id> — Докупить ресурсы для стадии проекта\n"
                  << "assign <id>   — Назначить оборудование на проект\n"
                  << "back          — Выход\n";
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
        int projectId = 0;
        stream >> command >> projectId;

        if (command == "pause") {
            context_.constructionService.pauseProject(projectId);
            continue;
        }

        if (command == "continue") {
            context_.constructionService.continueProject(projectId);
            continue;
        }

        if (command == "drop") {
            if (context_.constructionService.dropProject(projectId) &&
                context_.financeService.dropProject(projectId)) {
                context_.projectRepository.remove(projectId);
                ++context_.droppedProjects;
            }
            continue;
        }

        if (command == "resource") {
            const auto project = context_.projectRepository.findById(projectId);
            if (!project.has_value() || project->isFinished()) {
                continue;
            }

            const auto& needed = project->getCurrentPhase().getRequirements().resourcesPerWeek;
            ResourcePack toBuy;
            const auto& stock = context_.financeService.stockResources();
            for (const auto& [resourceType, amount] : needed.values) {
                const int current = stock.getAmountOf(resourceType);
                if (current < amount) {
                    toBuy.add(resourceType, amount - current);
                }
            }

            if (toBuy.values.empty()) {
                std::cout << "Ресурсов достаточно для завершения текущей стадии\n";
                continue;
            }

            if (!context_.financeService.buyResources(toBuy, resourcePrices())) {
                std::cout << "Недостаточно средств для докупки ресурсов\n";
            }
            continue;
        }

        if (command != "assign") {
            continue;
        }

        const auto project = context_.projectRepository.findById(projectId);
        if (!project.has_value() || project->isFinished()) {
            continue;
        }

        const auto& req = project->getCurrentPhase().getRequirements();
        const auto assignments = context_.constructionService.getAssignments();
        bool hasCandidates = false;

        for (const Equipment* equipment : context_.equipmentRepository.findAll()) {
            if (equipment->getState() != EquipmentState::Available) {
                continue;
            }

            const EquipmentType type = equipment->getType();
            int requiredCount = 0;
            if (std::holds_alternative<MachineType>(type)) {
                const auto reqIt = req.machines.find(std::get<MachineType>(type));
                if (reqIt == req.machines.end()) {
                    continue;
                }
                requiredCount = reqIt->second;
            } else {
                const auto reqIt = req.tools.find(std::get<ToolType>(type));
                if (reqIt == req.tools.end()) {
                    continue;
                }
                requiredCount = reqIt->second;

            }

            const int assignedCount = countAssignedByType(context_.equipmentRepository,
                                                          assignments,
                                                          projectId,
                                                          type);
            if (assignedCount >= requiredCount) {
                continue;
            }

            hasCandidates = true;
            printEquipmentSummary(*equipment);
        }

        if (!hasCandidates) {
            std::cout << "Подходящего свободного оборудования нет\n";
            continue;
        }

        std::cout << "Введите id оборудования для назначения на проект или back для выхода.\n";
        std::string input;
        if (!std::getline(std::cin, input)) {
            return;
        }

        if (input == "b" || input == "back") {
            continue;
        }

        int equipmentId = 0;
        if (!parseInt(input, equipmentId)) {
            continue;
        }

        context_.constructionService.assignEquipment(equipmentId, projectId);
    }
}
