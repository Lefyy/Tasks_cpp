#include "../../headers/app/application.h"

#include <functional>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {
class FunctionalCommand final : public Command {
public:
    using Handler = std::function<void(const std::vector<std::string>&)>;

    FunctionalCommand(std::string key, std::string description, Handler handler)
        : key_(std::move(key)), description_(std::move(description)), handler_(std::move(handler)) {}

    std::string key() const override { return key_; }
    std::string description() const override { return description_; }

    void execute(const std::vector<std::string>& args) override {
        handler_(args);
    }

private:
    std::string key_;
    std::string description_;
    Handler handler_;
};

ResourcePack makeResources(std::initializer_list<std::pair<ResourceType, int>> items) {
    ResourcePack pack;
    for (const auto& [type, amount] : items) {
        pack.add(type, amount);
    }

    return pack;
}

Project makeProject(int id,
                    const std::string& name,
                    int budget,
                    std::vector<ProjectPhase> phases) {
    return Project{id, name, budget, std::move(phases)};
}
}

Application::Application()
    : company_("Task2 Construction", "Moscow", 2'000'000),
      machineRepository_(),
      projectRepository_(),
      financeService_(company_, machineRepository_),
      constructionService_(company_,
                           projectRepository_,
                           machineRepository_,
                           const_cast<ResourcePack&>(financeService_.stockResources())),
      simulationService_(constructionService_),
      menu_(std::cin, std::cout) {
    registerCommands();
}

void Application::bootstrapDefaults() {
    const std::unordered_map<ResourceType, int> buyPrices{
        {ResourceType::Concrete, 100},
        {ResourceType::Steel, 180},
        {ResourceType::Wood, 70},
        {ResourceType::Fuel, 55},
    };

    financeService_.buyResources(
        makeResources({
            {ResourceType::Concrete, 2'500},
            {ResourceType::Steel, 1'600},
            {ResourceType::Wood, 1'200},
            {ResourceType::Fuel, 3'000},
        }),
        buyPrices);

    financeService_.buyMachine(MachineType::Excavator, 450'000, MachineCondition::Used);
    financeService_.buyMachine(MachineType::Excavator, 470'000, MachineCondition::Used);
    financeService_.buyMachine(MachineType::Bulldozer, 520'000, MachineCondition::Used);
    financeService_.buyMachine(MachineType::Crane, 780'000, MachineCondition::Used);
    financeService_.buyMachine(MachineType::Truck, 210'000, MachineCondition::Used);
    financeService_.buyMachine(MachineType::ConcreteMixer, 260'000, MachineCondition::Used);

    projectRepository_.add(makeProject(
        1,
        "Residential Building",
        2'800'000,
        {
            ProjectPhase{"Ground preparation",
                         {{{MachineType::Excavator, 1}, {MachineType::Bulldozer, 1}},
                          makeResources({{ResourceType::Fuel, 120}}),
                          2}},
            ProjectPhase{"Foundation",
                         {{{MachineType::Excavator, 1}, {MachineType::ConcreteMixer, 1}},
                          makeResources({{ResourceType::Concrete, 260},
                                         {ResourceType::Steel, 40},
                                         {ResourceType::Fuel, 90}}),
                          3}},
            ProjectPhase{"Frame",
                         {{{MachineType::Crane, 1}, {MachineType::Truck, 1}},
                          makeResources({{ResourceType::Concrete, 140},
                                         {ResourceType::Steel, 110},
                                         {ResourceType::Fuel, 75}}),
                          4}},
        }));

    projectRepository_.add(makeProject(
        2,
        "Warehouse",
        1'900'000,
        {
            ProjectPhase{"Site clearing",
                         {{{MachineType::Bulldozer, 1}},
                          makeResources({{ResourceType::Fuel, 80}}),
                          2}},
            ProjectPhase{"Base slab",
                         {{{MachineType::ConcreteMixer, 1}, {MachineType::Truck, 1}},
                          makeResources({{ResourceType::Concrete, 210},
                                         {ResourceType::Steel, 30},
                                         {ResourceType::Fuel, 70}}),
                          3}},
            ProjectPhase{"Assembly",
                         {{{MachineType::Crane, 1}},
                          makeResources({{ResourceType::Steel, 160},
                                         {ResourceType::Wood, 90},
                                         {ResourceType::Fuel, 60}}),
                          3}},
        }));

    projectRepository_.add(makeProject(
        3,
        "Road Section",
        1'400'000,
        {
            ProjectPhase{"Earthwork",
                         {{{MachineType::Excavator, 1}, {MachineType::Truck, 1}},
                          makeResources({{ResourceType::Fuel, 130}}),
                          2}},
            ProjectPhase{"Subgrade",
                         {{{MachineType::Bulldozer, 1}},
                          makeResources({{ResourceType::Concrete, 80},
                                         {ResourceType::Fuel, 90}}),
                          2}},
            ProjectPhase{"Surfacing",
                         {{{MachineType::ConcreteMixer, 1}},
                          makeResources({{ResourceType::Concrete, 170},
                                         {ResourceType::Steel, 20},
                                         {ResourceType::Fuel, 80}}),
                          3}},
        }));
}

void Application::registerCommands() {
    menu_.registerCommand(std::make_shared<FunctionalCommand>(
        "help",
        "show available commands",
        [](const std::vector<std::string>&) {
            std::cout << "help, next-week, stats, take <project_id>, pause <project_id>, buy-machine <type> <price>\n";
        }));

    menu_.registerCommand(std::make_shared<FunctionalCommand>(
        "next-week",
        "simulate next week",
        [this](const std::vector<std::string>&) {
            simulationService_.simulateNextWeek();
        }));

    menu_.registerCommand(std::make_shared<FunctionalCommand>(
        "stats",
        "print company stats",
        [this](const std::vector<std::string>&) {
            std::cout << "Current week: " << simulationService_.currentWeek() << '\n';
            std::cout << "Balance: " << company_.getBalance() << '\n';
        }));

    menu_.registerCommand(std::make_shared<FunctionalCommand>(
        "take",
        "take project: take <project_id>",
        [this](const std::vector<std::string>& args) {
            if (args.empty()) {
                return;
            }

            constructionService_.takeProject(std::stoi(args.front()));
        }));

    menu_.registerCommand(std::make_shared<FunctionalCommand>(
        "pause",
        "pause project: pause <project_id>",
        [this](const std::vector<std::string>& args) {
            if (args.empty()) {
                return;
            }

            constructionService_.pauseProject(std::stoi(args.front()));
        }));

    menu_.registerCommand(std::make_shared<FunctionalCommand>(
        "buy-machine",
        "buy machine: buy-machine <type> <price>",
        [this](const std::vector<std::string>& args) {
            if (args.size() < 2) {
                return;
            }

            const auto& typeRaw = args[0];
            const auto price = std::stoi(args[1]);

            MachineType machineType = MachineType::Excavator;
            if (typeRaw == "bulldozer") {
                machineType = MachineType::Bulldozer;
            } else if (typeRaw == "crane") {
                machineType = MachineType::Crane;
            } else if (typeRaw == "truck") {
                machineType = MachineType::Truck;
            } else if (typeRaw == "mixer") {
                machineType = MachineType::ConcreteMixer;
            }

            financeService_.buyMachine(machineType, price, MachineCondition::Used);
        }));
}

void Application::run() {
    menu_.run();
}
