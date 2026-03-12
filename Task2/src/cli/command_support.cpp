#include "../../headers/cli/command_support.h"

#include <iostream>
#include <sstream>

namespace {

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

std::string formatTools(const std::unordered_map<ToolType, int>& tools) {
    if (tools.empty()) {
        return "нет";
    }

    std::ostringstream out;
    bool first = true;
    for (const auto& [toolType, qty] : tools) {
        if (!first) {
            out << ", ";
        }
        first = false;
        out << toString(toolType) << " x" << qty;
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
    printKeyValueRow("Машины", formatMachines(req.machines));
    printKeyValueRow("Инструменты", formatTools(req.tools));
    printKeyValueRow("Ресурсы/неделя", formatResources(req.resourcesPerWeek));
    std::cout << "\n";
}

int salePriceForEquipment(const Equipment& equipment) {
    if (equipment.getCondition() == EquipmentCondition::Used) {
        return equipment.getPrice() / 2;
    }
    return equipment.getPrice();
}

void printEquipmentSummary(const Equipment& equipment) {
    printSeparator();
    printKeyValueRow("ID", std::to_string(equipment.getId()));
    printKeyValueRow("Категория", toString(equipment.getCategory()));
    printKeyValueRow("Тип", toString(equipment.getType()));
    printKeyValueRow("Состояние", toString(equipment.getState()));
    printKeyValueRow("Износ", toString(equipment.getCondition()));
    printKeyValueRow("Проект", std::to_string(equipment.getAssignedProjectId()));
    printKeyValueRow("Цена продажи", std::to_string(salePriceForEquipment(equipment)));
    std::cout << "\n";
}

int countAssignedByType(const InMemoryEquipmentRepository& equipmentRepository,
                        const std::unordered_map<int, std::vector<int>>& assignments,
                        int projectId,
                        const EquipmentType& type) {
    int assigned = 0;
    const auto projectIt = assignments.find(projectId);
    if (projectIt == assignments.end()) {
        return 0;
    }

    for (const int equipmentId : projectIt->second) {
        const Equipment* equipment = equipmentRepository.findById(equipmentId);
        if (equipment == nullptr) {
            continue;
        }

        if (equipment->getType() == type &&
            equipment->getState() == EquipmentState::Assigned &&
            equipment->getAssignedProjectId() == projectId) {
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

const std::unordered_map<ToolType, int>& toolCatalog() {
    static const std::unordered_map<ToolType, int> catalog{
        {ToolType::Jackhammer, 120'000},
        {ToolType::Generator, 175'000},
        {ToolType::LaserLevel, 85'000},
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
