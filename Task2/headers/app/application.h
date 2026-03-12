#pragma once

#include <memory>
#include <map>
#include <unordered_set>

#include "../cli/commands/command.h"
#include "../cli/main_menu_action.h"
#include "../domain/company.h"
#include "../repository/equipment_repository.h"
#include "../repository/project_repository.h"
#include "../service/construction_service.h"
#include "../service/finance_service.h"
#include "../service/simulation_service.h"

class Application {
public:
    Application();

    void bootstrapDefaults();
    void run();

private:
    void registerCommands();
    void printMainMenu() const;

    Company company_;

    InMemoryEquipmentRepository equipmentRepository_;
    InMemoryProjectRepository projectRepository_;

    FinanceService financeService_;
    ConstructionService constructionService_;
    SimulationService simulationService_;

    std::unordered_set<int> completedProjectIds_;
    int droppedProjects_{0};

    std::map<MainMenuAction, CommandPtr> commands_;
};
