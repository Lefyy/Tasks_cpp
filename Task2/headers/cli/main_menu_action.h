#pragma once

#include <string>

enum class MainMenuAction {
    Exit = 0,
    TakeProject = 1,
    BuyMachine = 2,
    BuyTools = 3,
    BuyResources = 4,
    MyMachines = 5,
    MyTools = 6,
    MyProjects = 7,
    Stats = 8,
    SimulateWeek = 9,
    Invalid = -1,
};

MainMenuAction parseMainMenuAction(const std::string& token);
