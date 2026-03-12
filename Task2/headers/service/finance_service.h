#pragma once

#include "../domain/company.h"
#include "../domain/project.h"
#include "../repository/equipment_repository.h"
#include "../repository/project_repository.h"

class FinanceService {
public:
    FinanceService(Company& company,
                   EquipmentRepository& equipmentRepository,
                   ProjectRepository& projectRepository);

    bool buyEquipment(const EquipmentType& type,
                      int price,
                      EquipmentCondition condition = EquipmentCondition::New);

    bool sellEquipment(int equipmentId, int salePrice);

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
    EquipmentRepository& equipmentRepository_;
    ProjectRepository& projectRepository_;
    ResourcePack stockResources_;
    int nextEquipmentId_{1};
};
