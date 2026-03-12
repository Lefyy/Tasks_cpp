#pragma once

#include <string>

enum class EquipmentCategory {
    Machine,
    Tool,
};

enum class EquipmentCondition {
    New,
    Used
};

enum class EquipmentState {
    Available,
    Assigned,
    Maintenance
};

class Equipment {
public:
    virtual ~Equipment() = default;

    virtual int getId() const = 0;
    virtual EquipmentCategory getCategory() const = 0;
    virtual std::string getDisplayName() const = 0;
    virtual bool isOperational() const = 0;

    virtual int getPrice() const = 0;
    virtual EquipmentState getState() const = 0;
    virtual EquipmentCondition getCondition() const = 0;
    virtual int getAssignedProjectId() const = 0;

    virtual void setState(EquipmentState newState) = 0;
    virtual void setCondition(EquipmentCondition newCondition) = 0;
    virtual void setAssignedProjectId(int projectId) = 0;
    virtual void releaseFromProject() = 0;

};

std::string toString(EquipmentCategory category);