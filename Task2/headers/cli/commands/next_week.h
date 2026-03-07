#pragma once

#include <ostream>
#include <vector>

#include "command.h"
#include "../../service/simulation_service.h"

class NextWeekCommand final : public Command {
public:
    NextWeekCommand(SimulationService& simulationService, std::ostream& output);

    std::string key() const override;
    std::string description() const override;
    void execute(const std::vector<std::string>& args) override;

private:
    SimulationService& simulationService_;
    std::ostream& output_;
};
