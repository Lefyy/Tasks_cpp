#include "../../headers/cli/main_menu_action.h"

MainMenuAction parseMainMenuAction(const std::string& token) {
    if (token == std::to_string(static_cast<int>(MainMenuAction::Exit))) {
        return MainMenuAction::Exit;
    }
    if (token == std::to_string(static_cast<int>(MainMenuAction::TakeProject))) {
        return MainMenuAction::TakeProject;
    }
    if (token == std::to_string(static_cast<int>(MainMenuAction::BuyMachine))) {
        return MainMenuAction::BuyMachine;
    }
    if (token == std::to_string(static_cast<int>(MainMenuAction::BuyResources))) {
        return MainMenuAction::BuyResources;
    }
    if (token == std::to_string(static_cast<int>(MainMenuAction::MyMachines))) {
        return MainMenuAction::MyMachines;
    }
    if (token == std::to_string(static_cast<int>(MainMenuAction::MyProjects))) {
        return MainMenuAction::MyProjects;
    }
    if (token == std::to_string(static_cast<int>(MainMenuAction::Stats))) {
        return MainMenuAction::Stats;
    }
    if (token == std::to_string(static_cast<int>(MainMenuAction::SimulateWeek))) {
        return MainMenuAction::SimulateWeek;
    }

    return MainMenuAction::Invalid;
}
