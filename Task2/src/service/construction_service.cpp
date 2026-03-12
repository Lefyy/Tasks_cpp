#include "../../headers/service/construction_service.h"

#include <algorithm>

ConstructionService::ConstructionService(Company& company,
                                         ProjectRepository& projectRepository,
                                         EquipmentRepository& equipmentRepository,
                                         ResourcePack& resourceStock)
    : company_(company),
      projectRepository_(projectRepository),
      equipmentRepository_(equipmentRepository),
      resourceStock_(resourceStock) {}

bool ConstructionService::takeProject(int projectId) {
    auto project = projectRepository_.findById(projectId);
    if (!project.has_value() || project->getState() != ProjectState::Draft) {
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

bool ConstructionService::continueProject(int projectId) {
    auto project = projectRepository_.findById(projectId);
    if (!project.has_value() || project->getState() != ProjectState::Paused) {
        return false;
    }

    project->activate();
    return projectRepository_.update(*project);
}

bool ConstructionService::dropProject(int projectId) {
    auto project = projectRepository_.findById(projectId);
    if (!project.has_value()) {
        return false;
    }

    if (project->getState() == ProjectState::Completed || project->getState() == ProjectState::Cancelled) {
        return false;
    }

    project->cancel();
    if (!projectRepository_.update(*project)) {
        return false;
    }

    const auto assignedIt = assignmentsByProject_.find(projectId);
    if (assignedIt != assignmentsByProject_.end()) {
        for (int equipmentId : assignedIt->second) {
            Equipment* equipment = equipmentRepository_.findById(equipmentId);
            if (equipment != nullptr) {
                equipment->releaseFromProject();
            }
        }
        assignmentsByProject_.erase(assignedIt);
    }

    return true;
}

ConstructionService::AssignEquipmentResult ConstructionService::assignEquipment(int equipmentId, int projectId) {
    const auto project = projectRepository_.findById(projectId);
    if (!project.has_value()) {
        return AssignEquipmentResult::ProjectNotFound;
    }

    if (project->getState() != ProjectState::Active) {
        return AssignEquipmentResult::InvalidProjectState;

    }

    Equipment* equipment = equipmentRepository_.findById(equipmentId);
    if (equipment == nullptr) {
        return AssignEquipmentResult::EquipmentNotFound;
    }

    if (equipment->getState() != EquipmentState::Available) {
        return AssignEquipmentResult::EquipmentBusy;

    }

    equipment->setState(EquipmentState::Assigned);
    equipment->setAssignedProjectId(projectId);
    assignmentsByProject_[projectId].push_back(equipmentId);
    return AssignEquipmentResult::Success;
}

ConstructionService::ReleaseEquipmentResult ConstructionService::releaseEquipment(int equipmentId) {
    Equipment* equipment = equipmentRepository_.findById(equipmentId);
    if (equipment == nullptr) {
        return ReleaseEquipmentResult::EquipmentNotFound;
    }

    if (equipment->getState() != EquipmentState::Assigned || equipment->getAssignedProjectId() < 0) {
        return ReleaseEquipmentResult::EquipmentNotAssigned;

    }

    const int projectId = equipment->getAssignedProjectId();
    auto it = assignmentsByProject_.find(projectId);
    if (it != assignmentsByProject_.end()) {
        auto& ids = it->second;
        ids.erase(std::remove(ids.begin(), ids.end(), equipmentId), ids.end());
        if (ids.empty()) {
            assignmentsByProject_.erase(it);
        }
    }

    equipment->releaseFromProject();
    return ReleaseEquipmentResult::Success;

}

std::unordered_map<int, std::vector<int>> ConstructionService::getAssignments() const {
    return assignmentsByProject_;
}

std::vector<int> ConstructionService::advanceWeek() {
    std::vector<int> progressed;
    for (auto project : projectRepository_.findActive()) {
        if (!hasRequirements(project) || !consumeWeeklyResources(project)) {
            continue;
        }

        project.advanceWeek();
        if (projectRepository_.update(project)) {
            progressed.push_back(project.getId());
        }
    }

    return progressed;
}

bool ConstructionService::hasRequirements(const Project& project) const {
    const auto& req = project.getCurrentPhase().getRequirements();
    std::unordered_map<MachineType, int> assignedMachines;
    std::unordered_map<ToolType, int> assignedTools;

    const auto it = assignmentsByProject_.find(project.getId());
    if (it != assignmentsByProject_.end()) {
        for (int equipmentId : it->second) {
            const Equipment* equipment = equipmentRepository_.findById(equipmentId);
            if (equipment == nullptr || equipment->getState() != EquipmentState::Assigned || equipment->getAssignedProjectId() != project.getId()) {
                continue;
            }

            const EquipmentType type = equipment->getType();
            if (std::holds_alternative<MachineType>(type)) {
                ++assignedMachines[std::get<MachineType>(type)];
            } else {
                ++assignedTools[std::get<ToolType>(type)];
            }
        }
    }

    for (const auto& [type, count] : req.machines) {
        if (assignedMachines[type] < count) return false;
    }

    for (const auto& [type, count] : req.tools) {
        if (assignedTools[type] < count) return false;
    }

    return resourceStock_.hasAtLeast(req.resourcesPerWeek);
}

bool ConstructionService::consumeWeeklyResources(const Project& project) {
    const auto& weekly = project.getCurrentPhase().getRequirements().resourcesPerWeek;
    if (!resourceStock_.hasAtLeast(weekly)) return false;
    for (const auto& [type, amount] : weekly.values) {
        if (!resourceStock_.consume(type, amount)) return false;

    }

    return true;
}
