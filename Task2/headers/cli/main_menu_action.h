#pragma once

#include <string>

enum class MainMenuAction {
    Exit = 0,
    TakeProject = 1,
    BuyMachine = 2,
    MyMachines = 3,
    MyProjects = 4,
    Stats = 5,
    SimulateWeek = 6,
    Invalid = -1,
};

MainMenuAction parseMainMenuAction(const std::string& token);
