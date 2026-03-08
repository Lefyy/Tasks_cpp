#include "../../headers/cli/command_support.h"

#include <iostream>

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

void printProjectSummary(const Project& project) {
    std::cout << "id=" << project.getId() << " | "
              << "name='" << project.getName() << "' | "
              << "budget=" << project.getBudget() << " | "
              << "state=" << toString(project.getState()) << '\n';

    if (project.isFinished()) {
        std::cout << "  Проект завершен\n";
        return;
    }

    const auto& phase = project.getCurrentPhase();
    const auto& req = phase.getRequirements();
    std::cout << "  stage='" << phase.getName() << "' ("
              << project.getElapsedWeeksInCurrentPhase() << "/" << req.durationWeeks
              << " недель)\n";

    std::cout << "  machines: ";
    if (req.machines.empty()) {
        std::cout << "нет\n";
    } else {
        bool first = true;
        for (const auto& [machineType, qty] : req.machines) {
            if (!first) {
                std::cout << ", ";
            }
            first = false;
            std::cout << toString(machineType) << " x" << qty;
        }
        std::cout << '\n';
    }

    std::cout << "  resources/week: ";
    if (req.resourcesPerWeek.values.empty()) {
        std::cout << "нет\n";
        return;
    }

    bool first = true;
    for (const auto& [resourceType, amount] : req.resourcesPerWeek.values) {
        if (!first) {
            std::cout << ", ";
        }
        first = false;
        std::cout << toString(resourceType) << "=" << amount;
    }
    std::cout << '\n';
}

int salePriceForMachine(const Machine& machine) {
    if (machine.getCondition() == MachineCondition::Used) {
        return machine.getPrice() / 2;
    }
    return machine.getPrice();
}

void printMachineSummary(const Machine& machine) {
    std::cout << "id=" << machine.getId()
              << " | type=" << toString(machine.getType())
              << " | condition=" << toString(machine.getCondition())
              << " | state=" << toString(machine.getState())
              << " | project=" << machine.getAssignedProjectId()
              << " | sale_price=" << salePriceForMachine(machine)
              << '\n';
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
