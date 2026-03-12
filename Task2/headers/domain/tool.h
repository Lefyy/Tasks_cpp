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

    ToolType getType() const;
    int getPrice() const;
    EquipmentState getState() const;
    EquipmentCondition getCondition() const;
    int getAssignedProjectId() const;

    void setState(EquipmentState newState);
    void setCondition(EquipmentCondition newCondition);
    void setAssignedProjectId(int projectId);
    void releaseFromProject();

private:
    int id_;
    ToolType type_;
    int price_;
    EquipmentState toolState_;
    EquipmentCondition toolCondition_;
    int assignedProjectId_;
};

std::string toString(ToolType type);
std::string toString(EquipmentCondition condition);
std::string toString(EquipmentState state);
