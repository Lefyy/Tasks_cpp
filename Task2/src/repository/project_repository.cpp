#include "../../headers/repository/project_repository.h"

namespace {
std::vector<Project> collectProjectsByState(
    const std::unordered_map<int, Project>& projects,
    ProjectState state
) {
    std::vector<Project> result;
    result.reserve(projects.size());
    for (const auto& [id, project] : projects) {
        (void)id;
        if (project.getState() == state) {
            result.push_back(project);
        }
    }
    return result;
}
} // namespace

void InMemoryProjectRepository::add(const Project& project) {
    projects_.insert_or_assign(project.getId(), project);
}

bool InMemoryProjectRepository::remove(int projectId) {
    return projects_.erase(projectId) > 0;
}

std::optional<Project> InMemoryProjectRepository::findById(int projectId) const {
    const auto it = projects_.find(projectId);
    if (it == projects_.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::vector<Project> InMemoryProjectRepository::findAll() const {
    std::vector<Project> result;
    result.reserve(projects_.size());
    for (const auto& [id, project] : projects_) {
        (void)id;
        result.push_back(project);
    }
    return result;
}

std::vector<Project> InMemoryProjectRepository::findActive() const {
    return collectProjectsByState(projects_, ProjectState::Active);
}

std::vector<Project> InMemoryProjectRepository::findDraft() const {
    return collectProjectsByState(projects_, ProjectState::Draft);
}

bool InMemoryProjectRepository::update(const Project& project) {
    const auto it = projects_.find(project.getId());
    if (it == projects_.end()) {
        return false;
    }
    it->second = project;
    return true;
}
