#include "../../../headers/cli/commands/take_project.h"

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
    output << "Usage: take_project <project_id>\n";
}

void printResult(std::ostream& output, int projectId, bool success) {
    if (success) {
        output << "Project #" << projectId << " has been taken into work.\n";
        return;
    }

    output << "Unable to take project #" << projectId
           << ". Check project state and requirements.\n";
}

}  // namespace

TakeProjectCommand::TakeProjectCommand(ConstructionService& constructionService, std::ostream& output)
    : constructionService_(constructionService), output_(output) {
}

std::string TakeProjectCommand::key() const {
    return "take_project";
}

std::string TakeProjectCommand::description() const {
    return "Take a draft project by id";
}

void TakeProjectCommand::execute(const std::vector<std::string>& args) {
    const auto projectId = parseProjectId(args);
    if (!projectId) {
        printUsage(output_);
        return;
    }

    const bool success = constructionService_.takeProject(*projectId);
    printResult(output_, *projectId, success);
}
