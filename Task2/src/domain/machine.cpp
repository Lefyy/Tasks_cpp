#include "../../headers/domain/machine.h"

Machine::Machine(int id, MachineType type, int purchasePrice)
    : id_(id),
      type_(type),
      price_(purchasePrice),
      machineState_(MachineState::Available),
      machineCondition_(MachineCondition::New),
      assignedProjectId_(-1) {}

int Machine::getId() const {
    return id_;
}

MachineType Machine::getType() const {
    return type_;
}

int Machine::getPrice() const {
    return price_;
}

MachineState Machine::getState() const {
    return machineState_;
}

MachineCondition Machine::getCondition() const {
    return machineCondition_;
}

int Machine::getAssignedProjectId() const {
    return assignedProjectId_;
}

void Machine::setState(MachineState newState) {
    machineState_ = newState;
}

void Machine::setCondition(MachineCondition newCondition) {
    machineCondition_ = newCondition;
}

void Machine::setAssignedProjectId(int projectId) {
    assignedProjectId_ = projectId;
}

void Machine::releaseFromProject() {
    assignedProjectId_ = -1;
    machineState_ = MachineState::Available;
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

std::string toString(MachineCondition condition) {
    switch (condition) {
        case MachineCondition::New:
            return "New";
        case MachineCondition::Used:
            return "Used";
    }

    return "Unknown MachineCondition";
}

std::string toString(MachineState state) {
    switch (state) {
        case MachineState::Available:
            return "Available";
        case MachineState::Assigned:
            return "Assigned";
        case MachineState::Maintenance:
            return "Maintenance";
    }

    return "Unknown MachineState";
}
