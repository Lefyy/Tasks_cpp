#pragma once

#include <ostream>
#include <vector>

#include "command.h"
#include "../../service/construction_service.h"
#include "../../service/simulation_service.h"

class StatsCommand final : public Command {
public:
    StatsCommand(ConstructionService& constructionService,
                 SimulationService& simulationService,
                 std::ostream& output);

    std::string key() const override;
    std::string description() const override;
    void execute(const std::vector<std::string>& args) override;

private:
    ConstructionService& constructionService_;
    SimulationService& simulationService_;
    std::ostream& output_;
};
