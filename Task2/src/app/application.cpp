#include "../../headers/app/application.h"

#include "../../headers/app/app_context.h"
#include "../../headers/cli/command_support.h"
#include "../../headers/cli/commands/buy_machine_menu_command.h"
#include "../../headers/cli/commands/buy_resources_menu_command.h"
#include "../../headers/cli/commands/buy_tools_menu_command.h"
#include "../../headers/cli/commands/exit_menu_command.h"
#include "../../headers/cli/commands/my_machines_menu_command.h"
#include "../../headers/cli/commands/my_projects_menu_command.h"
#include "../../headers/cli/commands/my_tools_menu_command.h"
#include "../../headers/cli/commands/simulate_week_menu_command.h"
#include "../../headers/cli/commands/stats_menu_command.h"
#include "../../headers/cli/commands/take_project_menu_command.h"

#include <iostream>

namespace {

Project makeProject(int id, const std::string& name, int budget, std::vector<ProjectPhase> phases) {
    return Project{id, name, budget, std::move(phases)};
}

}

Application::Application()
    : company_("Task2 Construction", "Moscow", 2'000'000),
      equipmentRepository_(),
      projectRepository_(),
      financeService_(company_, equipmentRepository_, projectRepository_),
      constructionService_(company_,
                           projectRepository_,
                           equipmentRepository_,
                           const_cast<ResourcePack&>(financeService_.stockResources())),
      simulationService_(constructionService_) {
    registerCommands();
}

void Application::bootstrapDefaults() {
    financeService_.buyResources(
        makeResources({
            {ResourceType::Concrete, 2'500},
            {ResourceType::Steel, 1'600},
            {ResourceType::Wood, 1'200},
            {ResourceType::Fuel, 3'000},
        }),
        resourcePrices());

    projectRepository_.add(makeProject(
        1,
        "Residential Building",
        2'800'000,
        {
            ProjectPhase{"Ground preparation",
                         {{{MachineType::Excavator, 1}, {MachineType::Bulldozer, 1}},
                         {{ToolType::Jackhammer, 1}},
                          makeResources({{ResourceType::Fuel, 120}}),
                          2}},
            ProjectPhase{"Foundation",
                         {{{MachineType::Excavator, 1}, {MachineType::ConcreteMixer, 1}},
                         {{ToolType::LaserLevel, 1}},
                          makeResources({{ResourceType::Concrete, 260},
                                         {ResourceType::Steel, 40},
                                         {ResourceType::Fuel, 90}}),
                          3}},
            ProjectPhase{"Frame",
                         {{{MachineType::Crane, 1}, {MachineType::Truck, 1}},
                         {{ToolType::Generator, 1}},
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
                         {{ToolType::Jackhammer, 1}},
                          makeResources({{ResourceType::Fuel, 80}}),
                          2}},
            ProjectPhase{"Base slab",
                         {{{MachineType::ConcreteMixer, 1}, {MachineType::Truck, 1}},
                         {{ToolType::LaserLevel, 1}},
                          makeResources({{ResourceType::Concrete, 210},
                                         {ResourceType::Steel, 30},
                                         {ResourceType::Fuel, 70}}),
                          3}},
            ProjectPhase{"Assembly",
                         {{{MachineType::Crane, 1}},
                         {{ToolType::Generator, 1}},
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
                         {{ToolType::Jackhammer, 1}},
                          makeResources({{ResourceType::Fuel, 130}}),
                          2}},
            ProjectPhase{"Subgrade",
                         {{{MachineType::Bulldozer, 1}},
                         {{ToolType::LaserLevel, 1}},
                          makeResources({{ResourceType::Concrete, 80},
                                         {ResourceType::Fuel, 90}}),
                          2}},
            ProjectPhase{"Surfacing",
                         {{{MachineType::ConcreteMixer, 1}},
                         {{ToolType::Generator, 1}},
                          makeResources({{ResourceType::Concrete, 170},
                                         {ResourceType::Steel, 20},
                                         {ResourceType::Fuel, 80}}),
                          3}},
        }));
}

void Application::registerCommands() {
    AppContext context{company_,
                       equipmentRepository_,
                       projectRepository_,
                       financeService_,
                       constructionService_,
                       simulationService_,
                       completedProjectIds_,
                       droppedProjects_};

    commands_[MainMenuAction::TakeProject] = std::make_shared<TakeProjectMenuCommand>(context);
    commands_[MainMenuAction::BuyMachine] = std::make_shared<BuyMachineMenuCommand>(context);
    commands_[MainMenuAction::BuyTools] = std::make_shared<BuyToolsMenuCommand>(context);
    commands_[MainMenuAction::BuyResources] = std::make_shared<BuyResourcesMenuCommand>(context);
    commands_[MainMenuAction::MyMachines] = std::make_shared<MyMachinesMenuCommand>(context);
    commands_[MainMenuAction::MyTools] = std::make_shared<MyToolsMenuCommand>(context);
    commands_[MainMenuAction::MyProjects] = std::make_shared<MyProjectsMenuCommand>(context);
    commands_[MainMenuAction::Stats] = std::make_shared<StatsMenuCommand>(context);
    commands_[MainMenuAction::SimulateWeek] = std::make_shared<SimulateWeekMenuCommand>(context);
    commands_[MainMenuAction::Exit] = std::make_shared<ExitMenuCommand>();
}

void Application::printMainMenu() const {
    std::cout << "\n";
    printSectionHeader("Главное меню");
    std::cout << "[1] Взять проект\n"
              << "[2] Купить машины\n"
              << "[3] Купить инструменты\n"
              << "[4] Купить ресурсы\n"
              << "[5] Мои машины\n"
              << "[6] Мои инструменты\n"
              << "[7] Мои проекты\n"
              << "[8] Статистика\n"
              << "[9] Симулировать неделю\n"
              << "[0] Выход\n";
    printSeparator();
    std::cout << "\n";
}

void Application::run() {
    bool running = true;
    while (running) {
        printMainMenu();

        std::string input;
        if (!std::getline(std::cin, input)) {
            return;
        }

        const MainMenuAction action = parseMainMenuAction(input);
        switch (action) {
            case MainMenuAction::TakeProject:
            case MainMenuAction::BuyMachine:
            case MainMenuAction::BuyTools:
            case MainMenuAction::BuyResources:
            case MainMenuAction::MyMachines:
            case MainMenuAction::MyTools:
            case MainMenuAction::MyProjects:
            case MainMenuAction::Stats:
            case MainMenuAction::SimulateWeek: {
                const auto it = commands_.find(action);
                if (it != commands_.end()) {
                    it->second->execute({});
                }
                break;
            }
            case MainMenuAction::Exit:
                running = false;
                break;
            case MainMenuAction::Invalid:
                break;
        }
    }
}
