#include "../../headers/domain/equipment.h"
#include "../../headers/domain/machine.h"
#include "../../headers/domain/tool.h"

std::string toString(EquipmentCategory category) {
    switch (category) {
        case EquipmentCategory::Machine:
            return "Machine";
        case EquipmentCategory::Tool:
            return "Tool";
    }

    return "Unknown EquipmentCategory";
}

std::string toString(EquipmentCondition condition) {
    switch (condition) {
        case EquipmentCondition::New:
            return "New";
        case EquipmentCondition::Used:
            return "Used";
    }

    return "Unknown EquipmentCondition";
}

std::string toString(EquipmentState state) {
    switch (state) {
        case EquipmentState::Available:
            return "Available";
        case EquipmentState::Assigned:
            return "Assigned";
        case EquipmentState::Maintenance:
            return "Maintenance";
    }

    return "Unknown EquipmentState";
}

std::string toString(const EquipmentType& type) {
    return std::visit([](const auto& value) { return toString(value); }, type);
}

EquipmentCategory categoryOf(const EquipmentType& type) {
    return std::holds_alternative<MachineType>(type) ? EquipmentCategory::Machine : EquipmentCategory::Tool;
}

