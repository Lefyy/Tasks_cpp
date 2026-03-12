#include "../../headers/repository/equipment_repository.h"

namespace {
std::vector<Equipment> collectAvailableEquipmentByType(
    const std::unordered_map<int, Equipment>& equipment,
    EquipmentType type
) {
    std::vector<Equipment> result;
    result.reserve(equipment.size());
    for (const auto& [id, equip] : equipment) {
        (void)id;
        if (equip.getType() == type && equip.getState() == EquipmentState::Available) {
            result.push_back(equip);
        }
    }
    return result;
}
} // namespace

void InMemoryEquipmentRepository::add(const Equipment& equipment) {
    equipment_.insert_or_assign(equipment.getId(), equipment);
}

bool InMemoryEquipmentRepository::remove(int equipmentId) {
    return equipment_.erase(equipmentId) > 0;
}

std::optional<Equipment> InMemoryEquipmentRepository::findById(int equipmentId) const {
    const auto it = equipment_.find(equipmentId);
    if (it == equipment_.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::vector<Equipment> InMemoryEquipmentRepository::findAll() const {
    std::vector<Equipment> result;
    result.reserve(equipment_.size());
    for (const auto& [id, equip] : equipment_) {
        (void)id;
        result.push_back(equip);
    }
    return result;
}

std::vector<Equipment> InMemoryEquipmentRepository::findAvailableByType(EquipmentType type) const {
    return collectAvailableEquipmentByType(equipment_, type);
}

bool InMemoryEquipmentRepository::update(const Equipment& equipment) {
    const auto it = equipment_.find(equipment.getId());
    if (it == equipment_.end()) {
        return false;
    }
    it->second = equipment;
    return true;
}
