#include "../../headers/repository/machine_repository.h"

namespace {
std::vector<Machine> collectAvailableMachinesByType(
    const std::unordered_map<int, Machine>& machines,
    MachineType type
) {
    std::vector<Machine> result;
    result.reserve(machines.size());
    for (const auto& [id, machine] : machines) {
        (void)id;
        if (machine.getType() == type && machine.getState() == MachineState::Available) {
            result.push_back(machine);
        }
    }
    return result;
}
} // namespace

void InMemoryMachineRepository::add(const Machine& machine) {
    machines_.insert_or_assign(machine.getId(), machine);
}

bool InMemoryMachineRepository::remove(int machineId) {
    return machines_.erase(machineId) > 0;
}

std::optional<Machine> InMemoryMachineRepository::findById(int machineId) const {
    const auto it = machines_.find(machineId);
    if (it == machines_.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::vector<Machine> InMemoryMachineRepository::findAll() const {
    std::vector<Machine> result;
    result.reserve(machines_.size());
    for (const auto& [id, machine] : machines_) {
        (void)id;
        result.push_back(machine);
    }
    return result;
}

std::vector<Machine> InMemoryMachineRepository::findAvailableByType(MachineType type) const {
    return collectAvailableMachinesByType(machines_, type);
}

bool InMemoryMachineRepository::update(const Machine& machine) {
    const auto it = machines_.find(machine.getId());
    if (it == machines_.end()) {
        return false;
    }
    it->second = machine;
    return true;
}
