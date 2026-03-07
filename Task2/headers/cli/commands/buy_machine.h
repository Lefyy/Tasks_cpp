#pragma once

#include <ostream>
#include <vector>

#include "command.h"
#include "../../service/finance_service.h"

class BuyMachineCommand final : public Command {
public:
    BuyMachineCommand(FinanceService& financeService, std::ostream& output);

    std::string key() const override;
    std::string description() const override;
    void execute(const std::vector<std::string>& args) override;

private:
    FinanceService& financeService_;
    std::ostream& output_;
};
