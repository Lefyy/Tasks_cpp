#pragma once

#include <ostream>
#include <vector>

#include "command.h"
#include "../../service/construction_service.h"

class AssignMachineCommand final : public Command {
public:
    AssignMachineCommand(ConstructionService& constructionService, std::ostream& output);

    std::string key() const override;
    std::string description() const override;
    void execute(const std::vector<std::string>& args) override;

private:
    ConstructionService& constructionService_;
    std::ostream& output_;
};
