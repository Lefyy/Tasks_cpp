#pragma once

#include "command.h"
#include "../../app/app_context.h"

class MyMachinesMenuCommand : public Command {
public:
    explicit MyMachinesMenuCommand(AppContext context);

    std::string key() const override;
    std::string description() const override;
    void execute(const std::vector<std::string>& args) override;

private:
    AppContext context_;
};
