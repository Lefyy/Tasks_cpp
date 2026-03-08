#pragma once

#include "command.h"
#include "../../app/app_context.h"

class TakeProjectMenuCommand : public Command {
public:
    explicit TakeProjectMenuCommand(AppContext context);

    std::string key() const override;
    std::string description() const override;
    void execute(const std::vector<std::string>& args) override;

private:
    AppContext context_;
};
