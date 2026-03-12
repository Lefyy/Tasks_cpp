#pragma once

#include <string>

#include "equipment.h"

enum class ToolType {
    Jackhammer,
    Generator,
    LaserLevel,
};

class Tool final : public Equipment {
public:
    Tool(int id, ToolType type, int purchasePrice);

    int getId() const override;
    EquipmentCategory getCategory() const override;
    std::string getDisplayName() const override;
    bool isOperational() const override;

    EquipmentType getType() const override;
    ToolType getToolType() const;
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
    ToolType type_;
    int price_;
    EquipmentState toolState_;
    EquipmentCondition toolCondition_;
    int assignedProjectId_;
};

std::string toString(ToolType type);
