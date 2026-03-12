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

    virtual void add(const Equipment& equipment) = 0;
    virtual bool remove(int equipmentId) = 0;
    virtual Equipment* findById(int equipmentId) = 0;
    virtual const Equipment* findById(int equipmentId) const = 0;
    virtual std::vector<const Equipment*> findAll() const = 0;
    virtual std::vector<const Equipment*> findAvailableByCategory(EquipmentCategory category) const = 0;
    virtual std::vector<const Equipment*> findAllByType(const EquipmentType& type) const = 0;

};

class InMemoryEquipmentRepository final : public EquipmentRepository {
public:
    void add(const Equipment& equipment) override;
    bool remove(int equipmentId) override;
    Equipment* findById(int equipmentId) override;
    const Equipment* findById(int equipmentId) const override;
    std::vector<const Equipment*> findAll() const override;
    std::vector<const Equipment*> findAvailableByCategory(EquipmentCategory category) const override;
    std::vector<const Equipment*> findAllByType(const EquipmentType& type) const override;

private:
    std::unordered_map<int, std::unique_ptr<Equipment>> equipment_;
};

