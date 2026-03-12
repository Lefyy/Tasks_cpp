#pragma once

#include <memory>
#include <string>
#include <variant>

enum class MachineType : int;
enum class ToolType : int;

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

using EquipmentType = std::variant<MachineType, ToolType>;

class Equipment {
public:
    virtual ~Equipment() = default;

    virtual int getId() const = 0;
    virtual EquipmentCategory getCategory() const = 0;
    virtual EquipmentType getType() const = 0;
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
    virtual std::unique_ptr<Equipment> clone() const = 0;
};

std::string toString(EquipmentCategory category);
std::string toString(EquipmentCondition condition);
std::string toString(EquipmentState state);
std::string toString(const EquipmentType& type);
EquipmentCategory categoryOf(const EquipmentType& type);
