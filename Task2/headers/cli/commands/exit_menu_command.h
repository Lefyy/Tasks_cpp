#pragma once

#include "command.h"

class ExitMenuCommand : public Command {
public:
    std::string key() const override;
    std::string description() const override;
    void execute(const std::vector<std::string>& args) override;
};
