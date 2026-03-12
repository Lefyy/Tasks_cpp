#pragma once

#include "command.h"
#include "../../app/app_context.h"

class MyToolsMenuCommand : public Command {
public:
    explicit MyToolsMenuCommand(AppContext context);
    std::string key() const override;
    std::string description() const override;
    void execute(const std::vector<std::string>& args) override;

private:
    AppContext context_;
};
