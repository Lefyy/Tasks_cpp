#include "../../headers/service/simulation_service.h"

#include <unordered_set>

SimulationService::SimulationService(ConstructionService& constructionService)
    : constructionService_(constructionService),
      currentWeek_(0) {}

int SimulationService::currentWeek() const noexcept {
    return currentWeek_;
}

WeekReport SimulationService::simulateNextWeek() {
    ++currentWeek_;

    WeekReport report;
    report.weekNumber = currentWeek_;

    const auto assignments = constructionService_.getAssignments();
    std::unordered_set<int> trackedProjects;
    for (const auto& [projectId, _] : assignments) {
        trackedProjects.insert(projectId);
    }

    report.progressedProjects = constructionService_.advanceWeek();

    const std::unordered_set<int> progressedSet(
        report.progressedProjects.begin(),
        report.progressedProjects.end());

    for (const int projectId : trackedProjects) {
        if (progressedSet.find(projectId) == progressedSet.end()) {
            report.blockedProjects.push_back(projectId);
        }
    }

    return report;
}
