#include "../../../headers/cli/commands/pause_project.h"

#include <optional>
#include <string>

namespace {

std::optional<int> parseProjectId(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        return std::nullopt;
    }

    try {
        return std::stoi(args[0]);
    } catch (...) {
        return std::nullopt;
    }
}

void printUsage(std::ostream& output) {
    output << "Usage: pause_project <project_id>\n";
}

void printResult(std::ostream& output, int projectId, bool success) {
    if (success) {
        output << "Project #" << projectId << " paused.\n";
        return;
    }

    output << "Unable to pause project #" << projectId << ".\n";
}

}  // namespace

PauseProjectCommand::PauseProjectCommand(ConstructionService& constructionService, std::ostream& output)
    : constructionService_(constructionService), output_(output) {
}

std::string PauseProjectCommand::key() const {
    return "pause_project";
}

std::string PauseProjectCommand::description() const {
    return "Pause an active project by id";
}

void PauseProjectCommand::execute(const std::vector<std::string>& args) {
    const auto projectId = parseProjectId(args);
    if (!projectId) {
        printUsage(output_);
        return;
    }

    const bool success = constructionService_.pauseProject(*projectId);
    printResult(output_, *projectId, success);
}
