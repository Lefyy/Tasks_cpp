#pragma once

#include <vector>

#include "../service/construction_service.h"

struct WeekReport {
    int weekNumber{};
    std::vector<int> progressedProjects;
    std::vector<int> blockedProjects;
};

class SimulationService {
public:
    explicit SimulationService(ConstructionService& constructionService);

    int currentWeek() const noexcept;
    WeekReport simulateNextWeek();

private:
    ConstructionService& constructionService_;
    int currentWeek_{};
};
