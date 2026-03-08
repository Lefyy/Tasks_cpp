#include "../../../headers/cli/commands/my_machines_menu_command.h"

#include "../../../headers/cli/command_support.h"

#include <iostream>
#include <sstream>

MyMachinesMenuCommand::MyMachinesMenuCommand(AppContext context)
    : context_(context) {}

std::string MyMachinesMenuCommand::key() const {
    return "3";
}

std::string MyMachinesMenuCommand::description() const {
    return "Моя техника";
}

void MyMachinesMenuCommand::execute(const std::vector<std::string>&) {
    while (true) {
        const auto machines = context_.machineRepository.findAll();
        for (const auto& machine : machines) {
            printMachineSummary(machine);
        }

        printSectionHeader("Команды:");
        std::cout << "sell <id>   - Продать машину\n"
                  << "assign <id> - Назначить машину на проект\n"
                  << "b           - Выход\n";
        printSeparator();
        std::cout << "\n";

        std::string line;
        if (!std::getline(std::cin, line)) {
            return;
        }

        if (line == "b") {
            return;
        }

        std::istringstream stream(line);
        std::string command;
        int machineId = 0;
        stream >> command >> machineId;

        if (command == "sell") {
            const auto machine = context_.machineRepository.findById(machineId);
            if (!machine.has_value()) {
                continue;
            }

            if (machine->getState() == MachineState::Assigned) {
                context_.constructionService.releaseMachine(machineId);
            }

            if (context_.financeService.sellMachine(machineId, salePriceForMachine(*machine))) {
                std::cout << "Вы успешно продали машину\n";
            }
            continue;
        }

        if (command != "assign") {
            continue;
        }

        const auto machine = context_.machineRepository.findById(machineId);
        if (!machine.has_value() || machine->getState() != MachineState::Available) {
            continue;
        }

        std::vector<Project> suitableProjects;
        const auto projects = context_.projectRepository.findAll();
        const auto assignments = context_.constructionService.getAssignments();

        for (const auto& project : projects) {
            if (!isProjectOwned(project) || project.isFinished() || project.getState() == ProjectState::Cancelled) {
                continue;
            }

            const auto& reqMachines = project.getCurrentPhase().getRequirements().machines;
            const auto reqIt = reqMachines.find(machine->getType());
            if (reqIt == reqMachines.end()) {
                continue;
            }

            const int assignedCount = countAssignedByType(context_.machineRepository,
                                                          assignments,
                                                          project.getId(),
                                                          machine->getType());
            if (assignedCount >= reqIt->second) {
                continue;
            }

            suitableProjects.push_back(project);
        }

        if (suitableProjects.empty()) {
            std::cout << "Такая машина пока не требуется ни на каком проекте, нажмите b для выхода\n";
            std::string back;
            while (std::getline(std::cin, back)) {
                if (back == "b") {
                    break;
                }
            }
            continue;
        }

        for (const auto& project : suitableProjects) {
            printProjectSummary(project);
        }

        std::cout << "Введите айди проекта для назначения машины или b для выхода\n";
        std::string input;
        if (!std::getline(std::cin, input)) {
            return;
        }

        if (input == "b") {
            continue;
        }

        int projectId = 0;
        if (!parseInt(input, projectId)) {
            continue;
        }

        context_.constructionService.assignMachine(machineId, projectId);
    }
}
