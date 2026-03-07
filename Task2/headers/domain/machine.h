#pragma once

#include <string>

enum class MachineType {
    Excavator,
    Bulldozer,
    Crane,
    Truck,
    ConcreteMixer
};

enum class MachineCondition {
    New,
    Used
};

enum class MachineState {
    Available,
    Assigned,
    Maintenance
};


class Machine {
public:
    Machine(int id, MachineType type, int purchasePrice);

    int getId() const;
    MachineType getType() const;
    int getPrice() const;
    MachineState getState() const;
    MachineCondition getCondition() const;
    int getAssignedProjectId() const;

    void setState(MachineState newState);
    void setCondition(MachineCondition newCondition);
    void setAssignedProjectId(int projectId);
    void releaseFromProject();

private:
    int id_;
    MachineType type_;
    int price_;
    MachineState machineState_;
    MachineCondition machineCondition_;
    int assignedProjectId_;
};

std::string toString(MachineType type);
std::string toString(MachineCondition condition);
std::string toString(MachineState state);

