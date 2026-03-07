#pragma once

#include <memory>
#include <string>
#include <vector>

class Command {
public:
    virtual ~Command() = default;

    virtual std::string key() const = 0;
    virtual std::string description() const = 0;
    virtual void execute(const std::vector<std::string>& args) = 0;
};

using CommandPtr = std::shared_ptr<Command>;
