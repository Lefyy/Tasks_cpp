#include "../../../headers/cli/commands/release_machine.h"

#include <optional>
#include <string>

namespace {

std::optional<int> parseId(const std::vector<std::string>& args) {
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
    output << "Usage: release-machine <machine_id>\n";
}

void printResult(std::ostream& output,
                 int machineId,
                 ConstructionService::ReleaseMachineResult result) {
    if (result == ConstructionService::ReleaseMachineResult::Success) {
        output << "Machine #" << machineId << " released from project.\n";
        return;
    }

    output << "Unable to release machine #" << machineId << ": ";
    if (result == ConstructionService::ReleaseMachineResult::MachineNotFound) {
        output << "machine not found.";
    } else if (result == ConstructionService::ReleaseMachineResult::MachineNotAssigned) {
        output << "machine is not assigned.";
    }

    output << '\n';
}

}  // namespace

ReleaseMachineCommand::ReleaseMachineCommand(ConstructionService& constructionService, std::ostream& output)
    : constructionService_(constructionService), output_(output) {
}

std::string ReleaseMachineCommand::key() const {
    return "release-machine";
}

std::string ReleaseMachineCommand::description() const {
    return "Release machine from project: <machine_id>";
}

void ReleaseMachineCommand::execute(const std::vector<std::string>& args) {
    const auto machineId = parseId(args);
    if (!machineId) {
        printUsage(output_);
        return;
    }

    const auto result = constructionService_.releaseMachine(*machineId);
    printResult(output_, *machineId, result);
}
