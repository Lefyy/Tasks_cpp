#include "../../../headers/cli/commands/stats.h"

namespace {

void printAssignments(std::ostream& output,
                      const std::unordered_map<int, std::vector<int>>& assignments) {
    if (assignments.empty()) {
        output << "No machine assignments yet.\n";
        return;
    }

    output << "Machine assignments by project:\n";
    for (const auto& [projectId, machineIds] : assignments) {
        output << "  project #" << projectId << ": ";
        if (machineIds.empty()) {
            output << "no machines\n";
            continue;
        }

        for (std::size_t i = 0; i < machineIds.size(); ++i) {
            output << machineIds[i] << (i + 1 == machineIds.size() ? '\n' : ',');
            if (i + 1 != machineIds.size()) {
                output << ' ';
            }
        }
    }
}

}  // namespace

StatsCommand::StatsCommand(ConstructionService& constructionService,
                           SimulationService& simulationService,
                           std::ostream& output)
    : constructionService_(constructionService),
      simulationService_(simulationService),
      output_(output) {
}

std::string StatsCommand::key() const {
    return "stats";
}

std::string StatsCommand::description() const {
    return "Show basic simulation stats";
}

void StatsCommand::execute(const std::vector<std::string>& args) {
    if (!args.empty()) {
        output_ << "Usage: stats\n";
        return;
    }

    output_ << "Current week: " << simulationService_.currentWeek() << '\n';
    printAssignments(output_, constructionService_.getAssignments());
}
