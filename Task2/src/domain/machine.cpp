#include "../../headers/domain/machine.h"

Machine::Machine(int id, MachineType type, int purchasePrice)
    : id_(id),
      type_(type),
      price_(purchasePrice),
      machineState_(EquipmentState::Available),
      machineCondition_(EquipmentCondition::New),
      assignedProjectId_(-1) {}

int Machine::getId() const {
    return id_;
}

EquipmentCategory Machine::getCategory() const {
    return EquipmentCategory::Machine;
}

std::string Machine::getDisplayName() const {
    return toString(type_);
}

bool Machine::isOperational() const {
    return machineState_ != EquipmentState::Maintenance;
}

EquipmentType Machine::getType() const {
    return type_;
}

MachineType Machine::getMachineType() const {
    return type_;
}

int Machine::getPrice() const {
    return price_;
}

EquipmentState Machine::getState() const {
    return machineState_;
}

EquipmentCondition Machine::getCondition() const {
    return machineCondition_;
}

int Machine::getAssignedProjectId() const {
    return assignedProjectId_;
}

void Machine::setState(EquipmentState newState) {
    machineState_ = newState;
}

void Machine::setCondition(EquipmentCondition newCondition) {
    machineCondition_ = newCondition;
}

void Machine::setAssignedProjectId(int projectId) {
    assignedProjectId_ = projectId;
}

void Machine::releaseFromProject() {
    assignedProjectId_ = -1;
    machineState_ = EquipmentState::Available;
}

std::unique_ptr<Equipment> Machine::clone() const {
    return std::make_unique<Machine>(*this);
}

std::string toString(MachineType type) {
    switch (type) {
        case MachineType::Excavator:
            return "Excavator";
        case MachineType::Bulldozer:
            return "Bulldozer";
        case MachineType::Crane:
            return "Crane";
        case MachineType::Truck:
            return "Truck";
        case MachineType::ConcreteMixer:
            return "ConcreteMixer";
    }

    return "Unknown MachineType";
}
