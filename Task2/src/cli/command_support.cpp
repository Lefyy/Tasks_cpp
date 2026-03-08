#include "../../headers/cli/command_support.h"

#include <iostream>
#include <sstream>

namespace {

constexpr int kKeyColumnWidth = 18;

std::string formatMachines(const std::unordered_map<MachineType, int>& machines) {
    if (machines.empty()) {
        return "нет";
    }

    std::ostringstream out;
    bool first = true;
    for (const auto& [machineType, qty] : machines) {
        if (!first) {
            out << ", ";
        }
        first = false;
        out << toString(machineType) << " x" << qty;
    }

    return out.str();
}

std::string formatResources(const ResourcePack& resources) {
    if (resources.values.empty()) {
        return "нет";
    }

    std::ostringstream out;
    bool first = true;
    for (const auto& [resourceType, amount] : resources.values) {
        if (!first) {
            out << ", ";
        }
        first = false;
        out << toString(resourceType) << "=" << amount;
    }

    return out.str();
}

}


ResourcePack makeResources(std::initializer_list<std::pair<ResourceType, int>> items) {
    ResourcePack pack;
    for (const auto& [type, amount] : items) {
        pack.add(type, amount);
    }
    return pack;
}

bool parseInt(const std::string& value, int& result) {
    try {
        size_t index = 0;
        const int parsed = std::stoi(value, &index);
        if (index != value.size()) {
            return false;
        }
        result = parsed;
        return true;
    } catch (...) {
        return false;
    }
}

bool isProjectOwned(const Project& project) {
    return project.getState() != ProjectState::Draft;
}

void printSeparator() {
    std::cout << "------------------------------------------------------------\n";
}

void printSectionHeader(const std::string& title) {
    printSeparator();
    std::cout << title << "\n";
    printSeparator();
}

void printKeyValueRow(const std::string& key, const std::string& value) {
    std::cout << "  " << key;
    if (static_cast<int>(key.size()) < kKeyColumnWidth) {
        std::cout << std::string(kKeyColumnWidth - static_cast<int>(key.size()), '.');
    }
    std::cout << ": " << value << "\n";
}

void printProjectSummary(const Project& project) {
    printSeparator();
    printKeyValueRow("ID", std::to_string(project.getId()));
    printKeyValueRow("Название", project.getName());
    printKeyValueRow("Бюджет", std::to_string(project.getBudget()));
    printKeyValueRow("Состояние", toString(project.getState()));

    if (project.isFinished()) {
        printKeyValueRow("Этап", "Проект завершен");
        std::cout << "\n";
        return;
    }

    const auto& phase = project.getCurrentPhase();
    const auto& req = phase.getRequirements();
    printKeyValueRow("Этап", phase.getName());
    printKeyValueRow("Прогресс",
                     std::to_string(project.getElapsedWeeksInCurrentPhase()) +
                         "/" +
                         std::to_string(req.durationWeeks) +
                         " недель");
    printKeyValueRow("Техника", formatMachines(req.machines));
    printKeyValueRow("Ресурсы/неделя", formatResources(req.resourcesPerWeek));
    std::cout << "\n";
}

int salePriceForMachine(const Machine& machine) {
    if (machine.getCondition() == MachineCondition::Used) {
        return machine.getPrice() / 2;
    }
    return machine.getPrice();
}

void printMachineSummary(const Machine& machine) {
    printSeparator();
    printKeyValueRow("ID", std::to_string(machine.getId()));
    printKeyValueRow("Тип", toString(machine.getType()));
    printKeyValueRow("Состояние", toString(machine.getState()));
    printKeyValueRow("Износ", toString(machine.getCondition()));
    printKeyValueRow("Проект", std::to_string(machine.getAssignedProjectId()));
    printKeyValueRow("Цена продажи", std::to_string(salePriceForMachine(machine)));
    std::cout << "\n";
}

int countAssignedByType(const InMemoryMachineRepository& machineRepository,
                        const std::unordered_map<int, std::vector<int>>& assignments,
                        int projectId,
                        MachineType type) {
    int assigned = 0;
    const auto projectIt = assignments.find(projectId);
    if (projectIt == assignments.end()) {
        return 0;
    }

    for (const int machineId : projectIt->second) {
        const auto machine = machineRepository.findById(machineId);
        if (!machine.has_value()) {
            continue;
        }

        if (machine->getType() == type &&
            machine->getState() == MachineState::Assigned &&
            machine->getAssignedProjectId() == projectId) {
            ++assigned;
        }
    }

    return assigned;
}

const std::unordered_map<MachineType, int>& machineCatalog() {
    static const std::unordered_map<MachineType, int> catalog{
        {MachineType::Excavator, 900'000},
        {MachineType::Bulldozer, 1'040'000},
        {MachineType::Crane, 1'560'000},
        {MachineType::Truck, 420'000},
        {MachineType::ConcreteMixer, 520'000},
    };

    return catalog;
}

const std::unordered_map<ResourceType, int>& resourcePrices() {
    static const std::unordered_map<ResourceType, int> prices{
        {ResourceType::Concrete, 100},
        {ResourceType::Steel, 180},
        {ResourceType::Wood, 70},
        {ResourceType::Fuel, 55},
    };

    return prices;
}
