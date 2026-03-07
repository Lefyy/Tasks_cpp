#pragma once

#include "../cli/menu.h"
#include "../domain/company.h"
#include "../repository/machine_repository.h"
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

    Company company_;

    InMemoryMachineRepository machineRepository_;
    InMemoryProjectRepository projectRepository_;

    FinanceService financeService_;
    ConstructionService constructionService_;
    SimulationService simulationService_;

    Menu menu_;
};
