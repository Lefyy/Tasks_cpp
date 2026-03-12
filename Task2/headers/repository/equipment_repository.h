#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "repository.h"
#include "../domain/equipment.h"
#include "../domain/machine.h"
#include "../domain/tool.h"

class EquipmentRepository : public Repository {
public:
    virtual ~EquipmentRepository() = default;

    virtual void addEquipment(const Equipment& equipment) = 0;
    virtual bool removeEquipment(int equipmentId) = 0;
    virtual std::optional<Equipment> findEquipmentById(int equipmentId) const = 0;
    virtual std::vector<Equipment> findAllEquipment() const = 0;
    virtual std::vector<Equipment> findAvailableEquipmentByCategory(EquipmentCategory category) const = 0;
    virtual std::vector<Equipment> findAllEquipmentByType(EquipmentType type) const = 0;
    virtual bool updateEquipment(const Equipment& equipment) = 0;
};

class InMemoryEquipmentRepository final : public EquipmentRepository {
public:
    void addEquipment(const Equipment& equipment) override;
    bool removeEquipment(int equipmentId) override;
    std::optional<Equipment> findEquipmentById(int equipmentId) const override;
    std::vector<Equipment> findAllEquipment() const override;
    std::vector<Equipment> findAvailableEquipmentByCategory(EquipmentCategory category) const override;
    std::vector<Equipment> findAllEquipmentByType(EquipmentType type) const override;
    bool updateEquipment(const Equipment& equipment) override;

private:
    std::unordered_map<int, std::unique_ptr<Equipment>> equipment_;
};

