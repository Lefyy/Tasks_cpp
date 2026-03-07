#pragma once

#include <vector>
#include <unordered_map>
#include <optional>

#include "repository.h"
#include "../domain/machine.h"

class MachineRepository : public Repository {
public:
    virtual ~MachineRepository() = default;

    virtual void add(const Machine& machine) = 0;
    virtual bool remove(int machineId) = 0;
    virtual std::optional<Machine> findById(int machineId) const = 0;
    virtual std::vector<Machine> findAll() const = 0;
    virtual std::vector<Machine> findAvailableByType(MachineType type) const = 0;
    virtual bool update(const Machine& machine) = 0;
};

class InMemoryMachineRepository final : public MachineRepository {
public:
    void add(const Machine& machine) override;
    bool remove(int machineId) override;
    std::optional<Machine> findById(int machineId) const override;
    std::vector<Machine> findAll() const override;
    std::vector<Machine> findAvailableByType(MachineType type) const override;
    bool update(const Machine& machine) override;

private:
    std::unordered_map<int, Machine> machines_;
};
