#include "../../headers/domain/equipment.h"

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
