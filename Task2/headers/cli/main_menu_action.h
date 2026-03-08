#pragma once

#include <string>

enum class MainMenuAction {
    Exit = 0,
    TakeProject = 1,
    BuyMachine = 2,
    BuyResources = 3,
    MyMachines = 4,
    MyProjects = 5,
    Stats = 6,
    SimulateWeek = 7,
    Invalid = -1,
};

MainMenuAction parseMainMenuAction(const std::string& token);
