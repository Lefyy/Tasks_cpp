#include "../../../headers/cli/commands/buy_machine_menu_command.h"

#include "../../../headers/cli/command_support.h"

#include <iostream>

BuyMachineMenuCommand::BuyMachineMenuCommand(AppContext context)
    : context_(context) {}

std::string BuyMachineMenuCommand::key() const {
    return "2";
}

std::string BuyMachineMenuCommand::description() const {
    return "Купить технику";
}

void BuyMachineMenuCommand::execute(const std::vector<std::string>&) {
    std::string statusLine = "Введите id машины, которую хотите купить, или back для выхода.";
    
    const std::vector<MachineType> order = {
        MachineType::Excavator,
        MachineType::Bulldozer,
        MachineType::Crane,
        MachineType::Truck,
        MachineType::ConcreteMixer,
    };

    while (true) {
        printSectionHeader("Покупка техники");
        for (std::size_t i = 0; i < order.size(); ++i) {
            const MachineType type = order[i];
            const int newPrice = machineCatalog().at(type);
            printSeparator();
            printKeyValueRow("ID", std::to_string(i + 1));
            printKeyValueRow("Тип", toString(type));
            printKeyValueRow("Состояние", "Новая");
            printKeyValueRow("Цена покупки", std::to_string(newPrice));
            printKeyValueRow("Цена продажи", std::to_string(newPrice / 2));
            std::cout << "\n";

        }

        printSeparator();
        printSectionHeader("Команды:");
        std::cout << "<id>  — Купить выбранную машину\n"
                  << "back  — Выход\n";
        printSeparator();
        std::cout << statusLine << '\n';
        std::string input;
        if (!std::getline(std::cin, input)) {
            return;
        }

        if (input == "b" || input == "back") {
            return;
        }

        int selected = 0;
        if (!parseInt(input, selected) || selected <= 0 || selected > static_cast<int>(order.size())) {
            continue;
        }

        const MachineType type = order[static_cast<std::size_t>(selected - 1)];
        const int price = machineCatalog().at(type);
        const bool purchased = context_.financeService.buyMachine(type, price, MachineCondition::New);

        if (!purchased) {
            statusLine = "У вас недостаточно средств для покупки этой машины. "
                         "Введите id машины, которую хотите купить, или back для выхода.";
            continue;
        }

        int newestId = -1;
        for (const auto& machine : context_.machineRepository.findAll()) {
            if (machine.getId() > newestId) {
                newestId = machine.getId();
            }
        }

        auto boughtMachine = context_.machineRepository.findById(newestId);
        if (boughtMachine.has_value()) {
            boughtMachine->setCondition(MachineCondition::Used);
            context_.machineRepository.update(*boughtMachine);
        }

        statusLine = "Машина успешно куплена. Введите id машины, которую хотите купить, или back для выхода.";
    }
}
