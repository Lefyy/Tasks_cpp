#pragma once

#include "../domain/company.h"
#include "../domain/project.h"
#include "../repository/machine_repository.h"
#include "../repository/project_repository.h"

class FinanceService {
public:
    FinanceService(Company& company,
                   MachineRepository& machineRepository,
                   ProjectRepository& projectRepository);

    bool buyMachine(MachineType type,
                    int price,
                    MachineCondition condition = MachineCondition::New);

    bool sellMachine(int machineId, int salePrice);

    bool buyResources(const ResourcePack& resources,
                      const std::unordered_map<ResourceType, int>& pricesPerUnit);

    bool sellResources(const ResourcePack& resources,
                       const std::unordered_map<ResourceType, int>& pricesPerUnit);

    ResourcePack& stockResources() noexcept;
    const ResourcePack& stockResources() const noexcept;

    bool takeProject(int projectId);
    bool dropProject(int projectId);

private:
    Company& company_;
    MachineRepository& machineRepository_;
    ProjectRepository& projectRepository_;
    ResourcePack stockResources_;
    int nextMachineId_{1};
};
