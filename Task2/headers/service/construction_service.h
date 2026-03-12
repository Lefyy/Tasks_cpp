#pragma once

#include <unordered_map>
#include <vector>

#include "../domain/company.h"
#include "../repository/project_repository.h"
#include "../repository/equipment_repository.h"

class ConstructionService {
public:
    enum class AssignEquipmentResult {
        Success,
        EquipmentNotFound,
        EquipmentBusy,
        ProjectNotFound,
        InvalidProjectState,
    };

    enum class ReleaseEquipmentResult {
        Success,
        EquipmentNotFound,
        EquipmentNotAssigned,
    };

    ConstructionService(Company& company,
                        ProjectRepository& projectRepository,
                        EquipmentRepository& equipmentRepository,
                        ResourcePack& resourceStock);

    bool takeProject(int projectId);
    bool pauseProject(int projectId);
    bool continueProject(int projectId);
    bool dropProject(int projectId);

    AssignEquipmentResult assignEquipment(int equipmentId, int projectId);
    ReleaseEquipmentResult releaseEquipment(int equipmentId);

    std::unordered_map<int, std::vector<int>> getAssignments() const;

    std::vector<int> advanceWeek();

private:
    bool hasRequirements(const Project& project) const;
    bool consumeWeeklyResources(const Project& project);

    Company& company_;
    ProjectRepository& projectRepository_;
    EquipmentRepository& equipmentRepository_;
    ResourcePack& resourceStock_;
    std::unordered_map<int, std::vector<int>> assignmentsByProject_;
};
