#pragma once

#include <unordered_map>
#include <vector>

#include "../domain/company.h"
#include "../repository/project_repository.h"
#include "../repository/machine_repository.h"

class ConstructionService {
public:
    enum class AssignMachineResult {
        Success,
        MachineNotFound,
        MachineBusy,
        ProjectNotFound,
        InvalidProjectState,
    };

    enum class ReleaseMachineResult {
        Success,
        MachineNotFound,
        MachineNotAssigned,
    };

    ConstructionService(Company& company,
                        ProjectRepository& projectRepository,
                        MachineRepository& machineRepository,
                        ResourcePack& resourceStock);

    bool takeProject(int projectId);
    bool pauseProject(int projectId);
    bool dropProject(int projectId);

    AssignMachineResult assignMachine(int machineId, int projectId);
    ReleaseMachineResult releaseMachine(int machineId);

    std::unordered_map<int, std::vector<int>> getAssignments() const;

    std::vector<int> advanceWeek();

private:
    bool hasRequirements(const Project& project) const;
    bool consumeWeeklyResources(const Project& project);

    Company& company_;
    ProjectRepository& projectRepository_;
    MachineRepository& machineRepository_;
    ResourcePack& resourceStock_;
    std::unordered_map<int, std::vector<int>> assignmentsByProject_;
};
