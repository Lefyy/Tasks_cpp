#include "../../../headers/cli/commands/assign_machine.h"

#include <optional>
#include <string>

namespace {

std::optional<int> parseId(const std::string& value) {
    try {
        return std::stoi(value);
    } catch (...) {
        return std::nullopt;
    }
}

void printUsage(std::ostream& output) {
    output << "Usage: assign-machine <machine_id> <project_id>\n";
}

void printResult(std::ostream& output,
                 int machineId,
                 int projectId,
                 ConstructionService::AssignMachineResult result) {
    if (result == ConstructionService::AssignMachineResult::Success) {
        output << "Machine #" << machineId << " assigned to project #" << projectId << ".\n";
        return;
    }

    output << "Unable to assign machine #" << machineId << " to project #" << projectId << ": ";
    if (result == ConstructionService::AssignMachineResult::MachineNotFound) {
        output << "machine not found.";
    } else if (result == ConstructionService::AssignMachineResult::MachineBusy) {
        output << "machine is busy.";
    } else if (result == ConstructionService::AssignMachineResult::ProjectNotFound) {
        output << "project not found.";
    } else if (result == ConstructionService::AssignMachineResult::InvalidProjectState) {
        output << "project state does not allow machine assignment.";
    }

    output << '\n';
}

}  // namespace

AssignMachineCommand::AssignMachineCommand(ConstructionService& constructionService, std::ostream& output)
    : constructionService_(constructionService), output_(output) {
}

std::string AssignMachineCommand::key() const {
    return "assign-machine";
}

std::string AssignMachineCommand::description() const {
    return "Assign machine to project: <machine_id> <project_id>";
}

void AssignMachineCommand::execute(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        printUsage(output_);
        return;
    }

    const auto machineId = parseId(args[0]);
    const auto projectId = parseId(args[1]);
    if (!machineId || !projectId) {
        printUsage(output_);
        return;
    }

    const auto result = constructionService_.assignMachine(*machineId, *projectId);
    printResult(output_, *machineId, *projectId, result);
}
