#include "../../headers/domain/tool.h"

Tool::Tool(int id, ToolType type, int purchasePrice)
    : id_(id),
      type_(type),
      price_(purchasePrice),
      toolState_(EquipmentState::Available),
      toolCondition_(EquipmentCondition::New),
      assignedProjectId_(-1) {}

int Tool::getId() const {
    return id_;
}

EquipmentCategory Tool::getCategory() const {
    return EquipmentCategory::Tool;
}

std::string Tool::getDisplayName() const {
    return toString(type_);
}

bool Tool::isOperational() const {
    return toolState_ != EquipmentState::Maintenance;
}

EquipmentType Tool::getType() const {
    return type_;
}

ToolType Tool::getToolType() const {
    return type_;
}

int Tool::getPrice() const {
    return price_;
}

EquipmentState Tool::getState() const {
    return toolState_;
}

EquipmentCondition Tool::getCondition() const {
    return toolCondition_;
}

int Tool::getAssignedProjectId() const {
    return assignedProjectId_;
}

void Tool::setState(EquipmentState newState) {
    toolState_ = newState;
}

void Tool::setCondition(EquipmentCondition newCondition) {
    toolCondition_ = newCondition;
}

void Tool::setAssignedProjectId(int projectId) {
    assignedProjectId_ = projectId;
}

void Tool::releaseFromProject() {
    assignedProjectId_ = -1;
    toolState_ = EquipmentState::Available;
}

std::unique_ptr<Equipment> Tool::clone() const {
    return std::make_unique<Tool>(*this);
}

std::string toString(ToolType type) {
    switch (type) {
        case ToolType::Jackhammer:
            return "Jackhammer";
        case ToolType::Generator:
            return "Generator";
        case ToolType::LaserLevel:
            return "LaserLevel";
    }

    return "Unknown ToolType";
}
