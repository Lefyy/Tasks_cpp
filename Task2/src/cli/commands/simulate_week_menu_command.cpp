#include "../../../headers/cli/commands/simulate_week_menu_command.h"

#include <iostream>

SimulateWeekMenuCommand::SimulateWeekMenuCommand(AppContext context)
    : context_(context) {}

std::string SimulateWeekMenuCommand::key() const {
    return "6";
}

std::string SimulateWeekMenuCommand::description() const {
    return "Симулировать неделю";
}

void SimulateWeekMenuCommand::execute(const std::vector<std::string>&) {
    const WeekReport report = context_.simulationService.simulateNextWeek();
    std::cout << "Переход на " << report.weekNumber << " неделю\n";
    std::cout << "Текущий бюджет: " << context_.company.getBalance() << '\n';

    for (const int projectId : report.progressedProjects) {
        const auto project = context_.projectRepository.findById(projectId);
        if (!project.has_value()) {
            continue;
        }

        if (project->getState() == ProjectState::Completed) {
            context_.completedProjectIds.insert(projectId);
            std::cout << "Проект " << projectId << " завершился\n";
        } else {
            std::cout << "Проект " << projectId << " перешел к следующей неделе/стадии\n";
        }
    }
}
