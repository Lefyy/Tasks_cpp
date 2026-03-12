#pragma once

#include <unordered_set>

#include "../domain/company.h"
#include "../repository/equipment_repository.h"
#include "../repository/project_repository.h"
#include "../service/construction_service.h"
#include "../service/finance_service.h"
#include "../service/simulation_service.h"

struct AppContext {
    Company& company;
    InMemoryEquipmentRepository& equipmentRepository;
    InMemoryProjectRepository& projectRepository;
    FinanceService& financeService;
    ConstructionService& constructionService;
    SimulationService& simulationService;
    std::unordered_set<int>& completedProjectIds;
    int& droppedProjects;
};
