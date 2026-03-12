#pragma once

#include <string>

#include "equipment.h"

enum class MachineType {
    Excavator,
    Bulldozer,
    Crane,
    Truck,
    ConcreteMixer
};

class Machine final : public Equipment {
public:
    Machine(int id, MachineType type, int purchasePrice);

    int getId() const override;
    EquipmentCategory getCategory() const override;
    std::string getDisplayName() const override;
    bool isOperational() const override;

    EquipmentType getType() const override;
    MachineType getMachineType() const;
    int getPrice() const override;
    EquipmentState getState() const override;
    EquipmentCondition getCondition() const override;
    int getAssignedProjectId() const override;

    void setState(EquipmentState newState) override;
    void setCondition(EquipmentCondition newCondition) override;
    void setAssignedProjectId(int projectId) override;
    void releaseFromProject() override;
    std::unique_ptr<Equipment> clone() const override;

private:
    int id_;
    MachineType type_;
    int price_;
    EquipmentState machineState_;
    EquipmentCondition machineCondition_;
    int assignedProjectId_;
};

std::string toString(MachineType type);

