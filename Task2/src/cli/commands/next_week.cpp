#include "../../../headers/cli/commands/next_week.h"

namespace {

void printWeekReport(std::ostream& output, const WeekReport& report) {
    output << "Week #" << report.weekNumber << " simulation complete.\n";

    output << "Progressed projects: ";
    if (report.progressedProjects.empty()) {
        output << "none\n";
    } else {
        for (std::size_t i = 0; i < report.progressedProjects.size(); ++i) {
            output << report.progressedProjects[i] << (i + 1 == report.progressedProjects.size() ? '\n' : ',');
            if (i + 1 != report.progressedProjects.size()) {
                output << ' ';
            }
        }
    }

    output << "Blocked projects: ";
    if (report.blockedProjects.empty()) {
        output << "none\n";
    } else {
        for (std::size_t i = 0; i < report.blockedProjects.size(); ++i) {
            output << report.blockedProjects[i] << (i + 1 == report.blockedProjects.size() ? '\n' : ',');
            if (i + 1 != report.blockedProjects.size()) {
                output << ' ';
            }
        }
    }
}

}  // namespace

NextWeekCommand::NextWeekCommand(SimulationService& simulationService, std::ostream& output)
    : simulationService_(simulationService), output_(output) {
}

std::string NextWeekCommand::key() const {
    return "next_week";
}

std::string NextWeekCommand::description() const {
    return "Advance simulation by one week";
}

void NextWeekCommand::execute(const std::vector<std::string>& args) {
    if (!args.empty()) {
        output_ << "Usage: next_week\n";
        return;
    }

    const WeekReport report = simulationService_.simulateNextWeek();
    printWeekReport(output_, report);
}
