#include "../../../headers/cli/commands/stats_menu_command.h"

#include "../../../headers/cli/command_support.h"

#include <iostream>
#include <unordered_map>
#include <vector>

StatsMenuCommand::StatsMenuCommand(AppContext context)
    : context_(context) {}

std::string StatsMenuCommand::key() const {
    return "8";
}

std::string StatsMenuCommand::description() const {
    return "Статистика";
}

void StatsMenuCommand::execute(const std::vector<std::string>&) {
    while (true) {
        std::unordered_map<MachineType, int> machineCounts;
        std::unordered_map<ToolType, int> toolCounts;

        for (const Equipment* equipment : context_.equipmentRepository.findAll()) {
            const EquipmentType type = equipment->getType();
            if (std::holds_alternative<MachineType>(type)) {
                ++machineCounts[std::get<MachineType>(type)];
            } else {
                ++toolCounts[std::get<ToolType>(type)];
            }
        }

        std::cout << "Компания: " << context_.company.getName() << '\n'
                  << "Бюджет: " << context_.company.getBalance() << '\n'
                  << "Количество проектов: "
                  << (context_.projectRepository.findAll().size() - context_.projectRepository.findDraft().size())
                  << '\n'
                  << "Машины:\n";

        for (const auto& [machineType, _] : machineCatalog()) {
            std::cout << "  " << toString(machineType) << ": " << machineCounts[machineType] << '\n';
        }

        std::cout << "Инструменты:\n";
        for (const auto& [toolType, _] : toolCatalog()) {
            std::cout << "  " << toString(toolType) << ": " << toolCounts[toolType] << '\n';
        }

        const std::vector<ResourceType> resourceOrder = {
            ResourceType::Concrete,
            ResourceType::Steel,
            ResourceType::Wood,
            ResourceType::Fuel,
        };

        std::cout << "Ресурсы на складе:\n";
        for (const ResourceType resourceType : resourceOrder) {
            std::cout << "  " << toString(resourceType)
                      << ": " << context_.financeService.stockResources().getAmountOf(resourceType)
                      << '\n';
        }

        std::cout << "Успешно завершено проектов: " << context_.completedProjectIds.size() << '\n'
                  << "Брошено проектов: " << context_.droppedProjects << '\n'
                  << "Текущая неделя: " << context_.simulationService.getCurrentWeek() << '\n'
                  << "Выход: back\n";

        std::string input;
        if (!std::getline(std::cin, input)) {
            return;
        }

        if (input == "b" || input == "back") {
            return;
        }
    }
}
