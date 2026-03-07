#include "../../headers/app/application.h"

#include "../../headers/cli/commands/assign_machine.h"
#include "../../headers/cli/commands/buy_machine.h"
#include "../../headers/cli/commands/next_week.h"
#include "../../headers/cli/commands/pause_project.h"
#include "../../headers/cli/commands/release_machine.h"
#include "../../headers/cli/commands/stats.h"
#include "../../headers/cli/commands/take_project.h"

#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

ResourcePack makeResources(std::initializer_list<std::pair<ResourceType, int>> items) {
    ResourcePack pack;
    for (const auto& [type, amount] : items) {
        pack.add(type, amount);
    }

    return pack;
}

Project makeProject(int id,
                    const std::string& name,
                    int budget,
                    std::vector<ProjectPhase> phases) {
    return Project{id, name, budget, std::move(phases)};
}

void printProject(const Project& project) {
    std::cout << "id=" << project.getId()
              << " name='" << project.getName() << "'"
              << " state=" << toString(project.getState())
              << " budget=" << project.getBudget() << '\n';
}

void printResourcePack(const ResourcePack& pack) {
    if (pack.values.empty()) {
        std::cout << "  (none)\n";
        return;
    }

    for (const auto& [resourceType, amount] : pack.values) {
        std::cout << "  - " << toString(resourceType) << ": " << amount << '\n';
    }
}

}

Application::Application()
    : company_("Task2 Construction", "Moscow", 2'000'000),
      machineRepository_(),
      projectRepository_(),
      financeService_(company_, machineRepository_),
      constructionService_(company_,
                           projectRepository_,
                           machineRepository_,
                           const_cast<ResourcePack&>(financeService_.stockResources())),
      simulationService_(constructionService_),
      menu_(std::cin, std::cout) {
    registerCommands();
}

void Application::bootstrapDefaults() {
    const std::unordered_map<ResourceType, int> buyPrices{
        {ResourceType::Concrete, 100},
        {ResourceType::Steel, 180},
        {ResourceType::Wood, 70},
        {ResourceType::Fuel, 55},
    };

    financeService_.buyResources(
        makeResources({
            {ResourceType::Concrete, 2'500},
            {ResourceType::Steel, 1'600},
            {ResourceType::Wood, 1'200},
            {ResourceType::Fuel, 3'000},
        }),
        buyPrices);

    financeService_.buyMachine(MachineType::Excavator, 450'000, MachineCondition::Used);
    financeService_.buyMachine(MachineType::Excavator, 470'000, MachineCondition::Used);
    financeService_.buyMachine(MachineType::Bulldozer, 520'000, MachineCondition::Used);
    financeService_.buyMachine(MachineType::Crane, 780'000, MachineCondition::Used);
    financeService_.buyMachine(MachineType::Truck, 210'000, MachineCondition::Used);
    financeService_.buyMachine(MachineType::ConcreteMixer, 260'000, MachineCondition::Used);

    projectRepository_.add(makeProject(
        1,
        "Residential Building",
        2'800'000,
        {
            ProjectPhase{"Ground preparation",
                         {{{MachineType::Excavator, 1}, {MachineType::Bulldozer, 1}},
                          makeResources({{ResourceType::Fuel, 120}}),
                          2}},
            ProjectPhase{"Foundation",
                         {{{MachineType::Excavator, 1}, {MachineType::ConcreteMixer, 1}},
                          makeResources({{ResourceType::Concrete, 260},
                                         {ResourceType::Steel, 40},
                                         {ResourceType::Fuel, 90}}),
                          3}},
            ProjectPhase{"Frame",
                         {{{MachineType::Crane, 1}, {MachineType::Truck, 1}},
                          makeResources({{ResourceType::Concrete, 140},
                                         {ResourceType::Steel, 110},
                                         {ResourceType::Fuel, 75}}),
                          4}},
        }));

    projectRepository_.add(makeProject(
        2,
        "Warehouse",
        1'900'000,
        {
            ProjectPhase{"Site clearing",
                         {{{MachineType::Bulldozer, 1}},
                          makeResources({{ResourceType::Fuel, 80}}),
                          2}},
            ProjectPhase{"Base slab",
                         {{{MachineType::ConcreteMixer, 1}, {MachineType::Truck, 1}},
                          makeResources({{ResourceType::Concrete, 210},
                                         {ResourceType::Steel, 30},
                                         {ResourceType::Fuel, 70}}),
                          3}},
            ProjectPhase{"Assembly",
                         {{{MachineType::Crane, 1}},
                          makeResources({{ResourceType::Steel, 160},
                                         {ResourceType::Wood, 90},
                                         {ResourceType::Fuel, 60}}),
                          3}},
        }));

    projectRepository_.add(makeProject(
        3,
        "Road Section",
        1'400'000,
        {
            ProjectPhase{"Earthwork",
                         {{{MachineType::Excavator, 1}, {MachineType::Truck, 1}},
                          makeResources({{ResourceType::Fuel, 130}}),
                          2}},
            ProjectPhase{"Subgrade",
                         {{{MachineType::Bulldozer, 1}},
                          makeResources({{ResourceType::Concrete, 80},
                                         {ResourceType::Fuel, 90}}),
                          2}},
            ProjectPhase{"Surfacing",
                         {{{MachineType::ConcreteMixer, 1}},
                          makeResources({{ResourceType::Concrete, 170},
                                         {ResourceType::Steel, 20},
                                         {ResourceType::Fuel, 80}}),
                          3}},
        }));
}

void Application::registerCommands() {
    menu_.registerCommand(std::make_shared<NextWeekCommand>(simulationService_, std::cout));
    menu_.registerCommand(std::make_shared<StatsCommand>(constructionService_, simulationService_, std::cout));
    menu_.registerCommand(std::make_shared<TakeProjectCommand>(constructionService_, std::cout));
    menu_.registerCommand(std::make_shared<PauseProjectCommand>(constructionService_, std::cout));
    menu_.registerCommand(std::make_shared<BuyMachineCommand>(financeService_, std::cout));
    menu_.registerCommand(std::make_shared<AssignMachineCommand>(constructionService_, std::cout));
    menu_.registerCommand(std::make_shared<ReleaseMachineCommand>(constructionService_, std::cout));
}

void Application::run() {
    menu_.run();
}
