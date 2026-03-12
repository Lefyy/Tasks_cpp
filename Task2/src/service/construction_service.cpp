#include "../../headers/service/construction_service.h"

#include <algorithm>
#include <unordered_map>

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
    if (!project.has_value()) {
        return false;
    }

    if (project->getState() != ProjectState::Draft) {
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
        for (const int eqId : assignedIt->second) {
            auto equipment = equipmentRepository_.findById(eqId);
            if (equipment.has_value()) {
                equipment->releaseFromProject();
                equipmentRepository_.update(*equipment);
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

    auto equipment = equipmentRepository_.findById(equipmentId);
    if (!equipment.has_value()) {
        return AssignEquipmentResult::EquipmentNotFound;
    }

    if (equipment->getState() != EquipmentState::Available) {
        return AssignEquipmentResult::EquipmentBusy;

    }

    equipment->setState(EquipmentState::Assigned);
    equipment->setAssignedProjectId(projectId);
    if (!equipmentRepository_.update(*equipment)) {
        return AssignEquipmentResult::EquipmentBusy;
    }

    assignmentsByProject_[projectId].push_back(equipmentId);
    return AssignEquipmentResult::Success;
}

ConstructionService::ReleaseEquipmentResult ConstructionService::releaseEquipment(int equipmentId) {
    auto equipment = equipmentRepository_.findById(equipmentId);
    if (!equipment.has_value()) {
        return ReleaseEquipmentResult::EquipmentNotFound;
    }

    if (equipment->getState() != EquipmentState::Assigned || equipment->getAssignedProjectId() < 0) {
        return ReleaseEquipmentResult::EquipmentNotAssigned;

    }

    const int projectId = equipment->getAssignedProjectId();
    auto assignmentIt = assignmentsByProject_.find(projectId);
    if (assignmentIt != assignmentsByProject_.end()) {
        auto& equipmentIds = assignmentIt->second;
        equipmentIds.erase(
            std::remove(equipmentIds.begin(), equipmentIds.end(), equipmentId),
            equipmentIds.end());

        if (equipmentIds.empty()) {
            assignmentsByProject_.erase(assignmentIt);

        }
    }

    equipment->releaseFromProject();
    if (!equipmentRepository_.update(*equipment)) {
        return ReleaseEquipmentResult::EquipmentNotAssigned;
    }

    return ReleaseEquipmentResult::Success;

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
    const auto& requiredEquipments = project.getCurrentPhase().getRequirements().equipments;

    std::unordered_map<EquipmentType, int> assignedByType;
    const auto assignmentIt = assignmentsByProject_.find(project.getId());
    if (assignmentIt != assignmentsByProject_.end()) {
        for (const int equipmentId : assignmentIt->second) {
            const auto equipment = equipmentRepository_.findById(equipmentId);
            if (!equipment.has_value()) {
                continue;
            }

            if (equipment->getState() == EquipmentState::Assigned &&
                equipment->getAssignedProjectId() == project.getId()) {
                ++assignedByType[equipment->getType()];
            }
        }
    }

    for (const auto& [equipmentType, requiredCount] : requiredEquipments) {
        const int assignedCount = assignedByType[equipmentType];
        if (assignedCount < requiredCount) {
            return false;
        }
    }

    return resourceStock_.hasAtLeast(project.getCurrentPhase().getRequirements().resourcesPerWeek);
}

bool ConstructionService::consumeWeeklyResources(const Project& project) {
    const auto& weeklyResources = project.getCurrentPhase().getRequirements().resourcesPerWeek;
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
