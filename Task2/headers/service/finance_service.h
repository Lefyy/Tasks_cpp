#pragma once

#include "../domain/company.h"
#include "../domain/project.h"
#include "../repository/machine_repository.h"

class FinanceService {
public:
    FinanceService(Company& company,
                   MachineRepository& machineRepository);

    bool buyMachine(MachineType type,
                    int price,
                    MachineCondition condition = MachineCondition::New);

    bool sellMachine(int machineId, int salePrice);

    bool buyResources(const ResourcePack& resources,
                      const std::unordered_map<ResourceType, int>& pricesPerUnit);

    bool sellResources(const ResourcePack& resources,
                       const std::unordered_map<ResourceType, int>& pricesPerUnit);

    const ResourcePack& stockResources() const noexcept;

private:
    Company& company_;
    MachineRepository& machineRepository_;
    ResourcePack stockResources_;
    int nextMachineId_{1};
};
