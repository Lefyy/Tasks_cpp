#include "../../headers/repository/equipment_repository.h"

void InMemoryEquipmentRepository::add(const Equipment& equipment) {
    equipment_[equipment.getId()] = equipment.clone();
}

bool InMemoryEquipmentRepository::remove(int equipmentId) {
    return equipment_.erase(equipmentId) > 0;
}

Equipment* InMemoryEquipmentRepository::findById(int equipmentId) {
    const auto it = equipment_.find(equipmentId);
    return it == equipment_.end() ? nullptr : it->second.get();
}

const Equipment* InMemoryEquipmentRepository::findById(int equipmentId) const {
    const auto it = equipment_.find(equipmentId);
    return it == equipment_.end() ? nullptr : it->second.get();
}

std::vector<const Equipment*> InMemoryEquipmentRepository::findAll() const {
    std::vector<const Equipment*> result;
    result.reserve(equipment_.size());
    for (const auto& [id, equipment] : equipment_) {
        (void)id;
        result.push_back(equipment.get());
    }
    return result;
}

std::vector<const Equipment*> InMemoryEquipmentRepository::findAvailableByCategory(EquipmentCategory category) const {
    std::vector<const Equipment*> result;
    for (const auto& [id, equipment] : equipment_) {
        (void)id;
        if (equipment->getCategory() == category && equipment->getState() == EquipmentState::Available) {
            result.push_back(equipment.get());
        }
    }
    return result;

}

std::vector<const Equipment*> InMemoryEquipmentRepository::findAllByType(const EquipmentType& type) const {
    std::vector<const Equipment*> result;
    for (const auto& [id, equipment] : equipment_) {
        (void)id;
        if (equipment->getType() == type) {
            result.push_back(equipment.get());
        }

    }
    return result;
}
