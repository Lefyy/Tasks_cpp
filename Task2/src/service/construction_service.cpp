#include "../../headers/service/construction_service.h"

#include <algorithm>
#include <unordered_map>

ConstructionService::ConstructionService(Company& company,
                                         ProjectRepository& projectRepository,
                                         MachineRepository& machineRepository,
                                         ResourcePack& resourceStock)
    : company_(company),
      projectRepository_(projectRepository),
      machineRepository_(machineRepository),
      resourceStock_(resourceStock) {}

bool ConstructionService::takeProject(int projectId) {
    auto project = projectRepository_.findById(projectId);
    if (!project.has_value()) {
        return false;
    }

    if (project->getState() != ProjectState::Draft &&
        project->getState() != ProjectState::Paused) {
        return false;
    }

    project->activate();
    return projectRepository_.update(*project);
}

bool ConstructionService::pauseProject(int projectId) {
    auto project = projectRepository_.findById(projectId);
    if (!project.has_value() || project->getState() != ProjectState::Active) {
        return false;
    }

    project->pause();
    return projectRepository_.update(*project);
}

bool ConstructionService::dropProject(int projectId) {
    auto project = projectRepository_.findById(projectId);
    if (!project.has_value()) {
        return false;
    }

    if (project->getState() == ProjectState::Completed ||
        project->getState() == ProjectState::Cancelled) {
        return false;
    }

    project->cancel();
    if (!projectRepository_.update(*project)) {
        return false;
    }

    const auto assignedIt = assignmentsByProject_.find(projectId);
    if (assignedIt != assignmentsByProject_.end()) {
        for (const int machineId : assignedIt->second) {
            auto machine = machineRepository_.findById(machineId);
            if (machine.has_value()) {
                machine->releaseFromProject();
                machineRepository_.update(*machine);
            }
        }

        assignmentsByProject_.erase(assignedIt);
    }

    return true;
}

bool ConstructionService::assignMachine(int machineId, int projectId) {
    const auto project = projectRepository_.findById(projectId);
    if (!project.has_value()) {
        return false;
    }

    auto machine = machineRepository_.findById(machineId);
    if (!machine.has_value() || machine->getState() != MachineState::Available) {
        return false;
    }

    machine->setState(MachineState::Assigned);
    machine->setAssignedProjectId(projectId);
    if (!machineRepository_.update(*machine)) {
        return false;
    }

    assignmentsByProject_[projectId].push_back(machineId);
    return true;
}

bool ConstructionService::releaseMachine(int machineId) {
    auto machine = machineRepository_.findById(machineId);
    if (!machine.has_value()) {
        return false;
    }

    const int projectId = machine->getAssignedProjectId();
    if (projectId >= 0) {
        auto assignmentIt = assignmentsByProject_.find(projectId);
        if (assignmentIt != assignmentsByProject_.end()) {
            auto& machineIds = assignmentIt->second;
            machineIds.erase(
                std::remove(machineIds.begin(), machineIds.end(), machineId),
                machineIds.end());

            if (machineIds.empty()) {
                assignmentsByProject_.erase(assignmentIt);
            }
        }
    }

    machine->releaseFromProject();
    return machineRepository_.update(*machine);
}

std::unordered_map<int, std::vector<int>> ConstructionService::getAssignments() const {
    return assignmentsByProject_;
}

std::vector<int> ConstructionService::advanceWeek() {
    std::vector<int> progressedProjects;

    const auto activeProjects = projectRepository_.findActive();
    for (auto project : activeProjects) {
        if (!hasRequirements(project)) {
            continue;
        }

        if (!consumeWeeklyResources(project)) {
            continue;
        }

        project.advanceWeek();
        if (projectRepository_.update(project)) {
            progressedProjects.push_back(project.getId());
        }
    }

    return progressedProjects;
}

bool ConstructionService::hasRequirements(const Project& project) const {
    const auto& requiredMachines = project.currentPhase().getRequirements().machines;

    std::unordered_map<MachineType, int> assignedByType;
    const auto assignmentIt = assignmentsByProject_.find(project.getId());
    if (assignmentIt != assignmentsByProject_.end()) {
        for (const int machineId : assignmentIt->second) {
            const auto machine = machineRepository_.findById(machineId);
            if (!machine.has_value()) {
                continue;
            }

            if (machine->getState() == MachineState::Assigned &&
                machine->getAssignedProjectId() == project.getId()) {
                ++assignedByType[machine->getType()];
            }
        }
    }

    for (const auto& [machineType, requiredCount] : requiredMachines) {
        const int assignedCount = assignedByType[machineType];
        if (assignedCount < requiredCount) {
            return false;
        }
    }

    return resourceStock_.hasAtLeast(project.currentPhase().getRequirements().resourcesPerWeek);
}

bool ConstructionService::consumeWeeklyResources(const Project& project) {
    const auto& weeklyResources = project.currentPhase().getRequirements().resourcesPerWeek;
    if (!resourceStock_.hasAtLeast(weeklyResources)) {
        return false;
    }

    for (const auto& [resourceType, amount] : weeklyResources.values) {
        if (!resourceStock_.consume(resourceType, amount)) {
            return false;
        }
    }

    return true;
}
